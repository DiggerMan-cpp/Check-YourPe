#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

string GetSectionName(IMAGE_SECTION_HEADER* sectionHeader, int sectionIndex)
{
    char sectionName[9] = { 0 };
    memcpy(sectionName, sectionHeader[sectionIndex].Name, 8);
    return sectionName;
}

int main()
{
    string filePath = "123.exe";

    ifstream file(filePath, ios::binary);
    if (!file)
    {
        cout << "Failed to open file" << endl;
        return 1;
    }
    IMAGE_DOS_HEADER dosHeader;
    file.read((char*)&dosHeader, sizeof(dosHeader));
    if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE)
    {
        cout << "DOS header error" << endl;
        return 1;
    }

    IMAGE_NT_HEADERS ntHeaders;
    file.seekg(dosHeader.e_lfanew);
    file.read((char*)&ntHeaders, sizeof(ntHeaders));
    if (ntHeaders.Signature != IMAGE_NT_SIGNATURE)
    {
        cout << "Invalid NT header" << endl;
        return 1;
    }

    IMAGE_SECTION_HEADER* sectionHeader = IMAGE_FIRST_SECTION(&ntHeaders);

   
    cout << "Sections:" << endl;
    for (int i = 0; i < ntHeaders.FileHeader.NumberOfSections; i++)
    {
        cout << "Name: " << GetSectionName(sectionHeader, i) << endl;
        cout << "Virtual address: " << hex << sectionHeader[i].VirtualAddress << endl;
        cout << "Virtual size: " << hex << sectionHeader[i].Misc.VirtualSize << endl;
        cout << "Raw data offset: " << hex << sectionHeader[i].PointerToRawData << endl;
        cout << "Raw data size: " << hex << sectionHeader[i].SizeOfRawData << endl;
        cout << endl;
    }

 
    IMAGE_IMPORT_DESCRIPTOR* importDescriptor = (IMAGE_IMPORT_DESCRIPTOR*)ImageDirectoryEntryToData(ntHeaders.OptionalHeader.DataDirectory, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, NULL);

   
    cout << "Imports:" << endl;
    while (importDescriptor->Name)
    {
        cout << "Library name: " << (char*)((char*)&dosHeader + importDescriptor->Name) << endl;

       
        IMAGE_THUNK_DATA* thunkData = (IMAGE_THUNK_DATA*)((char*)&dosHeader + importDescriptor->FirstThunk);
        while (thunkData->u1.AddressOfData)
        {
            if (thunkData->u1.Ordinal & IMAGE_ORDINAL_FLAG)
            {
                cout << "Ordinal: " << dec << (thunkData->u1.Ordinal & 0xFFFF) << endl;
            }
            else
            {
                IMAGE_IMPORT_BY_NAME* importByName = (IMAGE_IMPORT_BY_NAME*)((char*)&dosHeader + thunkData->u1.AddressOfData);
                cout << "Function name: " << importByName->Name << endl;
            }

            thunkData++;
        }

        importDescriptor++;
    }

   
    IMAGE_EXPORT_DIRECTORY* exportDirectory = (IMAGE_EXPORT_DIRECTORY*)ImageDirectoryEntryToData(ntHeaders.OptionalHeader.DataDirectory, TRUE, IMAGE_DIRECTORY_ENTRY_EXPORT, NULL);

    cout << "Exports:" << endl;
    if (exportDirectory)
    {
        DWORD* addressOfFunctions = (DWORD*)((char*)&dosHeader + exportDirectory->AddressOfFunctions);
        DWORD* addressOfNames = (DWORD*)((char*)&dosHeader + exportDirectory->AddressOfNames);
        WORD* addressOfNameOrdinals = (WORD*)((char*)&dosHeader + exportDirectory->AddressOfNameOrdinals);

        for (int i = 0; i < exportDirectory->NumberOfFunctions; i++)
        {
            cout << "Ordinal: " << dec << (exportDirectory->Base + i) << endl;

            bool hasName = false;
            for (int j = 0; j < exportDirectory->NumberOfNames; j++)
            {
                if (addressOfNameOrdinals[

j] == i)
                {
                    cout << "Function name: " << (char*)((char*)&dosHeader + addressOfNames[j]) << endl;
                    hasName = true;
                    break;
                }
            }

            if (!hasName)
            {
                cout << "No name" << endl;
            }
        }
    }

    return 0;
}
