#include "pichandler.h"  

typedef struct
{
	unsigned char bfType[2]; //file type
	unsigned long bfSize;	//
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long bfOffBits;
} __attribute__((packed)) BitMapFileHeader;

typedef struct
{
	unsigned long biSize; // BitMapFileHeader bits
	long biWidth;		  //bitmap width
	long biHeight;		  //bitmap height
	unsigned short biPlanes;
	unsigned short biBitCount; //bits per pixel:1,4,8,16,24,32
	unsigned long biCompression;
	unsigned long biSizeImage; //bitmap size
	long biXPelsPerMeter;
	long biYPelsPerMeter;
	unsigned long biClrUsed;
	unsigned long biClrImportant;
} __attribute__((packed)) BitMapInfoHeader;

typedef struct
{
	unsigned char Blue;
	unsigned char Green;
	unsigned char Red;
	unsigned char Reserved;
} __attribute__((packed)) RgbQuad;

int showThePic(char *bmpPath, int x, int y, int animation, int showway, unsigned int backgroundcolor)
{

	//open the bmp files
	char *openfile;
	openfile = bmpPath;

	int bmp_fd = open(openfile, O_RDONLY);
	if (-1 == bmp_fd)
	{
		printf("open bmp error!\n");
		return -1;
	}
	//open the monitor
	int lcdFd;
	lcdFd = open("/dev/fb0", O_RDWR);
	if (-1 == lcdFd)
	{
		printf("open error!\n");
		close(lcdFd);
		return -1;
	}
	//get the file height and width
	BitMapFileHeader FileHead;
	BitMapInfoHeader InfoHead;
	RgbQuad rgb;
	long pixLength;
	long pixHeight;
	read(bmp_fd, &FileHead, sizeof(BitMapFileHeader));
	read(bmp_fd, &InfoHead, sizeof(BitMapInfoHeader));

	pixLength = InfoHead.biWidth;
	pixHeight = InfoHead.biHeight;
	//printf("%ld", InfoHead.biSizeImage);

	// try to get the rgb infomation
	off_t ret_lseek = lseek(bmp_fd, 54, SEEK_SET);
	if (-1 == bmp_fd)
	{
		printf("lseek error!\n");
		close(bmp_fd);
		return -1;
	}
	char *bmp_buff;
	bmp_buff = (char *)malloc(InfoHead.biSizeImage);
	//start to read the bmp files content(when meet the big image,use )
	// char bmp_buff[1920*3][1080];
	ssize_t ret_read = read(bmp_fd, bmp_buff, InfoHead.biSizeImage);
	if (-1 == ret_read)
	{
		printf("read error!\n");
		close(bmp_fd);
		return -1;
	}
	close(bmp_fd);
	//use memory reflect to control the monitor

	unsigned int *lcdToMemoryBuff = mmap(NULL, PIXEL_ARGB, PROT_WRITE | PROT_READ, MAP_SHARED, lcdFd, 0);

	int position_X, position_Y, offset = 0;
	;

	// //convert bgr to argb

	//unsigned int lcd_buff[PIXEL] = {0};

	int i = 0, j = 0;
	if (animation == ANIMATION_MODE_NONE)
	{
		for (i = 0; i < pixHeight; i++)
		{
			for (j = 0; j < pixLength; j++)
			{
				unsigned int tmpcolor = bmp_buff[(pixLength * (pixHeight - 1 - i) + j) * 3] << 0 |
										bmp_buff[(pixLength * (pixHeight - 1 - i) + j) * 3 + 1] << 8 |
										bmp_buff[(pixLength * (pixHeight - 1 - i) + j) * 3 + 2] << 16;

				if (showway == SHOW_WAYS_COVER_SHADING)
				{
					lcdToMemoryBuff[(i + y) * 800 + j + x] = tmpcolor;
				}
				else if (showway == SHOW_WAYS_COVER_NOSHADING)
				{
					if (tmpcolor != backgroundcolor)
						lcdToMemoryBuff[(i + y) * 800 + j + x] = tmpcolor;
				}
			}
		}
	}

	int moveLength = 40;
	int movetimes;
	int times;
	if (animation == ANIMATION_MODE_FLYLEFT)
	{
		movetimes = 800 / moveLength;
		for (times = 0; times < movetimes; times++)
		{
			for (i = 0; i < 800; i++)
			{
				for (j = 0; j < 480; j++)
				{
					if (i + moveLength <= 799)
					{
						lcdToMemoryBuff[j * 800 + i] = lcdToMemoryBuff[j * 800 + i + moveLength];
					}
					else
					{
						unsigned int tmpcolor = bmp_buff[(800 * (480 - 1 - j) + i+(times+1)*moveLength -800) * 3] << 0 |
										bmp_buff[(800 * (480 - 1 - j) + i+(times+1)*moveLength -800) * 3 + 1] << 8 |
										bmp_buff[(800 * (480 - 1 - j) + i+(times+1)*moveLength -800) * 3 + 2] << 16;
						lcdToMemoryBuff[j * 800 + i] = tmpcolor;
					}
				}
			}
			usleep(10000/movetimes);
		}
	}
	if (animation == ANIMATION_MODE_FLYRIGHT)
	{
		movetimes = 800 / moveLength;
		for (times = 0; times < movetimes; times++)
		{
			for (i = 799; i >= 0; i--)
			{
				for (j = 0; j < 480; j++)
				{
					if (i - moveLength >=0)
					{
						lcdToMemoryBuff[j * 800 + i] = lcdToMemoryBuff[j * 800 + i - moveLength];
					}
					else
					{
						unsigned int tmpcolor = bmp_buff[(800 * (480 - 1 - j) + i-(times+1)*moveLength +800) * 3] << 0 |
										bmp_buff[(800 * (480 - 1 - j) + i-(times+1)*moveLength +800) * 3 + 1] << 8 |
										bmp_buff[(800 * (480 - 1 - j) + i-(times+1)*moveLength +800) * 3 + 2] << 16;
						lcdToMemoryBuff[j * 800 + i] =tmpcolor;
					}
				}
			}
			usleep(1000/movetimes);
		}
	}
	if (animation == ANIMATION_MODE_FLYUP)
	{
		movetimes = 480 / moveLength;
		for (times = 0; times < movetimes; times++)
		{
			for (i = 0; i <480; i++)
			{
				for (j = 0; j < 800; j++)
				{
					if (i + moveLength < 480)
					{
						lcdToMemoryBuff[i * 800 + j] = lcdToMemoryBuff[(i+moveLength) * 800 + j];
					}
					else
					{
						unsigned int tmpcolor = bmp_buff[(800 * (959- i -(times+1)*moveLength) + j ) * 3] << 0 |
										bmp_buff[(800 * (959 - i - (times+1)*moveLength) + j ) * 3 + 1] << 8 |
										bmp_buff[(800 * (959 - i - (times+1)*moveLength) + j ) * 3 + 2] << 16;
						lcdToMemoryBuff[i * 800 + j] =tmpcolor;
					}
				}
			}
			usleep(10000/movetimes);
		}
	}
	if (animation == ANIMATION_MODE_FLYDOWN)
	{
		movetimes = 480 / moveLength;
		for (times = 0; times < movetimes; times++)
		{
			for (i = 479; i >=0; i--)
			{
				for (j = 0; j < 800; j++)
				{
					if (i - moveLength >=0)
					{
						lcdToMemoryBuff[i * 800 + j] = lcdToMemoryBuff[(i-moveLength) * 800 + j];
					}
					else
					{
						unsigned int tmpcolor = bmp_buff[(800 * ((times+1)*moveLength-i-1) + j ) * 3] << 0 |
										bmp_buff[(800 * ((times+1)*moveLength-i-1) + j ) * 3 + 1] << 8 |
										bmp_buff[(800 * ((times+1)*moveLength-i-1) + j ) * 3 + 2] << 16;
						lcdToMemoryBuff[i * 800 + j] =tmpcolor;
					}
				}
			}
			usleep(10000/movetimes);
		}
	}
	int pages = 15;
	int p = 0;
	if (animation == ANIMATION_MODE_WINDOWSHADES)
	{
		for (i = 0; i < pixHeight / pages; i++)
		{
			for (p = 0; p < pages; p++)
			{
				for (j = 0; j < pixLength; j++)
				{
					int line = p * pixHeight / pages + i;
					unsigned int tmpcolor = bmp_buff[(pixLength * (pixHeight - 1 - line) + j) * 3] << 0 |
											bmp_buff[(pixLength * (pixHeight - 1 - line) + j) * 3 + 1] << 8 |
											bmp_buff[(pixLength * (pixHeight - 1 - line) + j) * 3 + 2] << 16;
					if (showway == SHOW_WAYS_COVER_SHADING)
					{
						lcdToMemoryBuff[(line + y) * 800 + j + x] = tmpcolor;
					}
					else if (showway == SHOW_WAYS_COVER_NOSHADING)
					{
						if (tmpcolor != backgroundcolor)
							lcdToMemoryBuff[(line + y) * 800 + j + x] = tmpcolor;
					}
				}
			}
			usleep(200000 * pages / pixHeight);
		}
	}

	//write(lcdFd, lcd_buff, PIXEL_ARGB);
	free(bmp_buff);
	int releaseLcd = munmap(lcdToMemoryBuff, PIXEL_ARGB);
	if (releaseLcd == -1)
	{
		printf("fail to release the lcd buff\n");
	}
	close(lcdFd);
	return 0;
}
void showWelcome(int x,int y){
	
	showThePic("../material/welcome.bmp",x, y, ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_SHADING,0xffffffff);

	
}

