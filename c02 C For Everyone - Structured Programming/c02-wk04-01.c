/*
Branko Milosevic
Coursera C for Everyone: Structured Programming
wk04-01
*/

#include <stdio.h>
#include <stdlib.h>

int* read_data (const char* file_name, int *n)
{
    FILE* file = fopen (file_name, "r");
    int i = 0;
    int number;
    int *data;

    fscanf (file, "%d", n);
    printf ("\nnumber of elements: %d", *n);

    data = (int *) malloc(sizeof(int) * (*n));

    while (!feof (file))
    {
        fscanf (file, "%d", &number);
        data[i] = number;
        i++;
    }
    fclose (file);

    return data;
}

void print_data(int *data, int n, char *message)
{
    int i;
    printf ("\n\n%s\n", message);
    for (i = 0; i < n; i++)
        printf (" %02d ", data[i]);
    printf ("\n<< End of data >>\n");
}

double avg_data(int *data, int n)
{
    double avg;
    int i;

    for (i = 0; i < n; i++)
        avg += data[i];

    return avg/n;
}

int min_data(int *data, int n)
{
    int min = INT_MAX;
    int i;

    for (i = 0; i < n; i++)
        if (min > data[i])
            min = data[i];

    return min;
}

int max_data(int *data, int n)
{
    int max = INT_MIN;
    int i;

    for (i = 0; i < n; i++)
        if (max < data[i])
            max = data[i];

    return max;
}

int main ()
{
    int *data;
    int number_of_elements = 0;

    data = read_data("input.txt", &number_of_elements);
    print_data(data, number_of_elements, "<< Data imported from file >>");
    printf("\n-> Average value:    %0.2f", avg_data(data, number_of_elements));
    printf("\n-> Minimum value:    %02d", min_data(data, number_of_elements));
    printf("\n-> Maximum value:    %02d", max_data(data, number_of_elements));
    printf("\n---end of the report---\n\n");
    return 0;
}
