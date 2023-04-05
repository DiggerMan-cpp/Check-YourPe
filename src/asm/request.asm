asm
section .data
    host db "example.com", 0 ; хост
    path db "/redirect", 0 ; пас реквеста
    port dw 80
    user_agent db "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36", 0 ; юзер агент
    request db "GET /redirect HTTP/1.1\r\nHost: example.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36\r\nConnection: close\r\n\r\n", 0

section .bss
    buffer resb 1024 ; тут буфер

section .text
    global _start

_start:
    mov eax, 0x66
    mov ebx, 2
    mov ecx, 1
    mov edx, 0
    int 0x80
    mov esi, eax

    mov eax, 0x66
    mov ebx, esi
    mov ecx, host
    mov edx, port
    push edx
    push ecx
    push ebx
    mov ecx, esp
    mov edx, 16
    int 0x80

    mov eax, 4
    mov ebx, esi
    mov ecx, request
    mov edx, request_len
    int 0x80

    mov eax, 3
    mov ebx, esi
    mov ecx, buffer
    mov edx, 1024
    int 0x80

    mov eax, 4
    mov ebx, 1
    mov ecx, buffer
    mov edx, eax
    int 0x80

    mov eax, 0x6
    mov ebx, esi
    int 0x80

    mov eax, 1
    xor ebx, ebx
    int 0x80
