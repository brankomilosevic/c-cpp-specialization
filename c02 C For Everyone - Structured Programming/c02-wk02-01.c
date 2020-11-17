#include <stdio.h>
#include <stdlib.h>

#define BASE_SALARY 50000
#define RANGE_SALARY 20000

typedef enum month { jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec} month;
typedef struct date{ month m; int d;} date;

typedef enum department { HR, Sales, Research, Software, Executive } department;
typedef struct employee { department dep; int salary; unsigned ssn; } employee;

date next_day(date day)
{
    date next_day;
    int max_day;

    switch (day.m)
    {
        case jan: max_day = 31; break;
        case feb: max_day = 28; break;
        case mar: max_day = 31; break;
        case apr: max_day = 30; break;
        case may: max_day = 31; break;
        case jun: max_day = 30; break;
        case jul: max_day = 31; break;
        case aug: max_day = 31; break;
        case sep: max_day = 30; break;
        case oct: max_day = 31; break;
        case nov: max_day = 30; break;
        case dec: max_day = 31; break;
        default: max_day = 0;
    }

    if (day.d == max_day)
    {
        next_day.d = 1;
        next_day.m = (day.m + 1) % 12;
    }
    else
    {
        next_day.d = day.d + 1;
        next_day.m = day.m;
    }

    return next_day;
}

void print_date (date day)
{
    printf ("");
    switch (day.m)
    {
        case jan: printf("January   "); break;
        case feb: printf("February  "); break;
        case mar: printf("March     "); break;
        case apr: printf("April     "); break;
        case may: printf("May       "); break;
        case jun: printf("June      "); break;
        case jul: printf("July      "); break;
        case aug: printf("August    "); break;
        case sep: printf("September "); break;
        case oct: printf("October   "); break;
        case nov: printf("November  "); break;
        case dec: printf("December  "); break;
        default:  printf("error !!!");
    }
    printf("%d.", day.d);
}

int main()
{
    date d;
    employee emp[10];
    int i;

    d.m = 0; d.d = 1; print_date(d); printf("\tNext day:\t"); print_date(next_day(d)); printf("\n");
    d.m = 1; d.d =28; print_date(d); printf("\tNext day:\t"); print_date(next_day(d)); printf("\n");
    d.m = 2; d.d =14; print_date(d); printf("\tNext day:\t"); print_date(next_day(d)); printf("\n");
    d.m = 9; d.d =31; print_date(d); printf("\tNext day:\t"); print_date(next_day(d)); printf("\n");
    d.m =11; d.d =31; print_date(d); printf("\tNext day:\t"); print_date(next_day(d)); printf("\n");
    printf("\n");

    srand(time(0));

    for (i = 0; i < 10; i++)
    {
        emp[i].dep    = rand() % 4;
        emp[i].salary = BASE_SALARY + (rand() % RANGE_SALARY);
        emp[i].ssn    = 123456789 + i;
    }

    for (i = 0; i < 10; i++)
    {
        printf("Employee: #%02d\tDepartment: %d\tSalary: %d\tSSN: %09d\n", i, emp[i].dep, emp[i].salary, emp[i].ssn);
    }
}
