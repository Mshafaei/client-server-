
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 



int main(void)
{
    char hostName[100];
    char currencyCodeInput[100];
    char *fhostName;
    char *currencyCode;
    char portNum[100];
    int  intPort=0;
    

    printf("enter server  name: ");
    fgets(hostName, 100, stdin);
    fhostName=(char * ) malloc(strlen(hostName));
    strncpy(fhostName, hostName,strlen(hostName)-1);


    printf("enter port number: ");
    fgets(portNum, 100, stdin);
    intPort=atoi(portNum);

    printf("enter currency code: ");
    fgets(currencyCodeInput, 100, stdin);


    currencyCode=(char * ) malloc(strlen(currencyCodeInput));
    strncpy(currencyCode, currencyCodeInput,strlen(currencyCodeInput)-1);

    struct hostent* h;
    h = gethostbyname(fhostName);
    if(h == NULL){
	fprintf(stderr,"ERROR, no shuch host\n");
	exit(0);
    }

    int sockfd = 0, n = 0;
    char recvBuff[1024];
    char sendBuff[1025];
    int connfd = 0;
    struct sockaddr_in serv_addr; 


    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    bcopy((char *)h->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,h->h_length);


    serv_addr.sin_port = htons(intPort); 



    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 


   snprintf(sendBuff, sizeof(sendBuff), "%s", currencyCode);
   write(sockfd, sendBuff, strlen(sendBuff)); 
   fflush(stdout);


   while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
   {

        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
	break;
    } 

    if(n < 0)
    {
       printf("\n Read error \n");
    } 
    close(sockfd);
    return 0;
}

