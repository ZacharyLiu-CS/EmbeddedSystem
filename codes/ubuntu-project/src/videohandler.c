#include "videohandler.h"

// 视频播放：mplayer,既可以播放音乐，也可以播放视频
// 播放：mplayer -x 800 -y 480 -zoom 2.avi &  &:表示后台运行
// 暂停：killall –STOP mplayer &
// 继续：killall –CONT mplayer &
// 停止：killall mplayer
// 使用linux中system


//start to play the video
void play_video(const char * video_path){
	char tmp[1024] = {0};
	sprintf(tmp,"mplayer  %s &",video_path);
	system(tmp);
}


//pause the play the video
void pause_video(){
	system("killall -STOP mplayer &");
}

//stop the play the video
void stop_video(){
	system("killall -9 mplayer");
}

//continue to play the video
void conti_video(){
	system("killall -CONT mplayer &");
}
