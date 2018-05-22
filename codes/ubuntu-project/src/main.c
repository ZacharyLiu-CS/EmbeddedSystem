#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "linux/input.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include "ts.h"
#include "pichandler.h"
#include "musichandler.h"
#include "socketCommunication.h"
#include "videohandler.h"
#include "touch.c"
#define TYPE_MP3 100
#define TYPE_BMP 101
#define TYPE_MP4 102
#define connactServerIp "192.168.1.110"
#define myIp "192.168.1.123"

int indexPic = 0;
int indexMusic = 0;
int indexVideo = 0;

enum _UI
{

	UI_MAIN,
	UI_PIC,
	UI_MUSIC,
	UI_VOICE,
	UI_VIDEO,
	UI_END
};
enum _UI ui_status;

enum _PIC
{
	PIC_MENUE,
	PIC_PLAY,
	PIC_NEXT_WINDOWSHADE,
	PIC_NEXT_FLYLEFT,
	PIC_NEXT_FTYDOWN,
	PIC_PREV_WINDOWSHADE,
	PIC_PREV_FLYRIGHT,
	PIC_PREV_FLYUP,
	PIC_PLAY_SEQUENTIAL,
	PIC_PLAY_SHUFFLE,
	PIC_NULL

};
enum _PIC pic_status;

enum _MUSIC
{
	MUSIC_PLAY,
	MUSIC_PAUSE,
	MUSIC_HOLD,
	MUSIC_STOP,
	MUSIC_NEXT,
	MUSIC_PREV,
	MUSIC_CONTI,
	MUSIC_START,
	MUSIC_NULL

};

enum _MUSIC music_status;

enum _VIDEO
{
	VIDEO_PLAY,
	VIDEO_PAUSE,
	VIDEO_HOLD,
	VIDEO_STOP,
	VIDEO_NEXT,
	VIDEO_PREV,
	VIDEO_CONTI,
	VIDEO_START,
	VIDEO_NULL
};
enum _VIDEO video_status;
/**
 * @param  filepath is the path of the file you want to searh for
 * @param  filenames is the return of the result
 * @param  filetype defines the type of the file you want to search for
 * @return 
 * 
**/
int getDir(char *filepath, char *fileNames[99], int filetype)
{

	DIR *_dir = opendir(filepath);
	if (_dir == NULL)
	{
		return -1;
	}
	struct dirent *_dirent = NULL;
	int index_1 = 0;

	while (1)
	{
		_dirent = readdir(_dir);
		if (_dirent == NULL)
		{
			break;
		}
		if (filetype == TYPE_BMP)
		{
			if (NULL != strstr(_dirent->d_name, ".bmp"))
			{
				//printf("%s\n",_dirent->d_name);
				*(fileNames + index_1) = (char *)_dirent->d_name;
				index_1++;
			}
		}
		else if (filetype == TYPE_MP3)
		{

			if (NULL != strstr(_dirent->d_name, ".mp3"))
			{
				//printf("%s\n",_dirent->d_name);
				*(fileNames + index_1) = (char *)_dirent->d_name;
				index_1++;
			}
		}
		else if (filetype == TYPE_MP4)
		{
			if (NULL != strstr(_dirent->d_name, ".mp4"))
			{
				//printf("%s\n",_dirent->d_name);
				*(fileNames + index_1) = (char *)_dirent->d_name;
				index_1++;
			}
			else if (NULL != strstr(_dirent->d_name, ".avi"))
			{
				//printf("%s\n",_dirent->d_name);
				*(fileNames + index_1) = (char *)_dirent->d_name;
				index_1++;
			}
		}
	}
	return index_1;
}
void combineFilepatn(char *filepath, char *filename, char *shownPic)
{
	strcpy(shownPic, filepath);
	strcat(shownPic, "/");
	strcat(shownPic, filename);
}

void proc_ts_main(a, b)
{

	if (a >= 440 && a <= 520 && b >= 285 && b <= 365)
	{
		ui_status = UI_PIC;
		pic_status = PIC_PLAY;
	}
	else if (a >= 280 && a <= 360 && b >= 285 && b <= 365)
	{
		ui_status = UI_MUSIC;
	}
	else if (a >= 120 && a <= 200 && b >= 285 && b <= 365)
	{
		ui_status = UI_VIDEO;
	}
	else if (a >= 720 && a <= 800 && b >= 0 && b <= 80)
	{
		ui_status = UI_END;
	}
	else if (a >= 605 && a <= 685 && b >= 285 && b <= 365)
	{
		ui_status = UI_VOICE;
	}
	else if (a >= 740 && b <= 60)
	{
		ui_status = UI_END;
	}
}
void proc_main()
{
	printf("in the main menu\n");
	showWelcome(0, 0);
	//showShutdown_White(740,0);
	while (ui_status == UI_MAIN)
		;
}

