#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(){
    char *d = "poun ggg thhh.\n";
    char *f = strdup(d);
    char *g = "";
    d[14] = *g;
    printf("%s\n",d);
    if (f[15] == '\0'){
        printf("I'm right\n");
    }
    free(f);
}
