#include <stdio.h>
#include <strings.h>

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
    printf("Times for %s\n", (*p).name);
    for (i=0;i<(*p).block_amount;i++){
        printf("Start time #%i: ", i+1);
        scanf("%i:%i%c", &(*p).start_hr[i], &(*p).start_min[i], &(*p).start_ampm[i]);
        printf("End time #%i: ", i+1);
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
    }
}

void get_meet_times(person *p1, person *p2){
    int max_meets = (*p1).block_amount, i, j, shr, ehr, sampm, eampm;
    if ((*p1).block_amount < (*p2).block_amount){
        max_meets = (*p2).block_amount;
    }
    int meet_start_min[max_meets], meet_end_min[max_meets], scount = 0, ecount = 0;
    for (i=0;i<(*p1).block_amount;i++){
        for (j=0;j<(*p2).block_amount;j++){
            if (((*p1).tsm[i] <= (*p2).tsm[j]) && ((*p1).tem[i] >= (*p2).tsm[j]) || (((*p1).tsm[i] >= (*p2).tsm[j]) && ((*p1).tsm[i] <= (*p2).tem[j]))){
                if ((*p1).tsm[i] > (*p2).tsm[j]){
                    meet_start_min[scount] = (*p1).tsm[i];
                    scount++;
                } else {
                    meet_start_min[scount] = (*p2).tsm[j];
                    scount++;
                }
                if ((*p1).tem[i] < (*p2).tem[j]){
                    meet_end_min[ecount] = (*p1).tem[i];
                    ecount++;
                } else {
                    meet_end_min[ecount] = (*p2).tem[j];
                    ecount++;
                }
            }
        }
    }
    // TIMES CONFLICT AT THE LATEST START AND THE EARLIEST END TIME
    for (i=0;i<scount;i++){
        shr = meet_start_min[i]/60;
        sampm = 'a';
        ehr = meet_end_min[i]/60;
        eampm = 'a';
        if (meet_start_min[i]/60 > 12){
            shr = meet_start_min[i]/60 - 12;
            sampm = 'p';
        }
            if (meet_end_min[i]/60 > 12){
            ehr = meet_end_min[i]/60 - 12;
            eampm = 'p';
        }
        printf("You can meet between %i:%i%c and %i:%i%c\n", shr, meet_start_min[i]%60, sampm, ehr, meet_end_min[i]%60, eampm);
    }
}

int main(){
    person p1, p2;
    int i, j, count = 0;
    char day[10];
    FILE *fptr;

    fptr = fopen("austin_mon.txt","r");
    if (fptr == NULL){
        printf("Didn't work\n");
    }
    printf("this is weird\n");
    /*
    printf("What day is it: ");
    scanf("%s", day);

    if (strcmp(day, "sunday\0") == 0){
        printf("It's sunday\n");
    } else if (strcmp(day, "monday\0") == 0){
        printf("It's monday\n");
    } else if (strcmp(day, "tuesday\0") == 0){
        printf("It's tuesday\n");
    } else if (strcmp(day, "wednesday\0") == 0){
        printf("It's wednesday\n");
    } else if (strcmp(day, "thursday\0") == 0){
        printf("It's thursday\n");
    } else if (strcmp(day, "friday\0") == 0){
        printf("It's friday\n");
    } else if (strcmp(day, "saturday\0") == 0){
        printf("It's saturday\n");
    }

    
    while(fscanf(fptr, "%i:%i%c\n", &hr[count], &min[count], &ampm[count]) != EOF){
        printf("File read %i:%i%c\n", hr[count], min[count], ampm[count]);
    }
    */

    fclose(fptr);
    printf("success?\n");

    // Get names of people
    get_name(&p1);
    get_name(&p2);


    // Get number of blocks
    get_blocks(&p1);
    get_blocks(&p2);


    // Get the starting and ending times of each block
    get_start_end_times(&p1);
    get_start_end_times(&p2);


    // Convert hours to total minutes of the day
    tothrs2totmin(&p1);
    tothrs2totmin(&p2);


    // Display which times both people share free
    get_meet_times(&p1, &p2);


    return 0;
}