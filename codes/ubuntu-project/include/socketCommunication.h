/*
*author liuzhen
*date 2018-4-11
*/
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>

/*send file and recv string from server
* filepath is the path of the file you want send
*/
int sendFileAndGetString(char *filepath,char *SERVICE_IP,char *retureString);
