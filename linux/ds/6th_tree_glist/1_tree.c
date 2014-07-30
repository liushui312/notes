#include <stdio.h>
#include <stdlib.h>

#define MAX     10

struct node_t {
    int data;
    struct node_t *left;
    struct node_t *right;
};

enum {FRO, MID, END, LEV};

struct node_t *max(struct node_t *);
struct node_t *min(struct node_t *);
void fro(struct node_t *);
void mid(struct node_t *);
void end(struct node_t *);
void lev(struct node_t *);
void show(struct node_t *);
void travel(struct node_t *, int flag);
int depth(struct node_t *);
int count(struct node_t *);
int find(int key, struct node_t *);
int delete(int key, struct node_t **);
void destroy(struct node_t *);
void turn_left(struct node_t **root);
void turn_right(struct node_t **root);
void balance(struct node_t **root);

int find(int key, struct node_t *root)
{
    if (root == NULL)
        return -1;

    while (1)
    {
        if (key > root->data)
        {
            if (root->right == NULL)
                return -1;
            root = root->right;
        }
        else if (key < root->data)
        {
            if (root->left == NULL)
                return -1;
            root = root->left;
        }
        else
            return root->data;
    }
}

int delete(int key, struct node_t **root)
{
    struct node_t *tail = *root, *parent = NULL, *new = NULL;

    if (tail == NULL)
        return -1;

    while (1)
    {
        if (key > tail->data)
        {
            parent = tail;
            if (tail->right == NULL)
                return -1;
            tail = tail->right;
        }
        else if (key < tail->data)
        {
            parent = tail;
            if (tail->left == NULL)
                return -1;
            tail = tail->left;
        }
        else
            break;
    }

    if (tail->left == NULL)
        new = tail->right;
    if (tail->right == NULL)
        new = tail->left;
    if (tail->left != NULL && tail->right != NULL)
    {
        new = tail->left;
        max(tail->left)->right = tail->right;

        /*
         *new = tail->right;
         *min(tail->right)->left = tail->left;
         */
    }

    free(tail);

    if (parent == NULL)
    {
        *root = new; 
        return 0;
    }

    if (key >= parent->data)
        parent->right = new;
    else
        parent->left = new;

    return 0;
}

void turn_left(struct node_t **root)
{
    struct node_t *tail = (*root)->right;

    (*root)->right = NULL;
    min(tail)->left = *root;
    *root = tail;
}

void turn_right(struct node_t **root)
{
    struct node_t *tail = (*root)->left;

    (*root)->left = NULL;
    max(tail)->right = *root;
    *root = tail;
}

void balance(struct node_t **root)
{
    int l, r;

    if (*root == NULL)
        return;

    l = depth((*root)->left);
    r = depth((*root)->right);

    while (abs(l - r) > 1)
    {
        if (l > r)
            turn_right(root);
        else
            turn_left(root);

        l = depth((*root)->left);
        r = depth((*root)->right);
    }

    balance(&(*root)->left);
    balance(&(*root)->right);
}

void destroy(struct node_t *root)
{
    if (root == NULL)
        return;
    destroy(root->left);
    destroy(root->right);
    free(root);
}

struct node_t *max(struct node_t *root)
{
    while (root->right != NULL)
        root = root->right;
    return root;
}

struct node_t *min(struct node_t *root)
{
    while (root->left != NULL)
        root = root->left;
    return root;
}

int count(struct node_t *root)
{
    if (root == NULL)
        return 0;
    return 1 + count(root->left) + count(root->right);
}

int depth(struct node_t *root)
{
    if (root == NULL)
        return 0;
    return 1 + (depth(root->left) > depth(root->right) ? depth(root->left) : depth(root->right));
}

int insert(int data, struct node_t **root)
{
    struct node_t *new = NULL, *tail = *root;

    new = (struct node_t *)malloc(sizeof(struct node_t));
    new->data = data;
    new->left = NULL;
    new->right = NULL;

    if (*root == NULL)
        *root = new;
    else
    {
        while (1)
        {
            if (data >= tail->data)
            {
                if (tail->right == NULL)
                {
                    tail->right = new; 
                    break;
                }
                tail = tail->right;
            }
            else if (data < tail->data)
            {
                if (tail->left == NULL)
                {
                    tail->left = new;
                    break;
                }
                tail = tail->left;
            }
        }
    }
}

void show(struct node_t *root) 
{
    static int level = 0;
    int i;

    if (root == NULL)
        return;
    level++;
    show(root->right);
    level--;
    for (i = 0; i < level; i++)
    {
        printf("\t"); 
    }
    printf("%d\n", root->data);
    level++;
    show(root->left);
    level--;
}

void fro(struct node_t *root)
{
    if (root == NULL)
        return;
    printf("%d ", root->data);
    fro(root->left); 
    fro(root->right); 
}

void mid(struct node_t *root)
{
    if (root == NULL)
        return;
    mid(root->left); 
    printf("%d ", root->data);
    mid(root->right); 
}

void end(struct node_t *root)
{
    if (root == NULL)
        return;
    end(root->left); 
    end(root->right); 
    printf("%d ", root->data);
}

void lev(struct node_t *root)
{
    struct node_t *queue[100];
    int end = 0, front = 0;

    queue[end++] = root;

    while (end != front)
    {
        root = queue[front++];
        printf("%d ", root->data);
        if (root->left != NULL)
            queue[end++] = root->left;
        if (root->right != NULL)
            queue[end++] = root->right;
    }
}

void travel(struct node_t *root, int flag)
{
    switch (flag)
    {
        case FRO:
            fro(root);
            break;
        case MID:
            mid(root);
            break;
        case END:
            end(root);
            break;
        case LEV:
            lev(root);
            break;
        default:
            break;
    }
    printf("\n");
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
