/**
 *  parse the apisetschema.dll of the current machine.
 *  the program should be compiled for 64bit.
 */

#include <windows.h>
#include <stdio.h>

#define SCHEMA_FILENAME L"apisetschema.dll"
#define SCHEMA_SECTION ".apiset"

#define CSV_FILE_SEPERATOR L","
#define CSV_FILE_NEWLINE L"\n"
#define CSV_FILE_HEADERS (L"ApiSet" CSV_FILE_SEPERATOR L"Modules" CSV_FILE_NEWLINE)

typedef struct { /* the header of the apiset section of apisetschema.dll */
    DWORD Ver; /* 6 for windows 10\11 */
    DWORD MapSize;
    DWORD IsSealed; /* bit 0x01 indicate if sealed */
    DWORD SetsNum; /* number of api sets */
    DWORD OffsNspac; /* offset to namespace entries */
    DWORD OffsHash; /* offset to hash entries */
    DWORD MulCompHash; /* multiplier to use when computing hash */
} APISET_HEADER;

typedef struct {
    DWORD IsSealed; /* 0x01 indicate if the api set is sealed */
    DWORD OffsToName; /* offset from start of map to the set name */
    DWORD NameLen; /* the length of the api set name */
    DWORD NameLenNoLast; /* size, in bytes, of name of API Set up to but not including last hyphen */
    DWORD OffsArrHost; /* offset from start of map to array of value entries for hosts  */
    DWORD NumHosts; /* number of hosts */
} APISET_NAMESPACE_ENTRY;

typedef struct { /* the modules that implement the api set */
    DWORD flags; /* only one case of 1 */
    DWORD NameOffset; /* offset to the name of the importing module */
    DWORD NameLength; /* the length of the name */
    DWORD ValueOffset; /* offset to the name of the host module */
    DWORD ValueLength; /* the length of the name */
} APISET_HOST_ENTRY;

__declspec(dllexport) DWORD parser(char *filename)
{
    HANDLE file = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    WriteFile(file, CSV_FILE_HEADERS, sizeof(CSV_FILE_HEADERS)-2, NULL, NULL);

    BYTE *baseAddress = LoadLibraryW(SCHEMA_FILENAME);
    if (baseAddress == NULL) return;

    IMAGE_DOS_HEADER* idh = baseAddress;
    IMAGE_NT_HEADERS64* inth = baseAddress + idh->e_lfanew;
    IMAGE_SECTION_HEADER *ish = (BYTE *)inth + sizeof(IMAGE_NT_HEADERS64);
    while (strcmp(ish->Name, SCHEMA_SECTION) != 0) ish++;

    APISET_HEADER *ash = baseAddress + ish->VirtualAddress;
    BYTE* sectBaseAddress = ash; // base address of the section
    APISET_NAMESPACE_ENTRY *namespaceTab = sectBaseAddress + ash->OffsNspac; // table of ash->SetsNum entries

    DWORD implsets = 0; // the number of implemented sets
    for (DWORD i = 0; i < (ash->SetsNum); i++) {
        WriteFile(file, sectBaseAddress + namespaceTab[i].OffsToName, namespaceTab[i].NameLen, NULL, NULL);
        WriteFile(file, CSV_FILE_SEPERATOR, sizeof(CSV_FILE_SEPERATOR)-2, NULL, NULL);

        APISET_HOST_ENTRY* hostTab = sectBaseAddress + namespaceTab[i].OffsArrHost; // table of namespaceTab[i].NumHosts entries
        WriteFile(file, L"\"", 2, NULL, NULL);
        for (DWORD j = 0; j < namespaceTab[i].NumHosts; j++) {
            if (j != 0) WriteFile(file, CSV_FILE_SEPERATOR, sizeof(CSV_FILE_SEPERATOR) - 2, NULL, NULL);
            else if (hostTab[j].ValueLength != 0) implsets++;
            WriteFile(file, sectBaseAddress + hostTab[j].ValueOffset, hostTab[j].ValueLength, NULL, NULL);
        }
        WriteFile(file, L"\"" CSV_FILE_NEWLINE, 2 + sizeof(CSV_FILE_NEWLINE)-2, NULL, NULL);
    }

    CloseHandle(file);

    return implsets;
}
