/*
*author liuzhen
*date 2018-4-11
*/
#include "socketCommunication.h"
#include "pichandler.h"  


int sendFileAndGetString(char *filepath,char *SERVICE_IP,char *retureString){

    showVoiceBackground(0,0);
	//1. 创建socket
	int socke_fd = socket(AF_INET,SOCK_STREAM,0);

    //2. 构造服务器地址
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port =  htons(23333);
	addr.sin_addr.s_addr = inet_addr(SERVICE_IP);

	int ret ;
// **************************************************************************

	printf("start to connect %s \n",SERVICE_IP);
	//3. 连接服务器

	ret = -1;
	printf("connecting server\n");
	while(ret==-1){
		printf(".\n");
		
		usleep(1000000);
		ret = connect(socke_fd, (struct sockaddr *) &addr,sizeof(struct sockaddr));}
	if(-1 == ret){
		printf("connect failed!\n");
		return -1;
	}
	printf("connect!\n");
	showStatusVoice_Listening(200,360);
	system("/home/cqu/soundTest/alsa_record needToRecognize.wav");



	//4. 获取文件大小

	FILE *file_p = fopen(filepath,"r");
	if(file_p ==  NULL){
		printf("open file error!\n");
	}

	int file_size = 0;
	fseek(file_p,0,SEEK_END);//将文件指针偏移到文件末尾
	file_size = ftell(file_p);//获取当前的偏移量（偏移量就是文件大小）
	fseek(file_p,0,SEEK_SET);//将文件指针偏移到文件开头

	

	//5. 发送文件大小
	char send_buff[2048] = {0};
	int len = 0;
	sprintf(send_buff, "%d\n", file_size);
	len = send(socke_fd,send_buff,2048,0);

	//6. 读取文件内容
	//7. 发送文件内容
	
	bzero(send_buff,sizeof(send_buff));
	int total = 0;
	while((len = fread(send_buff, sizeof(char),sizeof(send_buff),file_p)) > 0)
	{

		int ret = send(socke_fd,send_buff,len,0);
		total += ret;
		printf("send:%d/%d/%d\n",len,total,file_size );
		bzero(send_buff,sizeof(send_buff));
	}
	fclose(file_p);
	printf("over sending\n");
	showVoiceBackground(0,0);
	showStatusVoice_Recognizing(200,360);
	ssize_t msg_len = 0;
	msg_len = recv(socke_fd, retureString, 2048, 0);
	printf("get string:%s\n",retureString);
	

	close(socke_fd);

	//8. 关闭socket，关闭文件
	//msg_len = recv(socke_fd, return_mess, 2048, 0);
	


	return 0; 
}
