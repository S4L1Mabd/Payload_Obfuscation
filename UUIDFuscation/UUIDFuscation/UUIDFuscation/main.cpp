#include <stdio.h>
#include <stdlib.h>
#include <windows.h>



typedef RPC_STATUS(WINAPI* fnUuidFromStringA)(
    RPC_CSTR StringUuid,
    UUID* Uuid
    );

/*ObsFuscation  Roujiiii */

char* GenerateUUID(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p) {

	char MyUUID[128];
	char seg1[32], seg2[32], seg3[32], seg4[32], seg5[32];

	sprintf(seg1, "%0.2x%0.2x%0.2x%0.2x", d, c, b, a);
	sprintf(seg2, "%0.2x%0.2x", f,e);
	sprintf(seg3, "%0.2x%0.2x", h,g);
	sprintf(seg4, "%0.2x%0.2x", i,j);
	sprintf(seg5, "%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x", k,l,m,n,o,p);

	sprintf(MyUUID, "%0.2x-%0.2x-%0.2x-%0.2x-%0.2x", seg1, seg2, seg3, seg4, seg5);

	return (char*)MyUUID;
}

BOOL GenerateUUIDfromShell(char* pShell, size_t sizeofShell) {


	if (pShell == NULL || sizeofShell == NULL || sizeofShell %16 != 0) {
		return FALSE;
	}

    int chunk_size = 16, count = 0;
    char* uuid_str = NULL;
    for (int i = 0; i < sizeofShell; i++) {
        if (chunk_size == 16) {
            count++;
            uuid_str = GenerateUUID(
                pShell[i], pShell[i + 1], pShell[i + 2], pShell[i + 3],
                pShell[i + 4], pShell[i + 5], pShell[i + 6], pShell[i + 7],
                pShell[i + 8], pShell[i + 9], pShell[i + 10], pShell[i + 11],
                pShell[i + 12], pShell[i + 13], pShell[i + 14], pShell[i + 15]
            );
            if (i == sizeofShell - 16) {
                printf("\"%s\"", uuid_str);
                break;
            }
            else {
                printf("\"%s\", ", uuid_str);
            }
            chunk_size = 1;
            if (count % 3 == 0) {
                printf("\n\t");
            }
        }
        else {
            chunk_size++;
        }
    }
    printf("\n};\n\n");
    return TRUE;

}

/*DeObsFuscation  bondo9iyaaa */

BOOL UuidDeobfuscation(IN CHAR* UuidArray[], IN SIZE_T NumElements, OUT PBYTE* ppDAddress, OUT SIZE_T* pDSize) {
    PBYTE pBuffer = NULL, tmpBuffer = NULL;
    SIZE_T buffSize = 0;
    RPC_STATUS status = 0;

    fnUuidFromStringA pUuidFromStringA = (fnUuidFromStringA)GetProcAddress(LoadLibrary(TEXT("RPCRT4")), "UuidFromStringA");
    if (pUuidFromStringA == NULL) {
        printf("[!] GetProcAddress Failed With Error: %d\n", GetLastError());
        return FALSE;
    }

    buffSize = NumElements * 16;
    pBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffSize);
    if (pBuffer == NULL) {
        printf("[!] HeapAlloc Failed With Error: %d\n", GetLastError());
        return FALSE;
    }

    tmpBuffer = pBuffer;

    for (int i = 0; i < NumElements; i++) {
        if ((status = pUuidFromStringA((RPC_CSTR)UuidArray[i], (UUID*)tmpBuffer)) != RPC_S_OK) {
            printf("[!] UuidFromStringA Failed At [%s] With Error 0x%0.8X", UuidArray[i], status);
            return FALSE;
        }
        tmpBuffer = (PBYTE)(tmpBuffer + 16);
    }

    *ppDAddress = pBuffer;
    *pDSize = buffSize;

    return TRUE;
}