void showShutdown_White(int x,int y){

	showThePic("../material/shutdown_white.bmp",x, y, ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_NOSHADING,0);
}

void showShutdown_Black(int x,int y){

	showThePic("../material/shutdown_black.bmp",x, y, ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_NOSHADING,0x00ffffff);
}


void showBackButton(int x,int y){
	
	showThePic("../material/back.bmp",740-x, y, ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_NOSHADING,0x00ffffff);
	
}
void showPicMenue(int x,int y){
	showThePic("../material/pic_choice.bmp",x, y, ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_SHADING,0x00ffffff);
}
void showMusicButtons(int x,int y ,int distance){
	
	showThePic("../material/musicBackground.bmp", 0, 0, ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_SHADING,0x00ffffff);
	showThePic("../material/prev.bmp", 310-distance, 420-y, ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_NOSHADING,0x00ffffff);
	showThePic("../material/pause.bmp", 370, 420-y,  ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_NOSHADING,0x00ffffff);
	showThePic("../material/next.bmp", 430+distance, 420-y,  ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_NOSHADING,0x00ffffff);
	showThePic("../material/back.bmp", 740-x, y,  ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_NOSHADING,0x00ffffff);
	
}
void showVideoButtons(int x,int y ,int distance){
	
	
	showThePic("../material/prev_white.bmp", 310-distance, 420-y, ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_SHADING,0x00ffffff);
	showThePic("../material/pause_white.bmp", 370, 420-y,  ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_SHADING,0x00ffffff);
	showThePic("../material/next_white.bmp", 430+distance, 420-y,  ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_SHADING,0x00ffffff);
	showThePic("../material/back_white.bmp", x, y,  ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_SHADING,0x00ffffff);
	showThePic("../material/return_white.bmp", 740, 0,  ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_SHADING,0x00ffffff);
	
}
void showVideoBac(){
	showThePic("../material/video_bacground.bmp",0, 0, ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_SHADING,0x00ffffff);
}

