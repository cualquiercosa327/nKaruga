#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <stack>

/* Thanks Lionel (the lion) */

uint32_t ReadLongBigEndian (FILE* output) {
    uint32_t temp_long;
    temp_long  = fgetc(output) << 24;
    temp_long |= fgetc(output) << 16;
    temp_long |= fgetc(output) << 8;
    temp_long |= fgetc(output);
    return temp_long;
}

uint32_t ReadLongLittleEndian (FILE* output) {
    uint32_t temp_long;
    temp_long  = fgetc(output);
    temp_long |= fgetc(output) << 8;
    temp_long |= fgetc(output) << 16;
    temp_long |= fgetc(output) << 24;
    return temp_long;
}

void WriteIntBigEndian (uint32_t long_in, FILE* output) {
    fputc (((int)(long_in >> 24)) & 0xFF, output);
    fputc (((int)(long_in >> 16)) & 0xFF, output);
    fputc (((int)(long_in >>  8)) & 0xFF, output);
    fputc (((int)(long_in      )) & 0xFF, output);
    fflush(output);
}

void WriteIntLittleEndian (uint32_t long_in, FILE* output) {
    fputc (((int)(long_in      )) & 0xFF, output);
    fputc (((int)(long_in >>  8)) & 0xFF, output);
    fputc (((int)(long_in >> 16)) & 0xFF, output);
    fputc (((int)(long_in >> 24)) & 0xFF, output);
    fflush(output);
}
