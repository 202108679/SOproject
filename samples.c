#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>


int main(int argc, const char* argv[]){
    
    if(argc != 4) printf("Wrong number of arguments!");
    int numberfrags = atoi(argv[2]), maxfragsize = atoi(argv[3]);
    
    FILE *fp;
    fp = fopen(argv[1], "r");
    //printf(&fp);
    if (fp == NULL) {
        perror("Error");
        return(-1);
    } 

    int inicio;
    //temos de defini fileupper e filelower...
    int fileupper = 0;
    fseek(fp, 0L, SEEK_END);
    int filelower = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    srand(time(NULL));
    int positions[numberfrags];

    for(int i = 0; i < numberfrags; i++){
        inicio = (rand() % (filelower - fileupper + 1));
        int repeated = 0;       
        for(int j = 0; j < numberfrags; j++)            
            if(positions[j] == inicio){
            i--;
            repeated = 1;
        }
    if(!repeated){
        positions[i] = inicio;
        continue;
        }
    }
    
    
    for (int k = 0; k < numberfrags; k++){
        
        /* inicio = (rand() % (filelower - fileupper + 1));
        int array = malloc(sizeof(int) ); */

        printf(">");
        fseek(fp, positions[k], SEEK_SET);   
        int c;
        for (int j = 0; j < maxfragsize; j++){
            if ((c=fgetc(fp)) != EOF) {
                if (c != '\n') putchar(c);              //a pelica vai ter de ser a do teclado ("'"), já que a do pdf come 2 caracteres e não consigo resolver esse problema
                else printf(" ");
            }
        }
        /* while( (c=fgetc(fp)) != EOF)
            putchar(c); */  
        printf("<\n");
    }
    return 0;
}