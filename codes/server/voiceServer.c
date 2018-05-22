#include <stdio.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#define SERVER_IP "192.168.1.110"
#define recvFile "/mnt/hgfs/ubuntu-files/Linux_aitalk_exp1166_5ac194bf/bin/wav/test.wav"
int main()
{
	//1. 创建socket
	int socked_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socked_fd == -1)
	{
		printf("socket error\n");
		return -1;
	}
	//2. 构造绑定的地址
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(23333);

	//将点分十进制转为in_addr类型usigned int
	struct in_addr ipv4_addr;
	inet_pton(AF_INET, SERVER_IP, &ipv4_addr);

	addr.sin_addr.s_addr = ipv4_addr.s_addr; // inet_addr("127.0.0.1");//这里有坑

	//3. 绑定地址
	int ret = bind(socked_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr));
	if (ret == -1)
	{
		printf("bind error!\n");
		return -1;
	}
	//4. 监听
	ret = listen(socked_fd, 5);
	if (ret == -1)
	{
		printf("listen error!\n");
		return -1;
	}

	while (1)
	{

		//5. accept
		struct sockaddr_in addr_recv;
		socklen_t addrlen = 0;
		printf("waiting for client!\n");
		int con_fd = accept(socked_fd, (struct sockaddr *)(&addr_recv), &addrlen);
		while (con_fd == -1)
			;
		if (con_fd == -1)
		{
			printf("accept error \n");
			return -1;
		}
		printf("client connect\n");
		char ip_addr[1024] = {0};
		inet_ntop(AF_INET, &(addr_recv.sin_addr), ip_addr, sizeof(ip_addr));
		printf("client ip address is :%s\n", ip_addr);
		//6. 收文件的大小
		char msg_buff[2048] = {0};
		ssize_t msg_len = 0;
		msg_len = recv(con_fd, msg_buff, 2048, 0);

		//7. 转换收到的文件大小为整形
		int file_size = 0;
		sscanf(msg_buff, "%d", &file_size);
		printf("file_size = %d\n", file_size);

		//8. 开始接收文件内容
		//创建一个文件
		FILE *file_p = fopen(recvFile, "w+");

		bzero(msg_buff, sizeof(msg_buff));
		int total_len = 0;
		while ((msg_len = recv(con_fd, msg_buff, 2048, 0)) > 0)
		{

			fwrite(msg_buff, sizeof(char), msg_len, file_p);
			total_len += msg_len;
			printf("recv:%d/%d\n", total_len, file_size);
			if (total_len >= file_size)
			{
				printf("over\n");
				break;
			}

			bzero(msg_buff, sizeof(msg_buff));
			msg_len = 0;
		}

		system("cd /mnt/hgfs/ubuntu-files/Linux_aitalk_exp1166_5ac194bf/bin &&./asr_sample");
		FILE *fp = fopen("/home/gec210/VoiceResult/result.txt", "r");
		char needTosendString[2048];
		fscanf(fp, "%s", needTosendString);
		send(con_fd, needTosendString, 2048, 0);
		printf("%s\n", needTosendString);
		fclose(fp);
		//system("rm -rf /home/gec210/VoiceResult/result.txt");

		close(con_fd);
	}

	close(socked_fd);
}
