#include <stdio.h> 
#include <stdlib.h> 
#include "messageErreur.h" 

/* Pour les erreurs utilisateurs   */


void DieWithUserMessage(const char *msg , const char *detail){

    fputs(msg , stderr); 
    fputs(":" , stderr); 
    fputs(detail , stderr); 
    fputc('\n' , stderr); 
    exit(1); 
}

/* Pour les erreurs Systemes   */


void DieWithSystemMessage(const char *msg){
    perror(msg); 
    exit(1); 
} 
