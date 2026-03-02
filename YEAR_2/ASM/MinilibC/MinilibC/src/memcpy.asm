BITS 64

SECTION .text

GLOBAL memcpy

memcpy:
    mov rax, rdi           ; Store the destination address in rax

    test rdx, rdx          ; Check if the length (rdx) is zero
    jz .done               ; If length is zero, jump to done

.loop:
    mov bl, [rsi]          ; Load byte from source address (rsi) into bl
    mov [rdi], bl          ; Store byte from bl to destination address (rdi)
    inc rsi                ; Increment source address
    inc rdi                ; Increment destination address
    dec rdx                ; Decrement length counter
    jnz .loop              ; If length counter is not zero, repeat loop

.done:
    ret                    ; Return from function