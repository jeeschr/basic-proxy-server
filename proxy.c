/*
 * proxy.c - A Simple Sequential Web proxy
 *
 * This program opens a socket and listens for connection. It enters a loop
 * where if an HTTP request is made through browser it opens a 
 * connection to the host and it prints the request
 * on the terminal. The request is forwarded to the server, response received,
 * and printed to the browser. Works on simple sites like www.example.com.
 *
 */ 

#include "csapp.h"
#include <strings.h>

//prints out error message
void myerror();

int main(int argc, char **argv)
{

  char buffer[MAXLINE];
  int clientfd;
  int serverfd;
  struct sockaddr_in clientaddr;
  int clientlen;
  int n;
  rio_t rio;
  int port;
  int sockfd;


  // check arguments

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
    exit(0);
  }
  
  port = atoi(argv[1]);
  
  //open connection on given port, bind, listen
 
  if((sockfd=Open_listenfd(port)) <0)
    myerror();


  //infinite server loop

  while(1){
  
    // accept

    clientlen=sizeof(clientaddr);

    if((clientfd = Accept(sockfd, (SA *)&clientaddr, &clientlen)) < 0)
      myerror();

    //display GET request

    recv(clientfd, buffer, MAXBUF, 0);

    printf("\nThe request in buffer is\n%s\n", buffer);

    ///////////////////begin parse host request/////////////////////
    
    char method[100];
    char filename[500];
    char rest[1000];

    sscanf(buffer, "%s %s %s", method, filename, rest); 

    printf("The FILENAME is initially %s\n", filename);
   
    char *host = strstr(filename, "www");
    char *temp_host = malloc(strlen(host)+1);
    char *temp_host2 = malloc(strlen(host)+1);
    strcpy(temp_host, host);
    strcpy(temp_host2, host);
    printf("The host string is initially %s\n", host);
     
    char * f = index(host,':');
    if(f==NULL){
      host = (char *) strtok(host, "/");
      //printf("The if host string is %s\n", host);
    }
    else{
      host = (char *) strtok(host, ":");
      //printf("The else host is %s\n", host);
    }

    // char *address = strstr(temp_host2, "/");
    // printf("\nMy directory string is %s\n", address);

    //extract port number
    char *tmp = NULL;
    tmp = (char *) strtok(temp_host, "/");
    tmp = (char *) strstr(temp_host, ":");
  
    //printf("\n\n Port: %s\n Address: %s\n Host: %s\n", tmp, address, host);
     
    ///////////////end parse host request//////////////////////

    //open port 80 unless otherwise specified and connect to server at given host

    if(tmp == NULL){
      if((serverfd = open_clientfd(host, 80)) < 0)
	myerror();
    }
    else{
      int tok;
      ++tmp;
      tok = atoi(tmp);
      //printf("PORT is %d", tok);
      if((serverfd = open_clientfd(host, tok)) < 0)
	myerror();
    }
      

    //send GET request to server

    Rio_writen(serverfd, "GET ", strlen("GET "));
    Rio_writen(serverfd, filename, strlen(filename));
    Rio_writen(serverfd, " HTTP/1.0\r\n\r\n", strlen(" HTTP/1.0\r\n\r\n"));

    //receive reply

    int response_len = 0;

    while((n=rio_readn(serverfd, buffer, MAXLINE)) > 0 ) {
      response_len += n;
      Rio_writen(clientfd, buffer, n);
      bzero(buffer, MAXLINE);
    }

    close(clientfd);
    close(serverfd);

  }

  return 0;
}


//prints error message and quits
void myerror(){
  perror("server");
  exit(1);
}
