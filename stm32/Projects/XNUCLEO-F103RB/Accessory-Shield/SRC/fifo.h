#ifndef _FIFO_H
#define _FIFO_H

#include "LIB_Config.h"

#define    FIFO_MAX_SIZE      4096                                           

int fifo_init(void);
int fifo_in(unsigned char m);
int fifo_out(unsigned char * ptr);
int fifo_size(void);
int fifo_freesize(void);

#endif /*_FIFO_H*/

