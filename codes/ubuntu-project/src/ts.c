
#include "ts.h"
struct tsdev *ts;
struct ts_sample tsInfo;
//open the touch screen
void openTs(void){
	ts = ts_open("/dev/event0",0);
	if (ts == NULL){
		//better than the printf(show the error reason)
		perror("ts open failed!");
		return ;

	}
}

//close the touch screen
void closeTs(void){
	int ret = ts_close(ts);
	if (ret < 0){
		perror("ts close failed!");
		return;
	}
}


void configTs(void){
	int ret = ts_config(ts);
	if (ret < 0){
		perror("configTs failed!");
		return;
	}

}

void getPos(int *x, int *y,unsigned int * press){
	int ret = ts_read(ts,&tsInfo,1);
	
	if (ret < 0){
		perror("getPos failed!");
		return;
	}

	*x = tsInfo.x;
	*y = tsInfo.y;
	* press = tsInfo.pressure;
}
