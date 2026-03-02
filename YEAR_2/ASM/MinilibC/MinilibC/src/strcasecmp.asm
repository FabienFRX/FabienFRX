BITS 64

SECTION .text

GLOBAL strcasecmp

strcasecmp:
    mov rax, rdi                ; Copy the address of the first string to rax

    .loop:
        mov dl, [rdi]           ; Load the byte at address rdi into dl
        mov cl, [rsi]           ; Load the byte at address rsi into cl
        cmp dl, cl              ; Compare the bytes in dl and cl
        je .next                ; If they are equal, jump to the next label

        cmp dl, 0               ; Check if the byte in dl is 0 (null terminator)
        je .end
        cmp cl, 0               ; Check if the byte in cl is 0 (null terminator)
        je .end

        cmp dl, 'a'             ; Check if the byte in dl is lowercase
        jl .check_upper         ; If it is, jump to the check_upper label
        cmp dl, 'z'             ; Check if the byte in dl is uppercase
        jg .check_upper         ; If it is, jump to the check_upper label
        jmp .next               ; Otherwise, move to the next byte

    .check_upper:
        cmp cl, 'a'             ; Check if the byte in cl is lowercase
        jl .compare             ; If it is, jump to the compare label
        cmp cl, 'z'             ; Check if the byte in cl is uppercase
        jg .compare             ; If it is, jump to the compare label
        jmp .next               ; Otherwise, move to the next byte

    .compare:
        sub dl, cl              ; Subtract the bytes in dl and cl
        movsx rax, dl           ; Sign-extend the result to rax
        ret                     ; Return

    .next:
        inc rdi                 ; Move to the next byte in the first string
        inc rsi                 ; Move to the next byte in the second string
        jmp .loop               ; Jump back to the start of the loop

    .end:
        xor rax, rax            ; Set rax to 0 (strings are equal)
        ret                     ; Return