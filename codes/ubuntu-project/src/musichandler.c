#include "musichandler.h"


// system("madplay north.mp3 -r &");//循环播放：参数-r

// system("killall -9 madplay");//利用system函数调用killall命令将madplay终止掉 

     
// system("killall -STOP madplay &");//利用system函数调用killall命令将madplay暂停

// system("killall -CONT madplay &");//利用system函数调用killall命令恢复madplay的播放

//play mp3
void play_mp3(const char * mp3_path){
	
	char tmp[1024] = {0};
	sprintf(tmp,"madplay %s &",mp3_path);
	
	system(tmp);
}

//pause the play the mp3
void pause_mp3(){
	system("killall -STOP madplay &");
}

//stop the play the mp3
void stop_mp3(){
	system("killall -9 madplay");
}

//continue to play the mp3
void conti_mp3(){
	system("killall -CONT madplay &");
}

