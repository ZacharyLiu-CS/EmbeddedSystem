#ifndef _TS_H
#define _TS_H
#include "tslib.h"
#include <stdio.h>
//open the touch screen
void openTs(void);

//close the touch screen
void closeTs(void);

//configure the touch screen
void configTs(void);

//get the touch position
void getPos(int *x,int *y,unsigned int * press);

#endif