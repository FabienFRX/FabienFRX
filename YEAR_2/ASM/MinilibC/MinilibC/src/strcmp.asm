BITS 64

SECTION .text

GLOBAL strcmp

strcmp:
    mov rax, rdi            ; Copy the address of the first string to rax

.loop:
    mov dl, [rdi]           ; Load the byte at address rdi into dl
    mov cl, [rsi]           ; Load the byte at address rsi into cl
    cmp dl, cl              ; Compare the bytes in dl and cl
    jne .done               ; If they are not equal jump to done
    test dl, dl             ; Check if the byte in dl is 0 (null terminator)
    jz .done                ; If it is, jump to the done label
    inc rdi                 ; Move to the next byte in the first string
    inc rsi                 ; Move to the next byte in the second string
    jmp .loop               ; Jump back to the start of the loop

.done:
    sub dl, cl              ; Subtract the bytes in dl and cl
    movsx rax, dl           ; Sign-extend the result to rax
    jz .equal               ; If the result is 0, jump to the equal label
    jl .less                ; If the result is less than 0, jump to the less label
    mov rax, 1              ; Set rax to 1 (first string is greater)
    jmp .return             ; Jump to the return label

.less:
    mov rax, -1             ; Set rax to -1 (first string is less)
    jmp .return             ; Jump to the return label

.equal:
    xor rax, rax            ; Set rax to 0 (strings are equal)

.return:
    ret                     ; Return