#ifndef MSTAT_H
#define MSTAT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

char *cmd_str = "./mstat [OPTION]\n\t./mstat -h\tDisplays this help menu.\n\t./mstat -i [-u]\tDisplays statistics about instruction types.\n\t./mstat -r [-u]\tDisplays information about the registers.\n\t./mstat -o [-u]\tDisplays number and percentage of opcodes used.\n\nOptional flags:\n\t-u\tDisplays human readable headers for the different outputs.\n\nExample: cat mips1.txt | ./mstat -r";

char *regNames[] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", 
"$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

int ifunc(void);
int rfunc(void);
int ofunc(void);

#endif