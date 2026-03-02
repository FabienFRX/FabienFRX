BITS 64                 ; 64-bit mode

SECTION .text           ; Code section

GLOBAL strchr           ; Export the symbol strchr

strchr:
    mov rax, rdi        ; Copy the address of the string to rax

    .loop:
        mov dl, [rdi]   ; Load the byte at address rdi into dl
        cmp dl, sil     ; Compare the byte in dl with the search character
        je .found       ; If they are equal, jump to the found label

        test dl, dl     ; Check if the byte in dl is 0 (null terminator)
        je .not_found   ; If it is, jump to the not_found label

        inc rdi         ; Move to the next byte in the string
        jmp .loop       ; Jump back to the start of the loop
    
    .found:
        mov rax, rdi    ; Copy the address of the found character to rax
        ret             ; Return
    
    .not_found:
        xor rax, rax    ; Clear rax to 0
        ret             ; Return
