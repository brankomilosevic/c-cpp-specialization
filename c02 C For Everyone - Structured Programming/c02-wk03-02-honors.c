/*
Branko Milosevic
Coursera C for Everyone: Structured Programming
wk03-02 Honors Assignment
*/

#include <stdio.h>
#include <stdlib.h>

#define NUM_ELEMENTS 200
#define MIN_ELEMENT 0
#define MAX_ELEMENT 49

typedef struct ll_numbers
{
    int data;
    struct ll_numbers *next;
    struct ll_numbers *previous;
} node;

/* head of the list is the address of the first element (secondary level of pointer referencing) */
void create_node(node **head, int n)
{
    node* new_node = (node* ) malloc(sizeof(node));

    new_node->data = n;

    new_node->next = *head;
    new_node->previous = 0;

    if ((*head) != 0)
        (*head)->previous = new_node;

    *head = new_node;
}

/* adding elements to the head of the list */
void generate_numbers (node **head, int n)
{
    int i;
    int d;
    srand(time(0));

    for (i = 0; i < n; i++)
    {
        d = MIN_ELEMENT + rand() % (MAX_ELEMENT - MIN_ELEMENT);
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
        if (i % 10 == 0) printf ("\n");
        current = current->next;
    }
    printf ("\n");
}

void remove_duplicates (node **head, int n)
{
    node* current = *head;
    node* to_remove = 0;

    while (current != 0 && current->next != 0)
    {
        //printf ("\n%d %d", current->data, current->next->data);
        if (current->data == current->next->data)
        {
            to_remove = current;
            //printf("<< to remove: %d >> ", current->data);

            if (to_remove->previous != 0 && to_remove->next != 0)
            {
                to_remove->next->previous = to_remove->previous;
                to_remove->previous->next = to_remove->next;
            }

            else if (to_remove == *head)
            {
                *head = to_remove->next;
                to_remove->next->previous = 0;
            }

            else if (to_remove->next == 0)
            {
                to_remove->previous->next = 0;
            }
        }

        current = current->next;
    }
}

int main()
{
    node* head = 0;
    generate_numbers(&head, NUM_ELEMENTS);
    print_numbers(head, "   Original list    ");
    sort_numbers(head, NUM_ELEMENTS);
    print_numbers(head, "    Sorted list     ");
    remove_duplicates(&head, NUM_ELEMENTS);
    print_numbers(head, " Removed duplicates ");
}
