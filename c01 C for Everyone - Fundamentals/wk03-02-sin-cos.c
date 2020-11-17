/*
Branko Milosevic
C for Everyone
wk02 assignment 02
/*

/* Sin and Cos value of interval 0->1 */

#include<stdio.h>
#include<math.h> /* has sin(), cos() */

int main(void)
{
    double interval;
    int i;
    for(i = 0; i < 11; i++)
    {
         interval = i/10.0;
         printf("Sin( %lf ) = %lf \t Cos( %lf ) = %lf\n", interval, sin(interval), interval, cos(interval) );
    }

    printf("\n+++++++\n");
    return 0;
}
