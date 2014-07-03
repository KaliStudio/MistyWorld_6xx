

#ifndef _WAVE_H
#define _WAVE_H

typedef union
{
    unsigned short*  pw;
    unsigned char*   pb;
} byte_and_short;

int libmpq_wave_decompress(unsigned char* out_buf, int out_length, unsigned char* in_buf, int in_length, int channels);

#endif                  /* _WAVE_H */
