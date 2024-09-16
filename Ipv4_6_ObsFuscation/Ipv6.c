#include <stdio.h>
#include <stdlib.h>
#include <windows.h>



/*Obfuscation*/

char* CreatingIpv6 (int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p) {

	unsigned char my1[32], my2[32], my3[32], my4[32];

	sprintf(my1, "%d%d%d%d", a, b, c, d);
	sprintf(my2, "%0.2X%0.2X:%0.2X%0.2X", e, f, g, h);
	sprintf(my3, "%0.2X%0.2X:%0.2X%0.2X", i, j, k, l);
	sprintf(my4, "%0.2X%0.2X:%0.2X%0.2X", m, n, o, p);
	unsigned char MyIpv6[128];

	sprintf(MyIpv6, "%s:%s%:%s:%s",my1,my2,my3,my4);

	return (char*)MyIpv6;
}

BOOL GenerateIpv4t(unsigned char* pShell, SIZE_T ShellSize) {

	if (pShell == NULL || ShellSize == NULL || ShellSize % 16 != 0) {
		return EXIT_FAILURE;
	}
	printf("char* Ipv4Array[%d] = { \n\t", (int)(ShellSize / 4));



	int max = 16, counter = 0;
	char* IP = NULL;
	for (int i = 0; i < ShellSize; i++) {


		if (max == 16) {
			counter++;

			IP = CreatingIpv4(pShell[i], pShell[i + 1],
				pShell[i + 2], pShell[i + 3], pShell[i+4], pShell[i+5], pShell[i+6], pShell[i+7], pShell[i+8], pShell[i+9], pShell[i+10], pShell[i+11], pShell[i+12], pShell[i+13], pShell[i+14], pShell[i+15]);
			if (i == ShellSize - 16) {

				printf("\"%s\"", IP);
				break;
			}
			else {

				printf("\"%s\", ", IP);
			}
			max = 1;

			if (counter % 3 == 0) {
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

/* same as IPv4 only this line  : 
* 
*  sBuffSize = NOfElements * 4;  ---->  sBuffSize = NOfElements * 16;
* 
* TmpBuffer = (PBYTE)(TmpBuffer + 4);    -----> TmpBuffer = (PBYTE)(TmpBuffer + 16);
*/