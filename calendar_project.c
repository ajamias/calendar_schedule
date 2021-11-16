#include <stdio.h>

typedef struct{
    char name[10],
    start_ampm[20],
    end_ampm[20];

    int block_amount,
    start_min[20],
    start_hr[20],
    end_min[20],
    end_hr[20],
    tsh[20],
    tsm[20],    // tsh = true starting hour
    teh[20],    // tem = true ending minute
    tem[20];
} person;

void get_name(person *p){
    printf("Enter name: ");
    scanf("%s", (*p).name);
}

void get_blocks(person *p){
    printf("How many blocks does %s have: ", (*p).name);
    scanf("%i", &(*p).block_amount);
}

void get_start_end_times(person *p){
    int i;
    for (i=0;i<(*p).block_amount;i++){
        printf("Enter start time: ");
        scanf("%i:%i%c", &(*p).start_hr[i], &(*p).start_min[i], &(*p).start_ampm[i]);
        printf("Enter end time: ");
        scanf("%i:%i%c", &(*p).end_hr[i], &(*p).end_min[i], &(*p).end_ampm[i]);
    }
}

void tothrs2totmin(person *p){
    int i;
    for (i=0;i<(*p).block_amount;i++){
        (*p).tsh[i] = (*p).start_hr[i];
        (*p).teh[i] = (*p).end_hr[i];
        if ((*p).start_ampm[i] == 'p' && (*p).start_hr[i] != 12){
            (*p).tsh[i] = (*p).start_hr[i] + 12;
        }
        if ((*p).end_ampm[i] == 'p' && (*p).end_hr[i] != 12){
            (*p).teh[i] = (*p).end_hr[i] + 12;
        }
        (*p).tsm[i] = (*p).tsh[i] * 60 + (*p).start_min[i];
        (*p).tem[i] = (*p).teh[i] * 60 + (*p).end_min[i];
        printf("starting minute: %i\n", (*p).tsm[i]);
        printf("ending minute: %i\n", (*p).tem[i]);
    }
}

int main(){
    person p1, p2;
    int i;

    // Get names of people
    get_name(&p1);
    get_name(&p2);
    printf("%s %s\n", p1.name, p2.name);


    // Get number of blocks
    get_blocks(&p1);
    get_blocks(&p2);
    printf("%i %i\n", p1.block_amount, p2.block_amount);


    // Get the starting and ending times of each block
    get_start_end_times(&p1);
//    get_start_end_times(&p2);

    // Print the starting and ending times
    for (i=0;i<p1.block_amount;i++){
        printf("%i:%i%c\n", p1.start_hr[i], p1.start_min[i], p1.start_ampm[i]);
        printf("%i:%i%c\n", p1.end_hr[i], p1.end_min[i], p1.end_ampm[i]);
    }
    /*
    for (i=0;i<p2.block_amount;i++){
        printf("%i:%i%c\n", p2.start_hr[i], p2.start_min[i], p2.start_ampm[i]);
        printf("%i:%i%c\n", p2.end_hr[i], p2.end_min[i], p2.end_ampm[i]);
    }
    */

    // Convert hours to total minutes of the day
    tothrs2totmin(&p1);
//    tothrs2totmin(&p2);
/*
    p2.tsm[0] = 750;
    p2.tem[0] = 810;
    if ((p1.tsm[0] <= p2.tsm[0]) && ((p1.tem[0] <= p2.tem[0]) && (p1.tem[0] >= p2.tsm[0]))){
        printf("They conflict\n");
    } else {
        printf("They don't conflict\n");
    }
*/

    return 0;
}