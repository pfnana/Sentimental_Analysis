#include "SAfunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// struct word {
//     char *word;
//     float score;
//     float SD;
//     int SIS_array[10];

// };

// struct score {
//     char* line;
//     float score;
// };




//going to return the number of lines read which would be equal to num of elements in the array
//Creates wordarray;
int createVaderdict(char *filename,struct word *wordarray){
    //wordarray = (struct word *)malloc(1*sizeof(struct word)); //pointer array of pointer words, as
    //As at now it stores only one struct word*
    // if (wordarray == NULL){
    //     printf("Allocation failed!\n");
    //     free(wordarray);
    //     return -1;
    // }
    //remember to deallocate at end. ^
    int lines_read = 0;
    char *line = NULL;
    size_t linelen = 0;
    
    FILE *file = fopen(filename,"r");
    if (file == NULL){
        printf("File wasn't read.\n");
        free(wordarray);
        return -1;
        //if file isn't read program is terminated.
    }
//parsing the file
//line by line
//close the file
while (getline(&line,&linelen,file) != -1){  //remember to deallocate line!
    lines_read++;
    if (lines_read > 1){
        wordarray = (struct word*)realloc(wordarray,lines_read*sizeof(struct word));
        if (wordarray == NULL){
            perror("Reallocation of Dictionary failed!");
            free(wordarray);
            free(line);
            fclose(file);
            return -1;
    }//deallocate
    }
    
    char *token = strtok(line,"\t");
    wordarray[lines_read - 1].word = strdup(token); //remember to deallocate
    
    token = strtok(NULL,"\t");
    wordarray[lines_read - 1].score = atof(token);
    token = strtok(NULL,"\t");
    wordarray[lines_read - 1].SD = atof(token);
    token = strtok(NULL,"\t");
    //token is [array]\n here
    token = strtok(NULL,"[], \n"); //token is now first element in array.
    int i = 0;
    while (token != NULL){
        wordarray[lines_read - 1].SIS_array[i] = atoi(token);
        token = strtok(NULL,"[], \n");
        i++;
    }
    
}
    fclose(file);
    free(line);
    return lines_read; //return number of words
}

//In both structs memory to be deallocated is memory in text fields and array themselves.  
void deallocateWordArray(struct word *array,int length){
    for (int i = 0; i < length ; i++){
        free(array[i].word);
    }
    free(array);
}

void deallocateScoreArray(struct score *array,int length){
    for (int i = 0; i < length ; i++){
        free(array[i].line);
    }
    free(array);
}

//figure out data type of function
int createScoreArray(struct word *wordarray,struct score *scorearray,int array_length,char *filename){ //argv[2]

    // scorearray = (struct score *)malloc(1*sizeof(struct score));
    // if (!scorearray){
    //     printf("score allocation failed!\n");
    //     free(scorearray);
    //     deallocateWordArray(wordarray,array_length);
    //     return -1;
    // }
    //remember to deallocate
    
    char *line = NULL;
    size_t line_length = 0;
    //(wordarray,arrat_length,argv[2])
    FILE *file = fopen(filename,"r"); //remember to close the file
    if (file == NULL){
        printf("File wasn't read!\n");
        free(scorearray);
        deallocateWordArray(wordarray,array_length);
        return -1;

    }
    int line_number = 0;
    while (getline(&line,&line_length,file) != -1){
        line_number++;
        char *sentence = strdup(line);  //free line later
        if (!sentence){
            printf("Sentence allocation failed!\n");
            free(scorearray);
            free(line);
            deallocateWordArray(wordarray,array_length);
            fclose(file);
            return -1;
        }
        float sum = 0.0;
        float avg = 0.0;
        char *token = strtok(line," ,.!?\n");
        float sentence_length = 0.0;
        
        do{ 
            for (int i = 0 ; i < array_length ; i++){ //check if a word is in the dictionary
                if (strcmp(tolowercase(token),wordarray[i].word) == 0){
                    sum += wordarray[i].score;
                    break;
                }//else value = 0 not needed cause nothing will be added
            }
            sentence_length++;
            token = strtok(NULL," ,.!?\n");

            
        }while (token != NULL );
        
        avg = (sum/sentence_length);
        if (line_number > 1){
            scorearray = (struct score *)realloc(scorearray,line_number*sizeof(struct score));
            if (scorearray == NULL){
                printf("Reallocation of scorearray failed!\n");
                free(scorearray);
                deallocateWordArray(wordarray,array_length);
                free(sentence);
                free(line);
                fclose(file);
                return -1;
        }
        }
        scorearray[line_number - 1].score = avg;
        scorearray[line_number - 1].line = strdup(sentence); //free this memory after using it.
        if (scorearray[line_number - 1].line == NULL){
            deallocateScoreArray(scorearray,line_number);
            deallocateWordArray(wordarray,array_length);
            free(sentence);
            free(line);
            fclose(file);
        }
    free(sentence);
    }
    fclose(file);
    free(line);
    return line_number; //return number of sentences
    
}


char *tolowercase(char* str){
    int length = strlen(str);
    for (int i = 0 ; i < length ; i++){
        str[i] = tolower(str[i]);
    }
}




// struct score**  LineScore(struct word ** wordarray,int array_length,char *filename){
//     char *line = NULL;
//     size_t line_length = 0;
//     //(wordarray,arrat_length,)
//     FILE *file = fopen(filename,"r"); //remember to close the file

//     while (getline(&line,&line_length,file) != -1){
//         float sum = 0;
//         float avg = 0;
//         char *token = strtok(line," ");
//         float sentence_length = 0;
//         do{    
//             for (int i = 0 ; i < array_length ; i++){ //check if a word is in the dictionary
//                 if (strcmp(token,wordarray[i]->word) == 0){
//                     sum += wordarray[i]->score;
//                     break;
//                 }//else value = 0 not needed cause nithing will be added
//             }
//             sentence_length++;
//             token = strtok(NULL," ");
            
            
//             }while (token != NULL );

//         avg = (sum/sentence_length);
//     }
// }
