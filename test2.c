#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE* fptr1;
    int i, hr[20], min[20];
    char ampm[20]
;    fptr1 = fopen("austinfri.txt", "r");
        if (fptr1 == NULL){
            printf("file couldn't open\n");
        } else {
            for (i=0;i<4;i++){
                fscanf(fptr1, "%i:%i%c\n", &hr[i], &min[i], &ampm[i]);
                printf("File line %i: %i:%i%c\n", i+1, hr[i], min[i], ampm[i]);
            }
            fclose(fptr1);
        }
}