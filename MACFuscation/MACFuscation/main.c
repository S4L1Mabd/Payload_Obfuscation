#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef NTSTATUS(NTAPI* fnRtlEthernetStringToAddressA)(
    PCSTR S,
    PCSTR* Terminator,
    PVOID Addr
    );

/*obsfucation*/

char* CreateMACAddress(int byte1, int byte2, int byte3, int byte4, int byte5, int byte6) {
    char macFormat[64];
    sprintf(macFormat, "%0.2X-%0.2X-%0.2X-%0.2X-%0.2X-%0.2X", byte1, byte2, byte3, byte4, byte5, byte6);
    return (char*)macFormat;
}

BOOL GenerateMACArray(unsigned char* pCodeBuffer, SIZE_T bufferSize) {
    if (pCodeBuffer == NULL || bufferSize == NULL || bufferSize % 6 != 0) {
        return FALSE;
    }

    printf("char* macArray [%d] = {\n\t", (int)(bufferSize / 6));

    int byteCount = 6, arrayCounter = 0;
    char* macAddress = NULL;

    for (int i = 0; i < bufferSize; i++) {
        if (byteCount == 6) {
            arrayCounter++;
            macAddress = CreateMACAddress(pCodeBuffer[i], pCodeBuffer[i + 1], pCodeBuffer[i + 2], pCodeBuffer[i + 3], pCodeBuffer[i + 4], pCodeBuffer[i + 5]);

            if (i == bufferSize - 6) {
                printf("\"%s\"", macAddress);
                break;
            }
            else {
                printf("\"%s\", ", macAddress);
            }

            byteCount = 1;

            if (arrayCounter % 6 == 0) {
                printf("\n\t");
            }
        }
        else {
            byteCount++;
        }
    }

    printf("\n};\n\n");
    return TRUE;
}


/*debsfucation*/

BOOL DeobfuscateMACArray(IN CHAR* macArray[], IN SIZE_T numElements, OUT PBYTE* deobfuscatedBuffer, OUT SIZE_T* deobfuscatedSize) {
    PBYTE buffer = NULL, tmpBuffer = NULL;
    SIZE_T bufferSize = NULL;
    PCSTR terminator = NULL;
    NTSTATUS status = NULL;

    fnRtlEthernetStringToAddressA pRtlEthernetStringToAddressA =
        (fnRtlEthernetStringToAddressA)GetProcAddress(GetModuleHandle(TEXT("NTDLL")), "RtlEthernetStringToAddressA");

    if (pRtlEthernetStringToAddressA == NULL) {
        printf("[!] GetProcAddress Failed With Error : %d \n", GetLastError());
        return FALSE;
    }

    bufferSize = numElements * 6;
    buffer = (PBYTE)HeapAlloc(GetProcessHeap(), 0, bufferSize);

    if (buffer == NULL) {
        printf("[!] HeapAlloc Failed With Error : %d \n", GetLastError());
        return FALSE;
    }

    tmpBuffer = buffer;

    for (int i = 0; i < numElements; i++) {
        if ((status = pRtlEthernetStringToAddressA(macArray[i], &terminator, tmpBuffer)) != 0x0) {
            printf("[!] RtlEthernetStringToAddressA Failed At [%s] With Error 0x%0.8X", macArray[i], status);
            return FALSE;
        }

        tmpBuffer = (PBYTE)(tmpBuffer + 6);
    }

    *deobfuscatedBuffer = buffer;
    *deobfuscatedSize = bufferSize;

    return TRUE;
}
