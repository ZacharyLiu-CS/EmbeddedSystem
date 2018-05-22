/*
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "linux/input.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/mman.h>
#include "ts.h"
#include "pichandler.h"
#include "musichandler.h"
*/
//input the filepath return all the filename
/*
int main()
{
 
	//	get all the bmp files in the file
	char filePath[10] = "../pic";
	DIR *_dir = opendir(filePath);
	char *bmpFiles[99];
	int index_1 = getDir(filePath, bmpFiles);

	// printf("cout num:%d\n", index_1);
	// for (int i = 0; i < index_1; i++)
	// {
	// 	printf("%s\n", bmpFiles[i]);
	// }

	//listen the device:touch screen

	char music[]="../mp3/Victory.mp3";
	play_mp3(music);
	char shownPic[199];
	combineFilepatn(filePath, bmpFiles[0], shownPic);
	while (1)
	{
		getPos(&a, &b, &press);

		if (press != lastPress)
		{
			pressNum++;

			if (press == 200)
			{
				last_a = a;
				last_b = b;
			}
			else
			{
				
				moveX += a - last_a;
				moveY += last_b - b;
				if (a - last_a == 0 && last_b - b == 0)
				{
					moveX = 0;
					moveY = 0;
					if (a > 400)
					{
						index = index == index_1 - 1 ? 0 : index + 1;
						combineFilepatn(filePath, bmpFiles[index], shownPic);
						if (a < 600)
							showThePic(shownPic,0,0,WINDOWSHADES);
						else 
							showThePic(shownPic,0,0,FLYIN);
					}
					else
					{
						index = index == 0 ? index_1 - 1 : index - 1;
						combineFilepatn(filePath, bmpFiles[index], shownPic);
						if (a > 200)
							showThePic(shownPic,0,0,WINDOWSHADES);
						else 
							showThePic(shownPic,0,0,FLYOUT);
					}
				}
				showThePic(shownPic,moveX,moveY,0);
				printf("%s:", shownPic);
				printf("move for : x = %d, y = %d\n", moveX, moveY);
			}
		}
		lastPress = press;
	}

	return 0;
}*/