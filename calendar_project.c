#include <stdio.h>

typedef struct{
    char name[10];
    int block_amount;
    char start_times[20][7], end_times[20][7];
} person;

int main(){
    person p1, p2;
    int i;
    printf("Enter name 1: ");
    scanf("%s", p1.name);
    printf("Enter name 2: ");
    scanf("%s", p2.name);
    printf("%s %s\n", p1.name, p2.name);

    printf("How many blocks does %s have: ", p1.name);
    scanf("%i", &p1.block_amount);
    printf("How many blocks does %s have: ", p2.name);
    scanf("%i", &p2.block_amount);
    printf("%i %i\n", p1.block_amount, p2.block_amount);

    for (i=0;i<p1.block_amount;i++){
        printf("Enter start time busy: ");
        scanf("%s", p1.start_times[i]);
        printf("Enter end time busy: ");
        scanf("%s", p1.end_times[i]);
    }
    /*
        for (i=0;i<p2.block_amount;i++){
        printf("Enter start time busy: ");
        scanf("%s", *p2.start_times);
        printf("Enter end time busy: ");
        scanf("%s", *p2.end_times);
    }
    */
    for (i=0;i<p1.block_amount;i++){
        printf("%s\n", p1.start_times[i]);
        printf("%s\n", p1.end_times[i]);
    }


    return 0;
}