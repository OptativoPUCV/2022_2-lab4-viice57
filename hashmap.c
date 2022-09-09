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

Pair * createPair(char * key,  void * value) {
    Pair * new = (Pair *) malloc(sizeof(Pair));
  
    new->key = key;
    new->value = value;
  
    return new;
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
    Pair * new = createPair(key, value);
    size_t i = hash(key, map->capacity);

    while(1) {
      if(!map->buckets[i] || !map->buckets[i]->key) {
        map->buckets[i] = new;
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


}


HashMap * createMap(long capacity) {
    HashMap * map = (HashMap *)malloc(sizeof(HashMap));

    if(!map) return NULL;
    map->buckets = (Pair **) calloc(capacity, sizeof(Pair *));
    map->size = 0;
    map->capacity = capacity;
    map->current = -1;
  
    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    Pair * eliminate = searchMap(map, key);

    if(eliminate) {
      eliminate->key = NULL;
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
    //Pair * first = searchMap(map, key);
    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