void showVideoPlay(){
	showThePic("../material/play_white.bmp", 370, 420,  ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_SHADING,0x00ffffff);
}

void showVideoPause(){
	showThePic("../material/pause_white.bmp", 370, 420,  ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_SHADING,0x00ffffff);
}
void showMusicPause(){
	showThePic("../material/pause.bmp", 370, 420,  ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_NOSHADING,0x00ffffff);
	
}

void showMusicPlay(){
	showThePic("../material/play.bmp", 370, 420,  ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_NOSHADING,0x00ffffff);
	
}

void showEnd(int x,int y){
	
	showThePic("../material/end.bmp",x, y, ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_SHADING,0x00ffffff);
}

void showRecognitionFailed(int x,int y){
	showThePic("../material/recognitionFailed.bmp",x,y,ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_NOSHADING,0x00ffffff);
}

void showVoiceBackground(int x,int y){
	showThePic("../material/voiceRecognition.bmp",x, y, ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_SHADING,0x00ffffff);

}

void showStatusVoice_Listening(int x,int y){
	showThePic("../material/listenning.bmp",x,y,ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_NOSHADING,0x00ffffff);
}

void showStatusVoice_Recognizing(int x,int y){
	showThePic("../material/recognizing.bmp",x,y,ANIMATION_MODE_WINDOWSHADES,SHOW_WAYS_COVER_NOSHADING,0x00ffffff);
}