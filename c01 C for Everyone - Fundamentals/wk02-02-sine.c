/*
Branko Milosevic
C for Everyone
wk02 assignment 01
/*

/* Sine of value between 0 and 1 (non inclusive) */

#include <stdio.h>
#include <math.h>

int main(void)
{
    double radians = 0;
    printf("Enter the value in radians between 0 and 1: ");
    scanf("%lf", &radians);
    if (radians<=0 || radians >=1)
    {
        printf("Incorrect value !!!\n");
        return 0;
    }
    printf("SIN(%lf) = %lf \n", radians, sin(radians));
    return 0;
}