void proc_ts_pic(int a, int b, int moveX, int moveY)
{
	if (pic_status != PIC_MENUE)
	{
		if (moveX == 0 && moveY == 0)
		{

			if (a > 400 && b > 60)
			{
				pic_status = PIC_NEXT_WINDOWSHADE;
			}
			else if (a < 400 && b > 60)
			{
				pic_status = PIC_PREV_WINDOWSHADE;
			}
			else if (a > 740 && b < 60)
			{
				pic_status = PIC_NULL;
				ui_status = UI_MAIN;
			}
		}
		else if (moveX == 0 && moveY != 0)
		{
			if (moveY > 0)
			{
				pic_status = PIC_PREV_FLYUP;
			}
			else
			{
				pic_status = PIC_NEXT_FTYDOWN;
			}
		}
		else if (moveX != 0 && moveY != 0)
		{
			int slope = moveY / moveX;
			if (slope >= -1 && slope < 1 && moveX > 0)
			{
				pic_status = PIC_PREV_FLYRIGHT;
			}
			else if ((slope >= 1 || slope < -1) && moveY > 0)
			{
				pic_status = PIC_PREV_FLYUP;
				printf("PIC_PREV_FLYUP\n");
			}
			else if (slope >= -1 && slope < 1 && moveX < 0)
			{
				pic_status = PIC_NEXT_FLYLEFT;
				printf("PIC_NEXT_FLYLEFT\n");
			}
			else if ((slope >= 1 || slope < -1) && moveY < 0)
			{
				pic_status = PIC_NEXT_FTYDOWN;
				printf("PIC_NEXT_FTYDOWN\n");
			}
		}
		else if (moveX != 0 && moveY == 0)
		{
			if (moveX > 0)
			{
				pic_status = PIC_PREV_FLYRIGHT;
			}
			else
			{
				pic_status = PIC_NEXT_FLYLEFT;
			}
		}
	}
	else
	{
		if (a < 400 && b > 165 && b < 315 && pic_status == PIC_MENUE)
		{
			pic_status = PIC_PLAY_SEQUENTIAL;
		}
		else if (a > 400 && b > 165 && b < 315 && pic_status == PIC_MENUE)
		{
			pic_status = PIC_PLAY_SHUFFLE;
		}
		else {
			pic_status = PIC_NULL;
		}
	}
}
void proc_pic()
{
	printf("in the pic menu\n");

	char filePath[10] = "../pic";
	char *bmpFiles[99];
	int index_1 = getDir(filePath, bmpFiles, TYPE_BMP);
	printf("total file num:%d\n", index_1);
	char shownPic[199];
	combineFilepatn(filePath, bmpFiles[0], shownPic);
	int animation[3] = {ANIMATION_MODE_WINDOWSHADES, ANIMATION_MODE_FLYLEFT, ANIMATION_MODE_FLYDOWN};
	srand((unsigned)time(NULL));
	showThePic(shownPic, 0, 0, ANIMATION_MODE_WINDOWSHADES, SHOW_WAYS_COVER_SHADING, 0x00ffffff);
	showBackButton(0, 0);
	while (ui_status == UI_PIC)
	{
		switch (pic_status)
		{
		case PIC_NULL:
		{
			continue;
		}
		case PIC_PLAY:
		{
			printf("in the play\n");
			combineFilepatn(filePath, bmpFiles[indexPic], shownPic);
			showThePic(shownPic, 0, 0, ANIMATION_MODE_WINDOWSHADES, SHOW_WAYS_COVER_SHADING, 0x00ffffff);
			showPicMenue(0, 165);
			pic_status = PIC_MENUE;
			//indexPic = indexPic == index_1 - 1 ? 0 : indexPic + 1;
			break;
		}
		case PIC_MENUE:
		{
			break;
		}
		case PIC_PLAY_SEQUENTIAL:
		{
			indexPic = indexPic == index_1 - 1 ? 0 : indexPic + 1;
			combineFilepatn(filePath, bmpFiles[indexPic], shownPic);
			showThePic(shownPic, 0, 0, animation[rand() % 3], SHOW_WAYS_COVER_SHADING, 0x00ffffff);
			usleep(1e6);
			break;
		}
		case PIC_PLAY_SHUFFLE:
		{
			indexPic = rand() % index_1;
			combineFilepatn(filePath, bmpFiles[indexPic], shownPic);
			showThePic(shownPic, 0, 0, animation[rand() % 3], SHOW_WAYS_COVER_SHADING, 0x00ffffff);
			usleep(1e6);
			break;
		}

		case PIC_NEXT_WINDOWSHADE:
		{
			printf("in the next\n");
			indexPic = indexPic == index_1 - 1 ? 0 : indexPic + 1;
			combineFilepatn(filePath, bmpFiles[indexPic], shownPic);
			showThePic(shownPic, 0, 0, ANIMATION_MODE_WINDOWSHADES, SHOW_WAYS_COVER_SHADING, 0x00ffffff);
			pic_status = PIC_NULL;
			showBackButton(0, 0);
			break;
		}
		case PIC_NEXT_FLYLEFT:
		{
			printf("in the next\n");
			indexPic = indexPic == index_1 - 1 ? 0 : indexPic + 1;
			combineFilepatn(filePath, bmpFiles[indexPic], shownPic);
			showThePic(shownPic, 0, 0, ANIMATION_MODE_FLYLEFT, SHOW_WAYS_COVER_SHADING, 0x00ffffff);
			pic_status = PIC_NULL;
			showBackButton(0, 0);
			break;
		}
		case PIC_NEXT_FTYDOWN:
		{
			printf("in the next\n");
			indexPic = indexPic == index_1 - 1 ? 0 : indexPic + 1;
			combineFilepatn(filePath, bmpFiles[indexPic], shownPic);
			showThePic(shownPic, 0, 0, ANIMATION_MODE_FLYDOWN, SHOW_WAYS_COVER_SHADING, 0x00ffffff);
			pic_status = PIC_NULL;
			showBackButton(0, 0);
			break;
		}
		case PIC_PREV_WINDOWSHADE:
		{
			printf("in the prev\n");
			indexPic = indexPic == 0 ? index_1 - 1 : indexPic - 1;
			combineFilepatn(filePath, bmpFiles[indexPic], shownPic);
			showThePic(shownPic, 0, 0, ANIMATION_MODE_WINDOWSHADES, SHOW_WAYS_COVER_SHADING, 0x00ffffff);
			pic_status = PIC_NULL;
			showBackButton(0, 0);
			break;
		}
		case PIC_PREV_FLYRIGHT:
		{
			printf("in the prev\n");
			indexPic = indexPic == 0 ? index_1 - 1 : indexPic - 1;
			combineFilepatn(filePath, bmpFiles[indexPic], shownPic);
			showThePic(shownPic, 0, 0, ANIMATION_MODE_FLYRIGHT, SHOW_WAYS_COVER_SHADING, 0x00ffffff);
			pic_status = PIC_NULL;
			showBackButton(0, 0);
			break;
		}
		case PIC_PREV_FLYUP:
		{
			printf("in the prev\n");
			indexPic = indexPic == 0 ? index_1 - 1 : indexPic - 1;
			combineFilepatn(filePath, bmpFiles[indexPic], shownPic);
			showThePic(shownPic, 0, 0, ANIMATION_MODE_FLYUP, SHOW_WAYS_COVER_SHADING, 0x00ffffff);
			pic_status = PIC_NULL;
			showBackButton(0, 0);
			break;
		}
		}
	}
}
void proc_ts_video(int a, int b)
{
	printf("process video\n");
	if (a > 370 && a < 430 && b > 420 && b < 480 && video_status == VIDEO_NULL)
	{
		video_status = VIDEO_START;
		printf("\nstart\n");
		showVideoPlay();
	}
	else if (a > 370 && a < 430 && b > 420 && b < 480 && video_status == VIDEO_PLAY)
	{
		video_status = VIDEO_HOLD;
		printf("\npause\n");
		showVideoPause();
	}
	else if (a > 370 && a < 430 && b > 420 && b < 480 && video_status == VIDEO_PAUSE)
	{
		video_status = VIDEO_CONTI;
		printf("\ncontinue\n");
		showVideoPlay();
	}
	else if (a > 760 && b < 60)
	{
		video_status = VIDEO_STOP;
		usleep(100);
		ui_status = UI_MAIN;
	}
	else if (a > 250 && a < 310 && b > 420 && b < 480)
	{
		video_status = VIDEO_PREV;
		showVideoPlay();
		printf("\nprev\n");
	}
	else if (a > 490 && a < 550 && b > 420 && b < 480)
	{
		video_status = VIDEO_NEXT;
		showVideoPlay();
		printf("\nnext\n");
	}
}
void proc_video()
{
	printf("in the video player menu\n");
	showVideoBac();
	showVideoButtons(0, 0, 60);
	char mp4filepath[10] = "../video";
	char *mp4Files[99];
	int totalmp4Count = getDir(mp4filepath, mp4Files, TYPE_MP4);
	char playvideo[199];
	int i = 0;
	printf("totalcount:%d\n", totalmp4Count);
	for (i = 0; i < totalmp4Count; i++)
	{
		printf("%s\n", mp4Files[i]);
	}

	while (ui_status == UI_VIDEO)
	{
		switch (video_status)
		{
		case VIDEO_NULL:
		{
			break;
		}
		case VIDEO_PAUSE:
		{
			break;
		}
		case VIDEO_PLAY:
		{
			break;
		}
		case VIDEO_START:
		{
			combineFilepatn(mp4filepath, mp4Files[indexVideo], playvideo);
			play_video(playvideo);
			video_status = VIDEO_PLAY;
			break;
		}
		case VIDEO_NEXT:
		{
			stop_video();
			indexVideo = indexVideo == totalmp4Count - 1 ? 0 : indexVideo + 1;
			combineFilepatn(mp4filepath, mp4Files[indexVideo], playvideo);
			play_video(playvideo);
			video_status = VIDEO_PLAY;
			break;
		}
		case VIDEO_PREV:
		{
			stop_video();
			indexVideo = indexVideo == 0 ? totalmp4Count - 1 : indexVideo - 1;
			combineFilepatn(mp4filepath, mp4Files[indexVideo], playvideo);
			play_video(playvideo);
			video_status = VIDEO_PLAY;
			break;
		}
		case VIDEO_STOP:
		{
			stop_video();
			video_status = VIDEO_NULL;
			break;
		}
		case VIDEO_HOLD:
		{
			pause_video();
			video_status = VIDEO_PAUSE;
			break;
		}
		case VIDEO_CONTI:
		{
			conti_video();
			video_status = MUSIC_PLAY;
			break;
		}
		}
	}
}

