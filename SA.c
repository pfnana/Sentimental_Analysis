#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "SAfunctions.h"
 

//Declaration of functions
struct word* createVaderDict(const char *filename, int *number);
void deallocateWordArray(struct word *array, int length);
 
void deallocateScoreArray(struct score *array, int length);
void toLowerCase(char *str);
void removeNewline(char *str);
 
int main(int argc, char *argv[]) {
    if (argc == 2) { //read lexicon
        int number_of_words = 0;
        struct word* wordArray = createVaderDict(argv[1], &number_of_words);
        if (wordArray == NULL) {
            return 1;
        }
        //number_of_words updated
        for (int i = 0; i < number_of_words; i++){
            printf("Word:\t%s\n",wordArray[i].word);
            printf("Score: %f\n\n",wordArray[i].score);
        }
        deallocateWordArray(wordArray, number_of_words);
        return 0;
        
    } else if (argc == 3) { // find scores of sentences
        int number_of_words = 0, number_of_sentences = 0;
        struct word* wordArray = createVaderDict(argv[1], &number_of_words);
        if (wordArray == NULL) {
            return 1;
        }
 
        struct score* scoreArray = createScoreArray(wordArray, &number_of_sentences, number_of_words, argv[2]);
        if (!scoreArray) {
            deallocateWordArray(wordArray, number_of_words);
            return 1;
        }
        //number of words and number of sentences updated
 
        // Printing of sentences and scores
        printf("     String sample");
        printf("%100s","");
        printf("Score\n");

        for (int i = 0; i < 130; i++) {    
            printf("-");
        }
        printf("\n");

        for (int i = 0; i < number_of_sentences ; i++){
            printf("%-100s",scoreArray[i].line);
            printf("%17s","");
            printf("%6f\n",scoreArray[i].score);
        }
 
        // Memory deallocation
        deallocateScoreArray(scoreArray, number_of_sentences);
        deallocateWordArray(wordArray, number_of_words);
    } else {
        printf("Usage: %s <file path of vader_lexicon.txt> <file path of sentencefile.txt>\n", argv[0]);
        return 1;
    }
 
    return 0;
}
 
struct word* createVaderDict(const char *filename, int *number) { //returns word array and num of elements in word array
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File wasn't opened.\n");
        return NULL;
    }
 
    struct word *wordArray = NULL;
    char *line = NULL;
    size_t linelen = 0;
    int lines_read = 0;
 
    while (getline(&line, &linelen, file) != -1) { //remember to deallocate
        wordArray = (struct word*)realloc(wordArray, (lines_read + 1) * sizeof(struct word)); //remember to deallocate
        if (wordArray == NULL) {
            printf("Reallocation failed!\n");
            free(line);
            fclose(file);
            return NULL;
        }
    //Fill the current word struct

    char *token = strtok(line,"\t");
    wordArray[lines_read].word = strdup(token); //remember to deallocate
    
    token = strtok(NULL,"\t");
    wordArray[lines_read].score = atof(token);
    token = strtok(NULL,"\t");
    wordArray[lines_read].SD = atof(token);
    token = strtok(NULL,"\t");
    //token is [array]\n here
    token = strtok(NULL,"[], \n"); //token is now first element in array.
    int i = 0;
    while (token != NULL){
        wordArray[lines_read].SIS_array[i] = atoi(token);
        token = strtok(NULL,"[], \n");
        i++;
    }
        lines_read++;
    }
 
    fclose(file);
    free(line);
    *number = lines_read; //lines_read assigned to global variable
    return wordArray;
}
 
void deallocateWordArray(struct word *array, int length) {
    for (int i = 0; i < length; i++) {
        free(array[i].word);
    }
    free(array);
}
 
struct score* createScoreArray(struct word *wordArray, int *number, int arrayLength, const char *filename) { //returns score array and num of elements in score array
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File wasn't opened.\n");
        return NULL;
    }
 
    struct score* scoreArray = NULL;
    char *line = NULL;
    size_t line_length = 0;
    int line_number = 0;
 
    while (getline(&line, &line_length, file) != -1) { //remember to deallocate
        char *sentence = strdup(line); // Copy the line for storage in scoreArray, remember to deallocate
        if (!sentence) {
            printf("Sentence allocation failed!\n");
            fclose(file);
            free(line);
            return NULL;
        }
        removeNewline(sentence);
 
        char *token = strtok(line, " ,.!?\n");
        char lowerToken[256]; // Assuming tokens won't exceed this length
        float sum = 0.0;
        int sentence_length = 0;
 
        while (token != NULL) {
            strncpy(lowerToken, token, sizeof(lowerToken) - 1);
            lowerToken[sizeof(lowerToken) - 1] = '\0'; // Ensure null termination
            toLowerCase(lowerToken);
 
            for (int i = 0; i < arrayLength; i++) {
                if (strcmp(lowerToken, wordArray[i].word) == 0) {
                    sum += wordArray[i].score;
                    break;
                }
            }
            token = strtok(NULL, " ,.!?\n");
            sentence_length++;
        }
 
        float avg = sentence_length > 0 ? sum / (float)sentence_length : 0.0;
        scoreArray = (struct score*)realloc(scoreArray, (line_number + 1) * sizeof(struct score)); //remember to deallocate
        if (scoreArray == NULL) {
            printf("Reallocation of score array failed!\n");
            fclose(file);
            free(sentence);
            free(line);
            return NULL;
        }
 
        scoreArray[line_number].score = avg;
        scoreArray[line_number].line = sentence; // Already duplicated above
        line_number++;
    }
 
    fclose(file);
    free(line);
    *number = line_number; //line_number being assigned to global variable
    return scoreArray;
}
 
void toLowerCase(char *str) {
    if (!str) return;
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}
 
void deallocateScoreArray(struct score *array, int length) {
    for (int i = 0; i < length; i++) {
        free(array[i].line);
    }
    free(array);
}
 

void removeNewline(char *str) {
    if (!str) return;  // Check for NULL pointer

    char *newlinePos = strchr(str, '\n');  //get the position of the newline character.
    if (newlinePos) {
        *newlinePos = '\0';  // Replace newline with '\0'.
    }
}




