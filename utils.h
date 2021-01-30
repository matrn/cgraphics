#ifndef __G_UTILS_H__
#define __G_UTILS_H__

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>


#define DEBUG 1


#if defined(DEBUG) && DEBUG > 0
    #define DEBUG_PRINT(fmt, ...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...) /* Don't do anything in release builds */
#endif



typedef struct term_info {
	int width;
	int height;	
} term_info_t;


term_info_t get_terminal_info();

void clear_terminal();
void reset_colors();

int min(int a, int b);
int max(int a, int b);

#endif