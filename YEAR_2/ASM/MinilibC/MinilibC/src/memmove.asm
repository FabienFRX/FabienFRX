BITS 64

SECTION .text

GLOBAL memmove

memmove:
    mov rax, rdi            ; Store the destination address in rax

    test rdx, rdx           ; Check if the length (rdx) is zero
    jz .done                ; If length is zero, jump to done

    cmp rdi, rsi            ; Compare destination and source addresses
    je .done                ; If they are equal, no need to move memory

    cmp rdi, rsi            ; Compare destination and source addresses
    jb .forward             ; If destination is before source, move forward
    ja .backward            ; If destination is after source, move backward

.forward:
    .loop:
        mov bl, [rsi]       ; Load byte from source address (rsi) into bl
        mov [rdi], bl       ; Store byte from bl to destination address (rdi)
        inc rsi             ; Increment source address
        inc rdi             ; Increment destination address
        dec rdx             ; Decrement length counter
        jnz .loop           ; If length counter is not zero, repeat loop
    jmp .done               ; Jump to done

.backward:
    lea rsi, [rsi + rdx - 1] ; Move source address to end of block
    lea rdi, [rdi + rdx - 1] ; Move destination address to end of block
    .loop2:
        mov bl, [rsi]       ; Load byte from source address (rsi) into bl
        mov [rdi], bl       ; Store byte from bl to destination address (rdi)
        dec rsi             ; Decrement source address
        dec rdi             ; Decrement destination address
        dec rdx             ; Decrement length counter
        jnz .loop2          ; If length counter is not zero, repeat loop
    jmp .done               ; Jump to done

.done:
    ret                     ; Return