#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void tothrs2totmin(FILE *fptr, int *tsm, int *tem){
    int i, start_hr[20], start_min[20], end_hr[20], end_min[20], tsh[20], teh[20], length = get_file_length(fptr)/2;
    char start_ampm[20], end_ampm[20];

    for (i=0;i<length;i++) {
        fscanf(fptr, "%i:%i%c\n", &(start_hr[i]), &(start_min[i]), &(start_ampm[i]));
        fscanf(fptr, "%i:%i%c\n", &(end_hr[i]), &(end_min[i]), &(end_ampm[i]));

        tsh[i] = start_hr[i];
        if ((start_ampm[i] == 'p') && (start_hr[i] != 12)){
            tsh[i] = start_hr[i] + 12;
        }

        teh[i] = end_hr[i];
        if ((end_ampm[i] == 'p') && (end_hr[i] != 12)){
            teh[i] = end_hr[i] + 12;
        }

        tsm[i] = tsh[i] * 60 + start_min[i];
        tem[i] = teh[i] * 60 + end_min[i];
    }
    rewind(fptr);
}

void get_meet_times(FILE *fptr1, FILE *fptr2, int* tsm1, int *tem1, int *tsm2, int *tem2){
    int i, j, meet_start_hr, meet_end_hr, meet_start_ampm, meet_end_ampm, max_meets, count1 = get_file_length(fptr1), count2 = get_file_length(fptr2);

    if (count1 > count2){
        max_meets = count1/2;
    } else {
        max_meets = count2/2;
    }
    int meet_start_min[max_meets], meet_end_min[max_meets], scount = 0, ecount = 0;

    for (i=0;i<count1/2;i++){
        for (j=0;j<count2/2;j++){
            if ((tsm1[i] <= tsm2[j]) && (tem1[i] >= tsm1[j]) || ((tsm1[i] >= tsm2[j]) && (tsm1[i] <= tem2[j]))){
                if (tsm1[i] > tsm2[j]){
                    meet_start_min[scount] = tsm1[i];
                    scount++;
                } else {
                    meet_start_min[scount] = tsm2[j];
                    scount++;
                }
                if (tem1[i] < tem2[j]){
                    meet_end_min[ecount] = tem1[i];
                    ecount++;
                } else {
                    meet_end_min[ecount] = tem2[j];
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
            meet_start_hr = meet_start_min[i]/60;
            meet_start_ampm = 'a';
            meet_end_hr = meet_end_min[i]/60;
            meet_end_ampm = 'a';
            if (meet_start_min[i]/60 > 12){
                meet_start_hr = meet_start_min[i]/60 - 12;
                meet_start_ampm = 'p';
            }
            if (meet_end_min[i]/60 > 12){
                meet_end_hr = meet_end_min[i]/60 - 12;
                meet_end_ampm = 'p';
            }
            printf("%i:%i%cm and %i:%i%cm\n", meet_start_hr, meet_start_min[i]%60, meet_start_ampm, meet_end_hr, meet_end_min[i]%60, meet_end_ampm);
        }
    }
}

int main(){
    int i, j, hr[20], min[20], blocks, tsm1[20], tem1[20], tsm2[20], tem2[20];
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
        // Get and open files
        fptr1 = get_fname(file_name1, get_name(name1), get_day(day), 'r');
        fptr2 = get_fname(file_name2, get_name(name2), day, 'r');
        if ((fptr1 == NULL) || (fptr2 == NULL)){
            printf("a file couldn't open\n");
        } else {
            // Convert hours to total minutes of the day
            tothrs2totmin(fptr1, tsm1, tem1);
            tothrs2totmin(fptr2, tsm2, tem2);
            rewind(fptr2);

            // Display which times both people share free
            get_meet_times(fptr1, fptr2, tsm1, tem1, tsm2, tem2);
        }

        fclose(fptr1);
        fclose(fptr2);

    } else {
        printf("Invalid choice\n");
    }
    return 0;
}