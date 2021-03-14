
#include "tad-lde.h"

// cria nodo da lista
NODE_T *createNode(int x, int y)
{

    NODE_T *temp;

    temp = malloc(sizeof(NODE_T));
    temp->x = x;
    temp->y = y;
    temp->next  = NULL;
    temp->previous  = NULL;

    return temp;
}


// cria lista
LIST_T *createList()
{

    LIST_T *temp;

    temp = malloc(sizeof(LIST_T));
    temp->head = NULL;
    temp->tail = NULL;

    return temp;
}


// adiciona elemento na lista
void push(LIST_T *list, int x, int y)
{

    if ( list->head == NULL && list->tail == NULL )
    {
        list->head = list->tail = createNode(x, y);
    }
    else 
    {
        NODE_T *temp = searchElement(list, x, y);

        if ( temp == NULL )
        {
            temp = createNode(x, y);
            list->tail->next = temp;
            temp->previous = list->tail; 
            list->tail = temp;
        }
    }

    return;
}


// busca elemento na lista
NODE_T *searchElement(LIST_T *list, int x, int y){

    NODE_T *aux = list->head; 

    while ( aux != NULL && (aux->x != x || aux->y != y) )
    {
        if ( aux->x == x && aux->y == y )
            return aux;

        aux = aux->next;
    }

    return aux;
}


// retira elemento da lista 
void pop(LIST_T *list, int x, int y)
{
    NODE_T *temp = searchElement(list, x, y);

    if ( temp != NULL )
    {
        if ( temp == list->head && temp == list->tail )
        {
            list->head = list->tail = NULL;
        }
        else if ( temp == list->head )
        {
            list->head       = list->head->next;
            list->head->previous = NULL;
        }
        else if ( temp == list->tail )
        {
            list->tail       = list->tail->previous;
            list->tail->next = NULL;
        }
        else
        {
            NODE_T *aux; 

            aux       = temp->previous;
            aux->next = temp->next;
            aux       = temp->next;
            aux->previous = temp->previous; 
        }

        free(temp);
    }

    return;
}


// mostra lista
void showLists(LIST_T *list)
{
    NODE_T *aux = list->head; 

    while ( aux != NULL )
    {
        printf("x: %d, y: %d\n", aux->x, aux->y);
        aux = aux->next;
    }

    printf("\n");
    return;
}


// deleta lista
void deleteList(LIST_T *list)
{
    if ( list->head != NULL && list->tail != NULL )
    {
        NODE_T* aux;

        while ( list->head != NULL )
        {
            aux = list->head;
            list->head = list->head->next;
            free(aux);
        }

        list->tail = NULL;
    }

    return;
}


// destroi a lista
void destroyList(LIST_T *list)
{
    deleteList(list);
    free(list);
    return;
}
