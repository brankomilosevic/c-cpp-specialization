/*
Branko Milosevic
Coursera C for Everyone: Structured Programming
wk03-01
*/

#include <stdio.h>
#include <stdlib.h>

#define NUM_ELEMENTS 100

typedef struct ll_numbers
{
    int data;
    struct ll_numbers *next;
} node;

/* head of the list is the address of the first element (secondary level of pointer referencing) */
void create_node(node **head, int n)
{
    node* new_node = (node* ) malloc(sizeof(node));
    if (new_node != 0)
    {
        new_node->next = *head;
        new_node->data = n;
        *head = new_node;
    }
    else
        printf ("\nError in creating new node!!!\n");
}

/* adding elements to the head of the list */
void generate_numbers (node **head, int n)
{
    int i;
    int d;
    srand(time(0));

    for (i = 0; i < n; i++)
    {
        d = rand() % n;
        create_node(head, d);
    }
}

/* bubble sort, number of elements is needed in order to pass the list two times */
void sort_numbers (node *head, int n)
{
    int i = 0;
    int j = 0;
    node *current;
    int temp;

    for (i = 0; i < n-1 ; i++)
    {
        current = head;

        while (current->next != 0)
        {
            if (current->data > current->next->data)
            {
                temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
            }
            current = current->next;
        }
    }
}

void print_numbers(node *head, char *message)
{
    int i = 0;
    node* current = head;

    printf ("\n<<< %s >>>\n", message);

    while (current != 0)
    {
        printf("%02d  ", current->data);
        i++;
        if (i % 5 == 0) printf ("\n");
        current = current->next;
    }
    printf ("\n");
}

int main()
{
    node* head = 0;
    generate_numbers(&head, NUM_ELEMENTS);
    print_numbers(head, "Original list");
    sort_numbers(head, NUM_ELEMENTS);
    print_numbers(head, " Sorted list ");
}
