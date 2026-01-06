#!/bin/bash

# --- Configuration ---
COMPILER_PREFIX="arm-none-eabi"
LOAD_ADDR="0x402F0400"

# Output files
ELF_FILE="blinky.elf"
BIN_FILE="blinky.bin"
MLO_FILE="MLO"

# Initialize variables for input files and arrays
ASM_FILE=""
LD_FILE=""
C_FILES=() 

# --- Functions ---

# Function to check if a command exists
check_command() {
    if ! command -v "$1" &> /dev/null
    then
        echo "Error: Command '$1' not found."
        echo "Please ensure the ARM cross-compiler (e.g., arm-none-eabi-gcc) and mkimage are installed."
        exit 1
    fi
}

# Function to clean up intermediate files and the final MLO
clean_build() {
    echo "--- Cleaning generated files ---"
    # Find all generated object files based on the source files provided (if possible)
    # Otherwise, clean common output files.
    OBJ_FILES_TO_REMOVE=$(printf "%s " "${C_FILES[@]/.c/.o}")
    if [ -n "$ASM_FILE" ]; then
        OBJ_FILES_TO_REMOVE="$OBJ_FILES_TO_REMOVE ${ASM_FILE/.s/.o}"
    fi

    rm -f $OBJ_FILES_TO_REMOVE $ELF_FILE $BIN_FILE $MLO_FILE
    
    # Final check cleanup if files were not passed as args
    if [ $# -eq 0 ]; then
        rm -f *.o $ELF_FILE $BIN_FILE $MLO_FILE
    fi
    echo "Cleanup complete."
}

# Function to display usage information
usage() {
    echo "Usage: $0 [-c | --clean] | <assembly_file.s> <linker_script.ld> <c_source_1.c> [c_source_2.c ...]"
    echo "Note: Files can be in any order. Must include one .s, one .ld, and at least one .c"
    echo "  -c, --clean: Only performs cleanup and exits."
    exit 1
}

# --- Argument Parsing ---

# Check for the clean flag
if [ "$1" == "-c" ] || [ "$1" == "--clean" ]; then
    clean_build
    exit 0
fi

# Check for minimum number of build arguments (1 .s + 1 .ld + 1 .c = 3)
if [ "$#" -lt 3 ]; then
    usage
fi

# Parse arguments by file extension
for arg in "$@"; do
    case "$arg" in
        *.s)
            if [ -n "$ASM_FILE" ]; then echo "Error: Only one .s file is allowed."; exit 1; fi
            ASM_FILE="$arg"
            ;;
        *.c)
            C_FILES+=("$arg") 
            ;;
        *.ld)
            if [ -n "$LD_FILE" ]; then echo "Error: Only one .ld file is allowed."; exit 1; fi
            LD_FILE="$arg"
            ;;
        *)
            echo "Error: Unknown file type or extension: $arg"
            exit 1
            ;;
    esac
done

# Final check to ensure required files were found
if [ -z "$ASM_FILE" ] || [ ${#C_FILES[@]} -eq 0 ] || [ -z "$LD_FILE" ]; then
    echo "Error: Missing required files (.s, one or more .c, or .ld)."
    exit 1
fi

# Determine all object files now
ASM_OBJ="${ASM_FILE/.s/.o}"
C_OBJECTS=$(printf "%s " "${C_FILES[@]/.c/.o}")
ALL_OBJECTS="$ASM_OBJ $C_OBJECTS"

# --- Main Build Process ---

echo "--- Starting MLO Build Script ---"

# 1. Check for required tools
check_command "${COMPILER_PREFIX}-as"
check_command "${COMPILER_PREFIX}-gcc"
check_command "${COMPILER_PREFIX}-ld"
check_command "${COMPILER_PREFIX}-objcopy"
check_command "mkimage"

# 2. Assemble Assembly File (.s -> .o)
echo "1. Assembling $ASM_FILE..."
${COMPILER_PREFIX}-as -mcpu=cortex-a8 -g "$ASM_FILE" -o "$ASM_OBJ"
if [ $? -ne 0 ]; then echo "Error during assembly."; clean_build; exit 1; fi

# 3. Compile C Files (.c -> .o)
echo "2. Compiling C files..."
for c_file in "${C_FILES[@]}"; do
    obj_file="${c_file/.c/.o}"
    echo "    Compiling $c_file to $obj_file"
    ${COMPILER_PREFIX}-gcc -mcpu=cortex-a8 -g -c "$c_file" -o "$obj_file"
    if [ $? -ne 0 ]; then echo "Error compiling $c_file."; clean_build; exit 1; fi
done

# 4. Link Object Files (.o -> .elf)
echo "3. Linking object files into $ELF_FILE..."
${COMPILER_PREFIX}-ld -T "$LD_FILE" $ALL_OBJECTS -o "$ELF_FILE"
if [ $? -ne 0 ]; then echo "Error during linking."; clean_build; exit 1; fi

# 5. Create Raw Binary (.elf -> .bin)
echo "4. Creating raw binary $BIN_FILE..."
${COMPILER_PREFIX}-objcopy -O binary "$ELF_FILE" "$BIN_FILE"
if [ $? -ne 0 ]; then echo "Error during objcopy."; clean_build; exit 1; fi

# 6. Format Binary to MLO (Add TI OMAP Header)
echo "5. Adding TI header and creating final $MLO_FILE..."
mkimage -T omapimage -a "$LOAD_ADDR" -d "$BIN_FILE" "$MLO_FILE"
if [ $? -ne 0 ]; then echo "Error during mkimage."; clean_build; exit 1; fi

# 7. Final Output Info
echo "--- Build Successful! ---"
echo "Created $MLO_FILE."
echo "Size of raw binary: $(ls -l $BIN_FILE | awk '{print $5}') bytes"
echo "Size of MLO file:   $(ls -l $MLO_FILE | awk '{print $5}') bytes"

