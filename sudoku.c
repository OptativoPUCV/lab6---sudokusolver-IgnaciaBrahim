#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int esta_en_lista(int *arr, int num, int size)
{
   int i;
   for(i = 0; i < size; i++)
   {
      if (arr[i] == num)
      {
         return 1;
      }
   }
   return 0;
}

int comprobar_filas(Node *n)
{
   int i, j;
   for (i = 0; i < 9 ; i++)
   {
      int fila[10] = {0};
      int real_index = 0; // cree una lista pa revisar que no se repitan los numeros, creo que no hay otra forma de hacerlo
      for(j = 0; j < 9; j++)
      {
         if (n->sudo[i][j] != 0)
         {
            int num = n->sudo[i][j];
            if (esta_en_lista(fila, num, real_index))
            {
               return 0;
            }
            fila[real_index] = num;
            real_index++;
         }
      }
   }
   return 1;
}

int comprobar_columnas(Node *n)
{
   int i, j;
   for (i = 0; i < 9 ; i++)
   {
      int columna[10] = {0};
      int real_index = 0; // cree una lista pa revisar que no se repitan los numeros, creo que no hay otra forma de hacerlo
      for(j = 0; j < 9; j++)
      {
         if (n->sudo[j][i] != 0)
         {
            int num = n->sudo[j][i];
            if (esta_en_lista(columna, num, real_index))
            {
               return 0;
            }
            columna[real_index] = num;
            real_index++;
         }
      }
   }
   return 1;
}

int comprobar_submatrices(Node *n)
{
   int k, p;
   for(k = 0; k < 9; k++)
   {
      int submatriz[10] = {0}; 
      int real_index = 0;
      for(p = 0; p < 9; p++)
      {
          int i = 3*(k/3) + (p/3);
          int j = 3*(k%3) + (p%3);
         
         if (n->sudo[i][j] != 0)
         {
            int num = n->sudo[i][j];
            if (esta_en_lista(submatriz, num, real_index))
            {
               return 0;
            }
            submatriz[real_index] = num;
            real_index++;
         }
      }
   }
   return 1;
}


int is_valid(Node* n)
{
   //No se repitan números en las filas
   //No se repitan números en las columnas
   //No se repitan números en las submatrices de 3x3
   //Si el estado es válido la función retorna 1, si no lo es retorna 0.
   int filas = comprobar_filas(n);
   int columnas = comprobar_columnas(n);
   int submatrices = comprobar_submatrices(n);
   return filas && columnas && submatrices;
}


List* get_adj_nodes(Node* n)
{
    List* list=createList();
    int i, j, k;
    for (i = 0; i < 9; i++)
    {
       for (j = 0; j < 9; j++)
       {
          if (n->sudo[i][j] == 0)
          {
             for(k = 1; k <= 9; k++)
             {
                 Node* cpy = copy(n);
                 cpy->sudo[i][j] = k;
                 if (is_valid(cpy))
                 {
                    pushBack(list, cpy);
                 }
                else
                 {
                    free(cpy);
                 }
             }
             return list;
             //los encuentra todos
          }
       }
    }
   return list;
}


int is_final(Node* n)
{
   List *aux = get_adj_nodes(n);
   if (get_size(aux) == 0)
   {
      return 1;
   }
    return 0;
}

Node* DFS(Node* initial, int* cont)
{
   Stack *stack = createStack();
   push(stack, initial);
   *cont = 0; //no sé si se inicializó en 0 en algún lado??
   while (top(stack) != NULL)
   {
      //saco/elimino.
      Node *aux_stack = top(stack);
      pop(stack);
      if (is_final(aux_stack))
      {
         return aux_stack;
      }
      //No es el estado final así que hay que obtener los vecinos:
      List *aux_list = get_adj_nodes(aux_stack);
      Node *list_index = first(aux_list);
      while (list_index != NULL)
      {
         push(stack, list_index); //se ponen en el stack cada uno de los elementos de la lista que son los vecinos :D
         list_index = next(aux_list); //avanzo
      }
      free(aux_stack); //liberé memoria del nodo 
      *cont = *cont + 1;
   }
   return NULL;
}


/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}
*/