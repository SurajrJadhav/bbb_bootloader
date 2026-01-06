.section .text
.global _start

_start:
	ldr r0, =_stack_top
	mov sp,r0
	bl main	
