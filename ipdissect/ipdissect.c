#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ErrorExit()
{
	fprintf(stderr, "Invalid ip. Format: N.N.N.N/N\n");
	exit(1);
}

int main(int argc, char *argv[])
{
	if (argc != 2) ErrorExit();

	int N1, N2, N3, N4, CIDR;

	sscanf(argv[1], "%d.%d.%d.%d/%d", &N1, &N2, &N3, &N4, &CIDR);

	if (N1 < 0 || N1 > 255 || N2 < 0 || N2 > 255
	|| N3 < 0 || N3 > 255 || N4 < 0 || N4 > 255) ErrorExit();
	if (CIDR < 0 || CIDR > 32) ErrorExit();

	printf("IP Address: %s\n", argv[1]);

	if (N1 < 128) {
		printf("- Class: A\n");
		printf("- Default Subnet Mask: 255.0.0.0\n");
	} else if (N1 < 192) {
		printf("- Class: B\n");
		printf("- Default Subnet Mask: 255.255.0.0\n");
	} else if (N1 < 224) {
		printf("- Class: C\n");
		printf("- Default Subnet Mask: 255.255.255.0\n");
	} else if (N1 < 240) {
		printf("- Class: D\n");
		printf("- Default Subnet Mask: N/A\n");
	} else {
		printf("- Class: E\n");
		printf("- Default Subnet Mask: N/A\n");
	}

	int inc_octet = CIDR / 8 + 1;
	int inc_mask = CIDR % 8;
	int supernet = 0;
	int i;
	for (i = 0; i < inc_mask; i++) {
		supernet |= 1 << (8 - i - 1);
	}

	printf("- Supernet Mask: ", supernet);
	unsigned int sn_mask = 0;
	int sn_loc = 24;
	for (i = 0; i < inc_octet - 1; i++) {
		sn_mask |= 0xFF << sn_loc;
		sn_loc -= 8;
		printf("255.");
	}
	sn_mask |= supernet << sn_loc;
	printf("%d", supernet);
	for (i = 0; i < 4 - inc_octet; i++) {
		printf(".0");
	}
	printf("\n");
	printf("- Supernet Bits: %d\n", inc_mask);
	printf("- Supernet Octet: %d\n", inc_octet);
	int host_count = ~sn_mask + 1;
	printf("- Total Hosts: %d\n", host_count - 2);
	int a, b, c, d;
	a = N1 & (sn_mask >> 24);
	b = N2 & (sn_mask >> 16);
	c = N3 & (sn_mask >> 8);
	d = N4 & (sn_mask & 0xFF);
	printf("- Network Id: %d.%d.%d.%d\n", a, b, c, d);
	printf("- First Valid IP: %d.%d.%d.%d\n", a, b, c, d + 1);
	unsigned int temp = (a << 24) | (b << 16) | (c << 8) | d;
	temp += host_count;
	temp -= 2;
	printf("- Last Valid IP: %d.%d.%d.%d\n", temp >> 24, (temp >> 16) & 0xFF, (temp >> 8) & 0xFF, (temp & 0xFF));
	temp += 2;
	temp -= 1;
	printf("- Broadcast IP: %d.%d.%d.%d\n", temp >> 24, (temp >> 16) & 0xFF, (temp >> 8) & 0xFF, (temp & 0xFF));
	temp += 1;
	printf("- Next IP: %d.%d.%d.%d\n", temp >> 24, (temp >> 16) & 0xFF, (temp >> 8) & 0xFF, (temp & 0xFF));

	return 0;
}