void proc_ts_music(int a, int b, int operation)
{

	if (a > 370 && a < 430 && b > 420 && b < 480 && music_status == MUSIC_NULL)
	{
		music_status = MUSIC_START;
		printf("\nstart\n");
		showMusicPlay();
	}
	else if (a > 370 && a < 430 && b > 420 && b < 480 && music_status == MUSIC_PLAY)
	{
		music_status = MUSIC_HOLD;
		printf("\npause\n");
		showMusicPause();
	}
	else if (a > 370 && a < 430 && b > 420 && b < 480 && music_status == MUSIC_PAUSE)
	{
		music_status = MUSIC_CONTI;
		printf("\ncontinue\n");
		showMusicPlay();
	}
	else if (a >740 && b < 60)
	{
		music_status = MUSIC_STOP;
		usleep(100);
		ui_status = UI_MAIN;
	}
	else if (a > 250 && a < 310 && b > 420 && b < 480)
	{
		music_status = MUSIC_PREV;
		showMusicPlay();
		printf("\nprev\n");
	}
	else if (a > 490 && a < 550 && b > 420 && b < 480)
	{
		music_status = MUSIC_NEXT;
		showMusicPlay();
		printf("\nnext\n");
	}
}

void proc_music()
{
	printf("in the music menu\n");
	showMusicButtons(0, 0, 60);
	char mp3filepath[10] = "../mp3";
	char *mp3Files[99];
	int totalMp3Count = getDir(mp3filepath, mp3Files, TYPE_MP3);
	char playMusic[199];
	int i = 0;
	printf("totalcount:%d\n", totalMp3Count);
	for (i = 0; i < totalMp3Count; i++)
	{
		printf("%s\n", mp3Files[i]);
	}

	while (ui_status == UI_MUSIC)
	{
		switch (music_status)
		{
		case MUSIC_NULL:
		{
			break;
		}
		case MUSIC_PAUSE:
		{
			break;
		}
		case MUSIC_PLAY:
		{
			break;
		}
		case MUSIC_START:
		{
			combineFilepatn(mp3filepath, mp3Files[indexMusic], playMusic);
			play_mp3(playMusic);
			music_status = MUSIC_PLAY;
			break;
		}
		case MUSIC_NEXT:
		{
			stop_mp3();
			indexMusic = indexMusic == totalMp3Count - 1 ? 0 : indexMusic + 1;
			combineFilepatn(mp3filepath, mp3Files[indexMusic], playMusic);
			play_mp3(playMusic);
			music_status = MUSIC_PLAY;
			break;
		}
		case MUSIC_PREV:
		{
			stop_mp3();
			indexMusic = indexMusic == 0 ? totalMp3Count - 1 : indexMusic - 1;
			combineFilepatn(mp3filepath, mp3Files[indexMusic], playMusic);
			play_mp3(playMusic);
			music_status = MUSIC_PLAY;
			break;
		}
		case MUSIC_STOP:
		{
			stop_mp3();
			music_status = MUSIC_NULL;
			break;
		}
		case MUSIC_HOLD:
		{
			pause_mp3();
			music_status = MUSIC_PAUSE;
			break;
		}
		case MUSIC_CONTI:
		{
			conti_mp3();
			music_status = MUSIC_PLAY;
			break;
		}
		}
		// music_status = MUSIC_NULL;
	}
}

