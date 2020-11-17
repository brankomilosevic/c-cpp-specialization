/*Branko Milosevic
Coursera C for Everyone: Structured Programming
wk03-02 Honors Assignment
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct tree_numbers
{
    int data;
    struct tree_numbers *left;
    struct tree_numbers *right;
} node;

int* read_data (const char* file_name, int *n)
{
    FILE* file = fopen (file_name, "r");
    int i = 0;
    int number;
    int *data;

    fscanf (file, "%d", n);
    // printf ("\nnumber of elements: %d", *n);

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

void print_array(int *data, int n, char *message)
{
    int i;
    printf ("\n\n%s\n", message);
    for (i = 0; i < n; i++)
        printf (" %02d ", data[i]);
    printf ("\n<< End of data >>\n");
}

/* head of the list is the address of the first element (secondary level of pointer referencing) */
node* create_node(int data)
{
    node* new_node = (node* ) malloc(sizeof(node));

    new_node->data = data;
    new_node->left = 0;
    new_node->right = 0;

    return new_node;
}

/* recursive approach */
node* generate_tree (node *root, int *data, int i, int n)
{
    if (i < n)
    {
        node* temp = create_node(data[i]);
        root = temp;

        root->left  = generate_tree(root->left, data, 2 * i + 1, n);
        root->right = generate_tree(root->right, data, 2 * i + 2, n);
    }
    return root;
}

void print_tree(node* root)
{
    if (root != NULL)
    {
        print_tree(root->left);
        printf (" %02d ", root->data);
        print_tree(root->right);
    }
}

int main()
{
    int *data;
    int number_of_elements = 0;
    node *root = 0;

    data = read_data("input.txt", &number_of_elements);
    print_array(data, number_of_elements, "Initial data");
    root = generate_tree(root, data, 0, number_of_elements);
    printf("\n\nHere how tree data looks like\n");
    print_tree(root);
    printf ("\n<< End of tree representation >>\n");
    return 0;
}
