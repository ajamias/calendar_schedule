#include <stdio.h>

typedef struct{
    char name[10];
    int block_amount, start_min[20], start_hr[20], end_min[20], end_hr[20];
    char start_ampm[20], end_ampm[20];
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
        scanf("%i:%i%c", &p1.start_hr[i], &p1.start_min[i], &p1.start_ampm[i]);
        printf("Enter end time busy: ");
        scanf("%i:%i%c", &p1.end_hr[i], &p1.end_min[i], &p1.end_ampm[i]);
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
        printf("%i:%i%c\n", p1.start_hr[i], p1.start_min[i], p1.start_ampm[i]);
        printf("%i:%i%c\n", p1.end_hr[i], p1.end_min[i], p1.end_ampm[i]);
    }
    /*
    for (i=0;i<p2.block_amount;i++){
    printf("%s\n", p2.start_times[i]);
    printf("%s\n", p2.end_times[i]);
    }
    */

    return 0;
}