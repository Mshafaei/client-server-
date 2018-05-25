#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <netdb.h>





int main(int argc, char *argv[])
{

    FILE * fp;
    char * line = NULL;
    char * line2 = NULL;
    size_t len = 0;
    size_t len2 = 0;
    ssize_t readF;
    int size_array =0;
    char portNum[11];
    int intPort=0;
   size_t sz = 10;

    fp = fopen("/home/ritual/Desktop/os/rates20.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((readF = getline(&line, &len, fp)) != -1) {
        size_array++;
    }
    fclose(fp);
    if (line)
        free(line);

    char *a[size_array];
    float b[size_array];
    int j=0;
    char * pch;
    char *token;
    fp = fopen("/home/ritual/Desktop/os/rates20.txt", "r");
	len2=0;
    while ((readF = getline(&line2, &len2, fp)) != -1) {

	pch=strtok(line2, " ");
	a[j] = (char * ) malloc(strlen(pch));
        strcpy(a[j], pch);
	token=strtok(NULL, " ");
	b[j] = atof(token);
	j++;
   	if (line2)
         free(line2);
	line2=NULL;
	len2=0;
    }

  
    fclose(fp);


//end of reading




//host name
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);

    struct hostent *hp;
    hp = gethostbyname(hostname);


//end host name

//get port number
   
    printf("enter server port number: ");

    if(fgets(portNum, sz, stdin)==NULL)
	printf("error stdin");

    intPort=atoi(portNum);
    printf("your port# :%d\n ", intPort);
//end getting port number

    int listenfd = 0, connfd = 0, n=0;
    struct sockaddr_in serv_addr; 
    char recvBuff[1024];
    char sendBuff[1025];
    char serverOut[1025];
    float ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 
    memset(serverOut, '0', sizeof(serverOut)); 

    serv_addr.sin_family = hp->h_addrtype;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(intPort); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
    fflush(stdout);
    listen(listenfd, 10); 

    while(1)
    {
	   connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

	   while ( (n = read(connfd, recvBuff, sizeof(recvBuff)-1)) > 0)
	    {
		recvBuff[n] = 0;
		int temp=-1;
		float currency=0.0;
		char *tempBuffer;

		for (int k=0;k<size_array;k++)
		{
		    if(strcmp(a[k],recvBuff)==0){
			temp=k;
			}

		}
		if (temp!=-1)
			currency=b[temp];
		else
			currency=-1;
		if (currency!=-1)
			snprintf(sendBuff, sizeof(sendBuff), "Today one USD is worth %f %s\n", currency, a[temp]);
		else 
			snprintf(sendBuff, sizeof(sendBuff), "Unknown currency \n");

		write(connfd, sendBuff, strlen(sendBuff)); 
		if (currency!=-1)
			snprintf(serverOut, sizeof(serverOut), "Today's rate for %s is %f", a[temp], currency);
		else
			snprintf(serverOut, sizeof(serverOut), "Today's rate for %s is %s", recvBuff, "Unknown");
	    } 
	   puts(serverOut);
	   fflush(stdout);
	    if( n == 0)
	    {
		puts("Client disconnected");
		fflush(stdout);
	    }
	    else if(n == -1)
	    {
		perror("recv failed");
	    }	     

	

         close(connfd);
         sleep(1);
     }
}
