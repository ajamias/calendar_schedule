#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char start_ampm[20],
    end_ampm[20];

    int start_min[20],
    start_hr[20],
    end_min[20],
    end_hr[20],
    tsm[20], 
    tem[20];
} person;

char* get_name(char *name){
    printf("Enter file name: ");
    scanf("%s", name);
    return name;
}

char* get_day(char *day){
    printf("What day is it: ");
    scanf("%s", day);
    if (strcmp(day, "sunday") == 0){
        strcpy(day, "sun");
    } else if (strcmp(day, "monday") == 0){
        strcpy(day, "mon");
    } else if (strcmp(day, "tuesday") == 0){
        strcpy(day, "tue");
    } else if (strcmp(day, "wednesday") == 0){
        strcpy(day, "wed");
    } else if (strcmp(day, "thursday") == 0){
        strcpy(day, "thu");
    } else if (strcmp(day, "friday") == 0){
        strcpy(day, "fri");
    } else if (strcmp(day, "saturday") == 0){
        strcpy(day, "sat");
    } else {
        printf("Invalid day\n");
        return NULL;
    }
    return day;
}

void get_fname(char *fname, char *name, char *day){
    strcpy(fname, name);
    strcat(fname, day);
    strcat(fname, ".txt");
}


void get_times(FILE *fptr, int block_amount){
    int i, start_hr[20], start_min[20], end_hr[20], end_min[20];
    char start_ampm[20], end_ampm[20];
    for (i=0;i<block_amount;i++){
        printf("Start time #%i: ", i+1);
        scanf("%i:%i%c", &start_hr[i], &start_min[i], &start_ampm[i]);
        fprintf(fptr, "%i:%i%c\n", start_hr[i], start_min[i], start_ampm[i]);
        printf("End time #%i: ", i+1);
        scanf("%i:%i%c", &end_hr[i], &end_min[i], &end_ampm[i]);
        fprintf(fptr, "%i:%i%c\n", end_hr[i], end_min[i], end_ampm[i]);
    }
}

void tothrs2totmin(FILE *fptr, person *p){
    int i = 0, tsh[20], teh[20];
    while(fscanf(fptr, "%i:%i%c\n", &(*p).start_hr[i], &(*p).start_min[i], &(*p).start_ampm[i]) != EOF){
        fscanf(fptr, "%i:%i%c\n", &(*p).end_hr[i], &(*p).end_min[i], &(*p).end_ampm[i]);

        tsh[i] = (*p).start_hr[i];
        if (((*p).start_ampm[i] == 'p') && ((*p).start_hr[i] != 12)){
            tsh[i] = (*p).start_hr[i] + 12;
        }

        teh[i] = (*p).end_hr[i];
        if (((*p).end_ampm[i] == 'p') && ((*p).end_hr[i] != 12)){
            teh[i] = (*p).end_hr[i] + 12;
        }

        (*p).tsm[i] = tsh[i] * 60 + (*p).start_min[i];
        (*p).tem[i] = teh[i] * 60 + (*p).end_min[i];
        i++;
    }
}



void get_meet_times(FILE *fptr1, FILE *fptr2, person *p1, person *p2){
    int i, j, shr, ehr, sampm, eampm, max_meets, count1 = 0, count2 = 0, hr1[20], min1[20], hr2[20], min2[20];
    char ampm1[20], ampm2[20];
    while(fscanf(fptr1, "%i:%i%c\n", &hr1[count1], &min1[count1], &ampm1[count1]) != EOF){
        count1++;
    }
    while(fscanf(fptr2, "%i:%i%c\n", &hr2[count2], &min2[count2], &ampm2[count2]) != EOF){
        count2++;
    }

    if (count1 > count2){
        max_meets = count1/2;
    } else {
        max_meets = count2/2;
    }
    int meet_start_min[max_meets], meet_end_min[max_meets], scount = 0, ecount = 0;

    for (i=0;i<count1/2;i++){
        for (j=0;j<count2/2;j++){
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
        // IF THE DIFFERENCE BETWEEN THE END AND START IS <= 15, DONT INCLUDE IT
        if (meet_end_min[i] - meet_start_min[i] > 15){
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
            printf("You can meet between %i:%i%cm and %i:%i%cm\n", shr, meet_start_min[i]%60, sampm, ehr, meet_end_min[i]%60, eampm);
        }
    }
}

int main(){
    int i, j, count = 0, hr[20], min[20], blocks;
    char file_name1[20], file_name2[20], choice, ampm[20], name1[10], name2[10], day[10];
    FILE *fptr1, *fptr2;

    printf("Chose an option:\nr = read file\nu = update/create file\nc = compare times\n");
    scanf("%c", &choice);

    if (choice == 'r'){ // ---- Reading a file ----
        get_fname(file_name1, get_name(name1), get_day(day));

        fptr1 = fopen(file_name1, "r");
        if (fptr1 == NULL){
            printf("file couldn't open\n");
        } else {
            while(fscanf(fptr1, "%i:%i%c\n", &hr[count], &min[count], &ampm[count]) != EOF){
                printf("File line %i: %i:%i%c\n", count+1, hr[count], min[count], ampm[count]);
                count++;
            }
        }
        fclose(fptr1);
    } else if (choice == 'u'){ // ---- Creating a new file and writing to it ----
        get_fname(file_name1, get_name(name1), get_day(day));

        fptr1 = fopen(file_name1, "w");
        if (fptr1 == NULL){
            printf("file couldn't open\n");
        } else {
            printf("How many blocks does %s have: ", name1);
            scanf("%i", &blocks);
            printf("Times for %s\n", name1);
            get_times(fptr1, blocks);
        }
        fclose(fptr1);
    } else if (choice == 'c'){ // ---- Comparing file times ----
        person p1, p2;
        // Get and open files
        get_day(day);
        get_fname(file_name1, get_name(name1), day);
        get_fname(file_name2, get_name(name2), day);
        fptr1 = fopen(file_name1,"r");
        fptr2 = fopen(file_name2,"r");
        if ((fptr1 == NULL) || (fptr2 == NULL)){
            printf("a file couldn't open\n");
        } else {
            printf("both files opened\n");
            // Convert hours to total minutes of the day
            tothrs2totmin(fptr1, &p1);
            tothrs2totmin(fptr2, &p2);
            rewind(fptr1);
            rewind(fptr2);

            // Display which times both people share free
            get_meet_times(fptr1, fptr2, &p1, &p2);
        }

        fclose(fptr1);
        fclose(fptr2);

    } else {
        printf("Invalid choice\n");
    }
    return 0;
}