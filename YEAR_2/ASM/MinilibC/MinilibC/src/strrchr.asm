BITS 64                 ; 64-bit mode

SECTION .text           ; Code section

GLOBAL strrchr           ; Export the symbol strchr

strrchr:
    xor rax, rax                ; Copy the address of the string to rax
    mov rcx, rdi              ; Copy the search character to rcx

    .loop:
        cmp byte [rcx], 0     ; Compare the byte in dl with the search character
        je .done              ; If they are equal, jump to the found label

        cmp byte [rcx], sil   ; Compare the byte in dl with the search character
        jne .next             ; If they are not equal, jump to the next label
        mov rax, rcx          ; Copy the address of the found character to rax

    .next:
        inc rcx               ; Move to the next byte in the string
        jmp .loop             ; Jump back to the start of the loop

    .done:
        cmp sil, 0            ; Check if the search character is 0 (null terminator)
        jne .return           ; If it is, jump to the return label
        mov rax, rcx          ; Copy the address of the found character to rax

    .return:
        ret                   ; Return
