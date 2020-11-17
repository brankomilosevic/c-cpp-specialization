/*
Branko Milosevic
C for Everyone
wk05 assignment 01
/*

/* Average weight of elephant seal
Input data read from local file */

#include<stdio.h>
#define NUM_SEALS 1000

int main(void)
{
    FILE *myFile;
    double avg_weight = 0;
    double total_weight = 0;
    int weights[NUM_SEALS];
    myFile = fopen("elephant_seal_data.txt", "r");

    // read data into array
    int i;

    for (i = 0; i < NUM_SEALS; i++)
        fscanf(myFile, "%d", &weights[i]);

    for (i = 0; i < NUM_SEALS; i++)
        total_weight += weights[i];

    avg_weight = total_weight / NUM_SEALS;

    printf("Average weight of %d elephant seals is: %lf\n", NUM_SEALS, avg_weight);
    return 0;
}