void proc_ts_voice(int a, int b)
{

	if (a < 60 && b < 60)
	{
		ui_status = UI_MAIN;
	}
}

void proc_voice()
{
	printf("in the voice menu\n");
	showVoiceBackground(0, 0);
	showBackButton(0, 0);
	system("../soundTest/mknod.sh");
	char *filepath = "/home/cqu/bin/needToRecognize.wav";
	while (ui_status == UI_VOICE)
	{

		char command[2048];
		sendFileAndGetString(filepath, connactServerIp, command);

		if (strcmp(command, "播放音乐") == 0)
			ui_status = UI_MUSIC;
		else if (strcmp(command, "打开相册") == 0)
			ui_status = UI_PIC;
		else if (strcmp(command, "关闭程序") == 0)
			ui_status = UI_END;
		else if (strcmp(command, "播放视频") == 0)
		{
			ui_status = UI_VIDEO;
		}
		else
		{
			printf("失败了！\n");
			showVoiceBackground(0, 0);
			showRecognitionFailed(200, 360);
			usleep(6000000);
		}
	}
}
void proc_end()
{
	showEnd(0, 0);
}
void *ts_pthread(void *test)
{
	int a, b, pressNum = 0;
	int last_a, last_b, moveX, moveY;
	unsigned int press;
	unsigned int lastPress = 0;
	openTs();
	configTs();
	int operation;
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

				moveX = a - last_a;
				moveY = last_b - b;
				if (a - last_a == 0 && last_b - b == 0)
				{
					moveX = 0;
					moveY = 0;
				}
				printf("get position:a = %d,b= %d\n", a, b);
				printf("move for : x = %d, y = %d\n", moveX, moveY);
				switch (ui_status)
				{
				case UI_MAIN:
				{
					moveX = 0;
					moveY = 0;
					proc_ts_main(a, b);
					break;
				}
				case UI_PIC:
				{
					printf("process pic touch\n");
					proc_ts_pic(a, b, moveX, moveY);
					break;
				}
				case UI_VIDEO:
				{
					moveX = 0;
					moveY = 0;
					proc_ts_video(a, b);
					break;
				}
				case UI_MUSIC:
				{
					moveX = 0;
					moveY = 0;
					proc_ts_music(a, b, operation);
					break;
				}
				case UI_VOICE:
				{
					moveX = 0;
					moveY = 0;
					proc_ts_voice(a, b);
					break;
				}
				}
			}
			lastPress = press;
		}
	}
}

int main()
{
	ui_status = UI_MAIN;
	pic_status = PIC_PLAY;
	music_status = MUSIC_NULL;
	video_status = VIDEO_NULL;
	pthread_t t0;

	if (pthread_create(&t0, NULL, ts_pthread, NULL) == -1)
	{
		puts("fail to create pthread t0");
		exit(1);
	}
	while (1)
	{
		switch (ui_status)
		{

		case UI_MAIN:
		{
			proc_main();
			break;
		}
		case UI_VIDEO:
		{
			proc_video();
			break;
		}
		case UI_PIC:
		{
			proc_pic();
			break;
		}
		case UI_MUSIC:
		{
			proc_music();
			break;
		}
		case UI_VOICE:
		{
			proc_voice();
			break;
		}
		case UI_END:
		{
			proc_end();
			return 0;
		}
		}
	}
	return 0;
}
