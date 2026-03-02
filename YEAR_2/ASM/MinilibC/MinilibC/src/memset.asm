BITS 64

SECTION .text

GLOBAL memset

memset:
    mov rax, rdi                ; Copy the address of the memory block to rax
    mov r8b, sil                ; Copy the value to set to r8b

    test rdx, rdx               ; Check if the counter is zero
    jz .done                    ; If it is, jump to the done label

    .loop:
        mov byte [rdi], r8b     ; Set the byte at address rax to the value in cl
        inc rdi                 ; Move to the next byte in the memory block
        dec rdx                 ; Decrement the counter
        jnz .loop               ; If the counter is not zero, jump back to the start of the loop

    .done:
        ret                     ; Return