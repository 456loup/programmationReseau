#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "messageErreur.h" 

int main(int argc , char **argv){

    if(argc < 3 || argc > 4){
        DieWithUserMessage("Parametres" , "<serveurAddr><Mot><portNO>"); 
    }

    /* CE SONT LES PARAMETRES QUE L ON PASSE A L INTERIEUR DU PROGRAMME   */
   
    char *servIP = argv[1]; // l adresse du serveur  
    char *echoString = argv[2]; // le message que l on veut envoyer un message 
    in_port_t servPort = atoi(argv[3]); // le port surlequel on veut l envoyer  

    int sock = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP); 
    if(sock < 0){
        DieWithSystemMessage("socket foiree "); 
    }
    

    /* REMPLISSAGE DE LA STRUCTURE SOCK ADDR DU SERVEUR   */
    struct sockaddr_in servAddr; 
    memset(&servAddr , 0 , sizeof(servAddr)); 
    servAddr.sin_family = AF_INET; 
    int rtnVal = inet_pton(AF_INET , servIP , &servAddr.sin_addr.s_addr);
    if(rtnVal == 0){
        DieWithSystemMessage(" l adresse que vous avez rentre N EXISTE PAS "); 
    }
    servAddr.sin_port = htons(servPort);

    /* FIN DE LA PARTIE INITIALISATION   */ 

    if(connect(sock , (struct sockaddr *)  &servAddr , sizeof(servAddr)) < 0){
        DieWithSystemMessage(" LA CONNECTION A ECHOUE "); 
    }

    size_t echoStringLen = strlen(echoString);
    
    ssize_t numBytes = send(sock , echoString , echoStringLen , 0); 
    if(numBytes < 0){
        DieWithSystemMessage(" ON A PAS REUSSI A ENVOYER LA CHAINE "); 
    }

    int totalBytesRcvd = 0; 
    fputs(" recu : " , stdout); 

    while(totalBytesRcvd < echoStringLen){
    
        char buffer[BUFSIZ];
	numBytes = recv(sock , buffer , BUFSIZ-1 , 0); 
        if(numBytes < 0){
	    DieWithSystemMessage(" AUCUN BYTES DE RECU "); 
	}

	totalBytesRcvd += numBytes;  
	buffer[numBytes] = '\0'; 
	fputs(buffer , stdout); 
    
    }

    fputc('\n' , stdout); 
    close(sock); 
    exit(0); 
}
