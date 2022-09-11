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
  Pair * newPair = createPair(key, value);
  size_t i = hash(key, map->capacity);

  while(1) {
    if(!map->buckets[i] || !map->buckets[i]->key) {
      map->buckets[i] = newPair;
      map->size++;
      map->current = i;
      break;
    }

    i++;
    if(i == map->capacity) i = 0;
  }
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair ** oldBuckets = map->buckets; //se copia el antiguo par

  size t capacity = map->capacity;
  size_t i;
  map->capacity *= 2; //se dobla la capacidad del arreglo
  map->buckets = (Pair **) calloc(map->capacity, sizeof(Pair *));
  map->size = 0;

  for(i = 0; i < map->capacity/2; i++) {
    if(oldBuckets[i]) insertMap(map, oldBuckets[i]->key, oldBuckets[i]->value);
  }
}

HashMap * createMap(long capacity) {
  HashMap * newMap = (HashMap *)malloc(sizeof(HashMap));

  if(!newMap) return NULL;
  
  newMap->buckets = (Pair **) calloc(capacity, sizeof(Pair *));
  newMap->size = 0;
  newMap->capacity = capacity;
  newMap->current = -1;
  
  return newMap;
}

void eraseMap(HashMap * map,  char * key) {    
  Pair * eliminatePair = searchMap(map, key);

  if(eliminatePair) {
    eliminatePair->key = NULL;
    map->size--;
  }
}

Pair * searchMap(HashMap * map,  char * key) { 
  size_t i = hash(key, map->capacity);
  size_t cont = 0;

  while(1) {
    if(!map->buckets[i] || !map->buckets[i]->key) break;
      
    if(is_equal(key, map->buckets[i]->key)) {
      map->current = i;
      return map->buckets[i];
    }

    i++;
    cont++;
      
    if(cont == map->capacity) break;
    if(i == map->capacity) i = 0;
  }
  
  return NULL;
}

Pair * firstMap(HashMap * map) {
  size_t i;

  if(!map->size) return NULL;
  
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
