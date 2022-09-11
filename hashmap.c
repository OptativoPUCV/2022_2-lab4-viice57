#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
  Pair ** buckets;
  long size; //cantidad de datos/pairs en la tabla
  long capacity; //capacidad de la tabla
  long current; //indice del ultimo dato accedido
};

Pair * createPair(char * key, void * value) {
  Pair * newPair = (Pair *) malloc(sizeof(Pair));
  
  newPair->key = key;
  newPair->value = value;
  
  return newPair;
}

long hash(char * key, long capacity) {
  unsigned long hash = 0;
  char * ptr;
  
  for(ptr = key; *ptr != '\0'; ptr++) {
    hash += hash*32 + tolower(*ptr);
  }
  
  return hash%capacity;
}

int is_equal(void* key1, void* key2){
  if(!key1|| !key2) return 0;
  if(strcmp((char*)key1,(char*)key2) == 0) return 1;
  
  return 0;
}

void insertMap(HashMap * map, char * key, void * value) {
  //creación del par
  Pair * newPair = createPair(key, value);

  //asignación del índice mediante f. hash
  size_t i = hash(key, map->capacity);

  while(1) {
    //comprobación de que exista el par
    if(!map->buckets[i] || !map->buckets[i]->key) {
      //asignamos  el nuevo par y cambiamos valores de la tabla.
      map->buckets[i] = newPair;
      map->size++;
      map->current = i;
      break;
    }

    i++;
    //al ser circular, se parte nuevamente desde 0
    if(i == map->capacity) i = 0;
  }
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair ** oldBuckets = map->buckets; //se copia el antiguo par

  size_t capacity = map->capacity; //se copia la anterior capacidad
  size_t i; //contador
  map->capacity *= 2; //se dobla la capacidad de la tabla
  map->buckets = (Pair **) calloc(map->capacity, sizeof(Pair *)); //nuevo par
  map->size = 0;

  //ciclo para la inserción de los elementos en la "nueva" tabla
  for(i = 0; i < capacity; i++) {
    if(oldBuckets[i]) insertMap(map, oldBuckets[i]->key, oldBuckets[i]->value);
  }
}

HashMap * createMap(long capacity) {
  //creación del mapa
  HashMap * newMap = (HashMap *)malloc(sizeof(HashMap));

  //comprobación de que exista
  if(!newMap) return NULL;

  //definiendo un nuevo par y valores por defecto de la tabla
  newMap->buckets = (Pair **) calloc(capacity, sizeof(Pair *));
  newMap->size = 0;
  newMap->capacity = capacity;
  newMap->current = -1;
  
  return newMap;
}

void eraseMap(HashMap * map,  char * key) {
  //busqueda del elemento a partir de la clave
  Pair * eliminatePair = searchMap(map, key);

  //si existe, su clave queda en null y se reduce el tamaño de la tabla
  if(eliminatePair) {
    eliminatePair->key = NULL;
    map->size--;
  }
}

Pair * searchMap(HashMap * map,  char * key) { 
  //variable "i" para asignar el índice; variable "cont" para no exceder capacidad
  size_t i = hash(key, map->capacity);
  size_t cont = 0;

  while(1) {
    //comprobación de que exista el par
    if(!map->buckets[i] || !map->buckets[i]->key) break;

    //si son iguales, se cambia el current al actual "i"
    if(is_equal(key, map->buckets[i]->key)) {
      map->current = i;
      return map->buckets[i];
    }

    i++;
    cont++;

    //comprobación de que no se exceda la capacidad del arreglo
    if(cont == map->capacity) break;

    //al ser circular, se parte nuevamente desde 0
    if(i == map->capacity) i = 0;
  }
  
  return NULL;
}

Pair * firstMap(HashMap * map) {
  size_t i;

  //si la tabla no tiene tamaño, no existe.
  if(!map->size) return NULL;

  //ciclo para buscar el primer elemento, siempre y cuando exista algo.
  for(i = 0; i < map->capacity; i++) {
    if(map->buckets[i] && map->buckets[i]->key) {
      map->current = i;
      return map->buckets[i];
    }
  }
  
  return NULL;
}

Pair * nextMap(HashMap * map) {
  size_t i;
  
  if(!map->size) return NULL;
  
  for(i = map->current + 1; i < map->capacity; i++) {
    if(map->buckets[i] && map->buckets[i]->key) {
      map->current = i;
      return map->buckets[i];
    }
  }
  
  return NULL;
}
