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
    printf("Enter name of person: ");
    scanf("%s", name);
    return name;
}

char* get_day(char *day){
    printf("\nWhat day is it: ");
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
    printf("\n");
    return day;
}

FILE* get_fname(char *fname, char *name, char *day, char choice){
    strcpy(fname, name);
    strcat(fname, day);
    strcat(fname, ".txt");
    return fopen(fname, &choice);
}

int get_file_length(FILE *fptr){
    int hr[20], min[20], count = 0;
    char ampm[20];
    while(fscanf(fptr, "%i:%i%c\n", &hr[count], &min[count], &ampm[count]) != EOF){
        count++;
    }
    rewind(fptr);
    return count;
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
        printf("\n");
    }
}

void tothrs2totmin(FILE *fptr, person *p, int *tsm, int *tem){
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

        tsm[i] = tsh[i] * 60 + (*p).start_min[i];
        tem[i] = teh[i] * 60 + (*p).end_min[i];
        i++;
    }
}

void get_meet_times(FILE *fptr1, FILE *fptr2, person *p1, person *p2, int* tsm, int *tem){
    int i, j, start_hr, end_hr, start_ampm, end_ampm, max_meets, count1 = get_file_length(fptr1), count2 = get_file_length(fptr2);
    char ampm1[20], ampm2[20];

    if (count1 > count2){
        max_meets = count1/2;
    } else {
        max_meets = count2/2;
    }
    int meet_start_min[max_meets], meet_end_min[max_meets], scount = 0, ecount = 0;

    for (i=0;i<count1/2;i++){
        for (j=0;j<count2/2;j++){
            if ((tsm[i] <= tsm[j]) && (tem[i] >= tsm[j]) || ((tsm[i] >= tsm[j]) && (tsm[i] <= tem[j]))){
                if (tsm[i] > tsm[j]){
                    meet_start_min[scount] = tsm[i];
                    scount++;
                } else {
                    meet_start_min[scount] = tsm[j];
                    scount++;
                }
                if (tem[i] < tem[j]){
                    meet_end_min[ecount] = tem[i];
                    ecount++;
                } else {
                    meet_end_min[ecount] = tem[j];
                    ecount++;
                }
            }
        }
    }

    // TIMES CONFLICT AT THE LATEST START AND THE EARLIEST END TIME
    printf("\nYou can meet between:\n");
    for (i=0;i<scount;i++){
        // IF THE DIFFERENCE BETWEEN THE END AND START IS <= 15, DONT INCLUDE IT
        if (meet_end_min[i] - meet_start_min[i] > 15){
            start_hr = meet_start_min[i]/60;
            start_ampm = 'a';
            end_hr = meet_end_min[i]/60;
            end_ampm = 'a';
            if (meet_start_min[i]/60 > 12){
                start_hr = meet_start_min[i]/60 - 12;
                start_ampm = 'p';
            }
            if (meet_end_min[i]/60 > 12){
                end_hr = meet_end_min[i]/60 - 12;
                end_ampm = 'p';
            }
            printf("%i:%i%cm and %i:%i%cm\n", start_hr, meet_start_min[i]%60, start_ampm, end_hr, meet_end_min[i]%60, end_ampm);
        }
    }
}

int main(){
    int i, j, count = 0, hr[20], min[20], blocks, tsm[20], tem[20];
    char file_name1[20], file_name2[20], choice, ampm[20], name1[10], name2[10], day[10];
    FILE *fptr1, *fptr2;

    printf("Chose an option:\nr = read file\nu = update/create file\nc = compare times\n");
    scanf("%c", &choice);
    printf("\n");

    if (choice == 'r'){ // ---- Reading a file ----
        fptr1 = get_fname(file_name1, get_name(name1), get_day(day), 'r');
        if (fptr1 == NULL){
            printf("file couldn't open\n");
        } else {
            for (i=0;i<get_file_length(fptr1);i++){
                fscanf(fptr1, "%i:%i%c\n", &hr[i], &min[i], &ampm[i]);
                printf("File line %i: %i:%i%c\n", i+1, hr[i], min[i], ampm[i]);
            }
        }
        fclose(fptr1);
    } else if (choice == 'u'){ // ---- Creating a new file and writing to it ----
        fptr1 = get_fname(file_name1, get_name(name1), get_day(day), 'w');
        if (fptr1 == NULL){
            printf("file couldn't open\n");
        } else {
            printf("How many blocks does %s have: ", name1);
            scanf("%i", &blocks);
            printf("\nTimes for %s:\n", name1);
            get_times(fptr1, blocks);
        }
        fclose(fptr1);
    } else if (choice == 'c'){ // ---- Comparing file times ----
        person p1, p2;
        // Get and open files
        fptr1 = get_fname(file_name1, get_name(name1), get_day(day), 'r');
        fptr2 = get_fname(file_name2, get_name(name2), day, 'r');
        if ((fptr1 == NULL) || (fptr2 == NULL)){
            printf("a file couldn't open\n");
        } else {
            // Convert hours to total minutes of the day
            tothrs2totmin(fptr1, &p1, tsm, tem);
            tothrs2totmin(fptr2, &p2, tsm, tem);
            rewind(fptr1);
            rewind(fptr2);

            // Display which times both people share free
            get_meet_times(fptr1, fptr2, &p1, &p2, tsm, tem);
        }

        fclose(fptr1);
        fclose(fptr2);

    } else {
        printf("Invalid choice\n");
    }
    return 0;
}