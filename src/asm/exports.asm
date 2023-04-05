section .data
    export_msg db 'Exportes:', 0

section .text
    global _start

_start:
    push dword export_msg
    call printf
    add esp, 4

    push dword [IMAGE_DIRECTORY_ENTRY_EXPORT + ebx]
    call [GetModuleHandle]
    mov ebx, eax

    mov ecx, [ebx + IMAGE_EXPORT_DIRECTORY.AddressOfNames]
    mov edx, [ebx + IMAGE_EXPORT_DIRECTORY.NumberOfNames]
.loop:
    cmp dword edx, 0
    je .done

    push dword [ecx]
    call [printf]
    add esp, 4

    add ecx, 4
    dec edx
    jmp .loop

.done:
    push 0
