#ifndef _PICHANDLER_H
#define _PICHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#define PIXEL 800 * 480
#define PIXEL_RGB 800 * 480 * 3
#define PIXEL_ARGB 800 * 480 * 4


#define ANIMATION_MODE_NONE 0
#define ANIMATION_MODE_FLYLEFT 1
#define ANIMATION_MODE_FLYRIGHT 2
#define ANIMATION_MODE_FLYUP 3
#define ANIMATION_MODE_FLYDOWN 4
#define ANIMATION_MODE_WINDOWSHADES 5


#define SHOW_WAYS_CLEAR 10
#define SHOW_WAYS_COVER_NOSHADING 11
#define SHOW_WAYS_COVER_SHADING 12


//show the bmp files on the screen
//animation include FLYIN WINDOWSHADES FLYOUT three mode
int showThePic(char *bmpPath, int x, int y, int animation, int showway, unsigned int backgroundcolor);

void showWelcome(int x,int y);

void showPicMenue(int x,int y);

void showShutdown_White(int x,int y);

void showShutdown_Black(int x,int y);

void showBackButton(int x,int y);

void showMusicButtons(int x,int y ,int distance);

void showVideoButtons(int x,int y ,int distance);

void showVideoBac();

void showVideoPlay();

void showVideoPause();

void showMusicPause();

void showMusicPlay();

void showEnd(int x,int y);

void showStatusVoice_Listening(int x,int y);

void showStatusVoice_Recognizing(int x,int y);

#endif