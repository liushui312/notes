#include <stdio.h>
#include <stdlib.h>

#define MAX     10

struct node_t {
    int data;
    struct node_t *left;
    struct node_t *right;
};

enum {FRO, MID, END, LEV};

struct node_t *max(struct node_t *root)
{
    if (root->right != NULL) 
        root = root->right;
    return root;
}

struct node_t *min(struct node_t *root)
{
    if (root->left != NULL)
        root = root->left;
    return root;
}

void fro(struct node_t *root)
{
    if (root == NULL)
        return;
    printf("%d ",root->data);
    fro(root->left);
    fro(root->right);
}

void mid(struct node_t *)
{
    if (root == NULL)
        return;
    mid(root->left);
    printf("%d ",root->data);
    mid(root->right);
}

void end(struct node_t *root)
{
  if (root == NULL)
        return;
    mid(root->left);
    mid(root->right);
    printf("%d ",root->data);

}

void lev(struct node_t *root)
{
    struct node_t *queue[100];
    int end = 0, front = 0;

    queue[end++] = root;

    while(end != front)
    {
        root = queue[front++];
        printf("%d ",root->data);
        if (root->left != NULL)
            queue[end++] = root->left;
        if (root->right != NULL)
            queue[end++] = root->right;
    }

}

void show(struct node_t *)
{

}

void travel(struct node_t *, int flag)
{

}

int depth(struct node_t *)
{
     
}

int count(struct node_t *)
{

}

int find(int key, struct node_t *)
{

}

int delete(int key, struct node_t **)
{

}

void destroy(struct node_t *)
{

}

void turn_left(struct node_t **root)
{

}

void turn_right(struct node_t **root)
{

}

void balance(struct node_t **root)
{

}



int main(void)
{
    struct node_t *root = NULL;
    int i, n;

    for (i = 0; i < MAX; i++)
    {
        n = rand() % 100; 
        printf("%d ", n);
        insert(n, &root);
    }
    printf("\n");

    printf("fro:");
    travel(root, FRO);
    printf("mid:");
    travel(root, MID);
    printf("end:");
    travel(root, END);
    printf("lev:");
    travel(root, LEV);
    printf("max = %d, min = %d, count = %d, depth = %d\n", 
            max(root)->data, min(root)->data, 
            count(root), depth(root));

    show(root);
    balance(&root);
    printf("========== balance ==============\n");
    show(root);


    while (1)
    {
        printf("please input delete key : ");
        scanf("%d", &n);

        if (n == -1)
            break;

        delete(n, &root);
        printf("========== delete ==============\n");
        show(root);
        balance(&root);
        printf("========== balance ==============\n");
        show(root);
    }

    return 0;
}
