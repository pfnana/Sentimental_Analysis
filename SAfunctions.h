#ifndef SAfunctions_h
#define SAfunctions_h

struct word {
    char *word;
    float score;
    float SD;
    int SIS_array[10];

};

struct score {
    char* line;
    float score;
};

struct word* createVaderdict(char *filename,int *number);
void deallocateWordArray(struct word *array,int length);
void deallocateScoreArray(struct score *array,int length);
struct score* createScoreArray(struct word *wordArray, int *number, int arrayLength, const char *filename) ;
char *tolowercase(char* str);
void removeNewline(char *str);

#endif 