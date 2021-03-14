#include <stdio.h>

#include <stdlib.h>

typedef struct NODE_T 
{
    int x;
    int y;
    struct NODE_T *next;
    struct NODE_T *previous;
} NODE_T;

typedef struct LIST_T 
{
    NODE_T *head;
    NODE_T *tail;
} LIST_T;


// cria nodo
NODE_T *createNode(int x, int y);

// cria lista
LIST_T *createList();

// insere elemento na lista 
void push(LIST_T *list, int x, int y);

// busca elemento na lista
NODE_T *searchElement(LIST_T *list, int x, int y);

// remove elemento na list a
void pop(LIST_T *list, int x, int y);

// mostra lista
void showLists(LIST_T *list);

// esvazia lista
void deleteList(LIST_T *list);

// destroi list elemento na lista
void destroyList(LIST_T *list);



