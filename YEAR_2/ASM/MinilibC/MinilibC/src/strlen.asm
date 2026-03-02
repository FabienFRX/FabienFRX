BITS 64                             ; 64-bit mode

SECTION .text                       ; Code section

GLOBAL strlen                       ; Export the symbol strlen

strlen:
    xor rax, rax                   ; Clear rax register to use it as a counter

    .loop:
        cmp byte [rdi + rax], 0    ; Compare the byte at address (rdi + rax) with 0 (null terminator)
        je .end                    ; If the byte is 0, jump to the end label
        inc rax                    ; Increment rax to move to the next byte
        jmp .loop                  ; Jump back to the start of the loop

    .end:
        ret                        ; Return, rax now contains the length of the string
