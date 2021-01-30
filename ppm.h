#ifndef __PPM_H__
#define __PPM_H__


#include <stdio.h>
#include <stdlib.h>

typedef struct {
     unsigned char red,green,blue;
} PPMPixel;

typedef struct {
     int x, y;
     PPMPixel *data;
} PPMImage;




#endif