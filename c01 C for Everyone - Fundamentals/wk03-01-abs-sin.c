/*
Branko Milosevic
C for Everyone
wk02 assignment 01
/*

/* Absolute value of Sin of value */

#include<stdio.h>
#include<math.h> /* has  sin(), abs(), and fabs() */

int main(void)
{
    double interval;
    int i;
    for(i = 0; i < 30; i++)
    {
         interval = i/10.0;
         printf("sin( %lf ) = %lf \t ABS( sin( %lf ) ) = %lf\n", interval, sin(interval), interval, fabs(sin(interval)) );
    }

    printf("\n+++++++\n");
    return 0;
}
