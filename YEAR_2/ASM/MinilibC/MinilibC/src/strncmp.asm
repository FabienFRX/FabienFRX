BITS 64

SECTION .text

GLOBAL strncmp

strncmp:
    test rdx, rdx             ; Check if n == 0
    jz .equal                 ; If n is 0, return 0 immediately

.loop:
    mov dl, [rdi]             ; Load character from s1
    mov cl, [rsi]             ; Load character from s2
    cmp dl, cl                ; Compare both characters
    jne .done                 ; If different, jump to done

    test dl, dl               ; Check if end of s1 (null terminator)
    jz .equal                 ; If yes, strings are equal

    dec rdx                   ; Decrement n (number of characters to compare)
    jz .equal                 ; If n reaches 0, strings are considered equal

    inc rdi                   ; Move to the next character in s1
    inc rsi                   ; Move to the next character in s2
    jmp .loop                 ; Continue comparison

.done:
    sub dl, cl                ; Compute ASCII difference between characters
    movsx rax, dl             ; Sign-extend result to 64-bit
    ret

.equal:
    xor rax, rax              ; Return 0 if strings are equal
    ret
