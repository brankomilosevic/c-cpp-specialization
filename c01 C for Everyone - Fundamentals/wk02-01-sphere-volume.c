/*
Branko Milosevic
C for Everyone
wk02 assignment 01
/*

/* Volume of the sphere */

#include<stdio.h>
#define PI 3.14159
int main(void)
{
    int radius;
    printf("Enter radius: ");
    scanf("%d", &radius);
    printf("Volume is : %lf \n\n", 4 * PI * radius*radius*radius/3.0 );
    return 0;
}
