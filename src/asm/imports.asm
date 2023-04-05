section .data
    import_msg db 'Imports:', 0

section .text
    global _start

_start:
    push dword import_msg
    call printf
    add esp, 4

    push dword [IMAGE_DIRECTORY_ENTRY_IMPORT + ebx]
    call [GetModuleHandle]
    mov ebx, eax

.loop:
    cmp dword [ebx], 0
    je .done

    push dword [ebx + IMAGE_IMPORT_DESCRIPTOR.Name]
    call [printf]
    add esp, 4

    mov ecx, [ebx + IMAGE_IMPORT_DESCRIPTOR.FirstThunk]
.loop2:
    cmp dword [ecx], 0
    je .next

    push dword [ecx + IMAGE_THUNK_DATA.AddressOfData + IMAGE_IMPORT_BY_NAME.Name]
    call [printf]
    add esp, 4

    add ecx, 4
    jmp .loop2

.next:
    add ebx, 20
    jmp .loop

.done:
    push 0
    call [exit]
