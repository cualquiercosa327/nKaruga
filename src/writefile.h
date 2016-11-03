#ifndef _WRITE_FILE
#define _WRITE_FILE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <stack>

uint32_t ReadLongBigEndian (FILE* output);
uint32_t ReadLongLittleEndian (FILE* output);
void WriteIntBigEndian (uint32_t long_in, FILE* output);
void WriteIntLittleEndian (uint32_t long_in, FILE* output);

#ifdef BIG_INDIAN
	#define WriteInt WriteIntBigEndian
	#define ReadInt ReadLongBigEndian
#else	
	#define WriteInt WriteIntLittleEndian
	#define ReadInt ReadLongLittleEndian
#endif

#endif
