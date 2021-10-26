#include "mstat.h"

/* DEFINE ARRAYS */
int regCountI[32];
int regCountR[32];
int regCountJ[32];
int opCodes[64];
int functions[64];

int i = 0;
int j = 0;

char line[11];
char *invalid;
int iType, rType, jType, total, funTot = 0;
unsigned long long hex = 0;

int main (int argc, char *argv[]) {
	// Initialize the regcount to 0
    for (int i = 0; i < 32; i++) {
        regCountI[i] = 0;
        regCountR[i] = 0;
        regCountJ[i] = 0;
    }
    // INITIALIZE THE OPCODES ARRAY TO 0
    for (i = 0; i < 64; i++) {
        opCodes[i] = 0;
    }
	// If no flags have been set or flag = -h then print usage and exit
	if ((argc == 1) || (strcmp(argv[1], "-h") == 0)) {
        printf("Usage:\t%s", cmd_str);
        return EXIT_FAILURE;
    }
    else if (strcmp(argv[1], "-i") == 0) {
    	if ((argc == 3) && (strcmp(argv[2], "-u") == 0)) {
    		j = 1;
    	}
    	else if ((argc == 3) && (strcmp(argv[2], "-u") != 0)) {
    		fprintf(stderr,"Error: This is not valid input.\n\nUsage:\t%s", cmd_str);
    		return EXIT_FAILURE;
    	}
    	ifunc();
    }
    else if (strcmp(argv[1], "-r") == 0) {
    	if ((argc == 3) && (strcmp(argv[2], "-u") == 0)) {
    		j = 1;
    	}
    	else if ((argc == 3) && (strcmp(argv[2], "-u") != 0)) {
    		fprintf(stderr,"Error: This is not valid input.\n\nUsage:\t%s", cmd_str);
    		return EXIT_FAILURE;
    	}
    	rfunc();
    }
    else if (strcmp(argv[1], "-o") == 0) {
    	if ((argc == 3) && (strcmp(argv[2], "-u") == 0)) {
    		j = 1;
    	}
    	else if ((argc == 3) && (strcmp(argv[2], "-u") != 0)) {
    		fprintf(stderr,"Error: This is not valid input.\n\nUsage:\t%s", cmd_str);
    		return EXIT_FAILURE;
    	}
    	ofunc();
    }
    else {
    	fprintf(stderr,"Error: This is not valid input.\n\nUsage:\t%s", cmd_str);
    	return EXIT_FAILURE;
    }
}
int ifunc (void) {
	if (!isatty(STDIN_FILENO)) {
		while (scanf("%s", line) != -1) {
			if(strlen(line) == 10) {
				hex = strtoull(line, &invalid, 16);
				if(*invalid == '\0') {
					hex = hex >> 26;
					if(hex == 0) {
						rType++;
						total++;
					}
					else if(hex == 2 || hex == 3) {
						jType++;
						total++;
					}
					else {
						iType++;
						total++;
					}
				}
				else {
					fprintf(stderr,"Error: This is not valid input.\n\nUsage:\t%s", cmd_str);
					return EXIT_FAILURE;
				}
			}
			else {
				fprintf(stderr,"Error: This is not valid input.\n\nUsage:\t%s", cmd_str);
				return EXIT_FAILURE;
			}
		}
	}
	else {
		fprintf(stderr,"Error: This is not valid input.\n\nUsage:\t%s", cmd_str);
		return EXIT_FAILURE;
	}
	if (j == 1){
		printf("TYPE\tCOUNT\tPERCENT\n");
	}
	printf("I-Type\t%d\t%f%%\n", iType, (float) iType / total * 100);
	printf("J-Type\t%d\t%f%%\n", jType, (float) jType / total * 100);
	printf("R-Type\t%d\t%f%%\n", rType, (float) rType / total * 100);
	return EXIT_SUCCESS;
}
int rfunc (void) {
	if (!isatty(STDIN_FILENO)) {
		while (scanf("%s", line) != -1) {
			if(strlen(line) == 10) {
				hex = strtoull(line, &invalid, 16);
				if(*invalid == '\0') {
					unsigned int opcode = hex >> 26;
					if (opcode == 0) { //R-TYPE
						unsigned int rs = hex << 6;
						unsigned int rt = hex << 11;
						unsigned int rd = hex << 16;
						rs = rs >> 27;
						rt = rt >> 27;
						rd = rd >> 27;
						regCountR[rs]++;
						regCountR[rt]++;
						regCountR[rd]++;
						total +=3;
					}
					else if (opcode != 2 || opcode != 3) { //I-TYPE
						int rs = hex << 6;
						int rt = hex << 11;
						rs = rs >> 27;
						rt = rt >> 27;
						regCountI[rs]++;
						regCountI[rt]++;
						total += 2;
					}
					else {
						fprintf(stderr,"Error: This is not valid input.\n\nUsage:\t%s", cmd_str);
						return EXIT_FAILURE;
					}
				}
			}
			else {
				fprintf(stderr,"Error: This is not valid input.\n\nUsage:\t%s", cmd_str);
				return EXIT_FAILURE;
			}
		}
	}
	else {
		fprintf(stderr,"Error: This is not valid input.\n\nUsage:\t%s", cmd_str);
		return EXIT_FAILURE;
	}
	if (j == 1) {
		printf("REG\tUSE\tR-TYPE\tI-TYPE\tJ-TYPE\tPERCENT\n");
		for (i = 0; i < 32; i++) {
			printf("%s\t%d\t%d\t%d\t%d\t%f%%\n", regNames[i], (regCountR[i] + regCountI[i] + regCountJ[i]), regCountR[i], regCountI[i], regCountJ[i], ((float) (regCountR[i] + regCountI[i] + regCountJ[i]) / total)*100);
		}
    }
    else {
		for (i = 0; i < 32; i++) {
			printf("$%d\t%d\t%d\t%d\t%d\t%f%%\n", i, (regCountR[i] + regCountI[i] + regCountJ[i]), regCountR[i], regCountI[i], regCountJ[i], ((float) (regCountR[i] + regCountI[i] + regCountJ[i]) / total)*100);
		}
    }
	return EXIT_SUCCESS;
}
int ofunc (void) {
	if (!isatty(STDIN_FILENO)) {
		while (scanf("%s", line) != -1) {
			if(strlen(line) == 10) {
				hex = strtoull(line, &invalid, 16);
				if (*invalid == '\0') {
					unsigned int opcode = hex >> 26;
					unsigned int function = hex;
					unsigned int mask = 0x0000003f;
					unsigned int func = mask & function;
					opCodes[opcode]++;
					if (opcode == 0) {
						functions[func]++;
						funTot++;
					}
					total++;
				}
				else {
					fprintf(stderr,"Error: This is not valid input.\n\nUsage:\t%s", cmd_str);
					return EXIT_FAILURE;
				}
			}
			else {
				fprintf(stderr,"Error: This is not valid input.\n\nUsage:\t%s", cmd_str);
				return EXIT_FAILURE;
			}
		}
	}
	else {
		fprintf(stderr,"Error: This is not valid input.\n\nUsage:\t%s", cmd_str);
		return EXIT_FAILURE;
	}
	if (j == 1) {
		printf("OPCODE\tCOUNT\tPERCENTAGE\n");
	}
	for (i = 0; i < 64; i++) {
		printf("0x%x\t%d\t%.1f%%\n", i, opCodes[i], ((float) opCodes[i] / total) * 100);
    }
    return EXIT_SUCCESS;
}