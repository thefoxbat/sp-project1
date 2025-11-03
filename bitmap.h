#ifndef _BITMAP_H_
#define _BITMAP_H_

#include "common.h"

/* Function declarations */
int bitmap_find_first_bit(unsigned char * bitmap, int size, int val);
int bitmap_set_bit(unsigned char * bitmap, int size, int target_pos);
int bitmap_clear_bit(unsigned char * bitmap, int size, int target_pos);
int bitmap_bit_is_set(unsigned char * bitmap, int size, int pos);
int bitmap_print_bitmap(unsigned char * bitmap, int size);

#endif /* _BITMAP_H_ */