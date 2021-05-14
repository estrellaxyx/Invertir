/* Este programa procesa cada fichero en el orden indicado (o en su defecto, la entrada */
/* estandar) leyendo cada linea del mismo (considerando un maximo de 2048 caracteres por linea) */
/* y emite por su salida estandar dichas lineas pero en orden inverso, es decir, la primera linea */
/* del primer fichero debera ser la ultima mostrada, y la primera mostrada la ultima del ultimo */
/* fichero.*/

/*Idea: Lee los ficheros linea por linea, */
/*guardar cada linea en una lista enlazada de forma que siempre se inserta en el principio de la lista.*/
/*Imprimir la lista*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXIMO 2048

#include "auxiliar.h"

//definir la estructura de la lista enlazada
typedef struct nodo
{
	char *dato;
	struct nodo *enlace;
} LISTA;

/*Prototipos de funciones*/
//insertar el *elemento en el principio de la lista **ptr
void insertar_primero(LISTA **ptr, char *elemento);

//lee las lineas del fichero *paf e inseertar cada linea a la lista
void leer_linea(FILE *paf, LISTA ** ptr);

void Ayuda(void)
{
	fprintf(stdout, "%s: Uso: %s [ fichero...]\n", argv0, argv0);
	fprintf(stdout, "%s: Invierte el orden de las lineas de los ficheros (o de la entrada).\n", argv0);
	exit(0);
}

void leer_linea(FILE *paf, LISTA **ptr)
{
	char linea[MAXIMO];
	while(fgets(linea,sizeof(linea),paf) != NULL)  insertar_primero(ptr, linea); 
}


void insertar_primero(LISTA **ptr, char *elemento)
{
	//Asignamos memoria suficiente para un puntero.
	LISTA * p1 = malloc(sizeof(LISTA));
	if(p1==NULL)
	{
		Error(EX_OSERR, "No se pudo ubicar la memoria dinámica necesaria.");
		exit(EX_OSERR);
	}
	//Insertar el elemento al puntero
	p1->dato = strdup(elemento);
	
	//Hacemos dicho puntero que sea el nodo anterior del primer nodo de la lista
	p1->enlace = *ptr;
	
	//Recorremos un puntero hacia el primero
	*ptr = p1;

}

int main(int argc, char * argv[])
{
	argv0 = "bocabajo";
	FILE * paf;
	int k;
	LISTA * n1 = NULL;
	
	if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
			Ayuda();
	}
	
	//Asignar memoria para n1;
	n1 = malloc(MAXIMO);
	if(n1==NULL) 
	{
		Error(EX_OSERR, "No se pudo ubicar la memoria dinámica necesaria.");
		exit(EX_OSERR);
	}
	
	
	if(argc > 1){//Para ficheros como argumentos de entrada
		for(k=1; k<argc;k++){
	
			if( (paf = fopen(argv[k],"r")) !=NULL )
			{
				leer_linea(paf, &n1);
				fclose(paf);//hay que cerrar los ficheros que hemos abierto
			}
			else
			{
				Error(EX_NOINPUT, "El fichero \"%s\" no puede ser leido.", argv[k]);
				exit(EX_NOINPUT);
			}
		}
	}
	else
	{//para entrada estandar
		if(stdin == NULL)
		{
			Error(EX_NOINPUT, "El fichero \"%s\" no puede ser leido.", stdin);
			exit(EX_NOINPUT);
		}
		leer_linea(stdin, &n1);
		
	}
	//Imprimimos la lista
	while(n1->enlace != NULL)
		{
			printf("%s",n1->dato);
			n1 = n1->enlace;
		}
	//Liberamos la memoria asignada
	free(n1);
	
	//Error(EX_OK,"Terminación correcta.");
	exit(EX_OK);
}
