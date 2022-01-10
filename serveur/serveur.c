#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include "messageErreur.h" 


static const int MAXPENDING = 5; 

void HandleTCPClient(int clientSocket){

    char buffer[BUFSIZ]; 

    ssize_t numBytesRcvd = recv(clientSocket , buffer , BUFSIZ , 0); 
    
    if(numBytesRcvd > 0){
    
        while(numBytesRcvd > 0){
	
	    ssize_t numBytesSent = send(clientSocket , buffer , numBytesRcvd , 0); 
	    if(numBytesSent < 0){
	        DieWithSystemMessage(" send () failed "); 
	    }else if( numBytesSent != numBytesRcvd){
	        DieWithUserMessage(" send() " , "sent unexpected number of bytes "); 
	    }
	    numBytesRcvd = recv(clientSocket , buffer , BUFSIZ , 0);
	    
	    if(numBytesRcvd < 0){
	        DieWithSystemMessage(" recv failed "); 
	    }
	}
    }
    close(clientSocket); 
}


int main(int argc , char **argv){

    if(argc != 2){
        DieWithUserMessage("Parameters" , "<Server Port>"); 
    }

    in_port_t servPort = atoi(argv[1]); 

    int servSock; 
    if( (servSock = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP)) < 0){
        DieWithSystemMessage(" Impossible de creer le socket "); 
    }

    struct sockaddr_in servAddr; 
    memset(&servAddr , 0 , sizeof(servAddr)); 
    servAddr.sin_family = AF_INET; 
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servAddr.sin_port = htons(servPort); 

    if(bind(servSock , (struct sockaddr *) &servAddr , sizeof(servAddr)) < 0){
        DieWithSystemMessage(" le bind n a pas marche 3"); 
    }

    if(listen(servSock , MAXPENDING) < 0){
        DieWithSystemMessage(" listen fail "); 
    }

    for(;;){
    
        struct sockaddr_in clientAddr; 
	socklen_t clientAddrLen = sizeof(clientAddr); 

	int clientSock = accept(servSock , (struct sockaddr *) &clientAddr , &clientAddrLen); 
	if(clientSock < 0){
	    DieWithSystemMessage(" L ACCEPTATION A A ECHOUE "); 
	}

	char clientName[INET_ADDRSTRLEN]; 
	if(inet_ntop(AF_INET , &clientAddr.sin_addr.s_addr , clientName , sizeof(clientName) ) != NULL ){
            printf(" en train de gerer le client %s/%d " , clientName , ntohs(clientAddr.sin_port)); 	    
	}else{
	    puts("Impossible d avoir l adresse du client "); 
	}

	HandleTCPClient(clientSock); 
    }
}
