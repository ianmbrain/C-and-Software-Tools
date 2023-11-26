/**
    @file map.c
    @author
    Implementation for the map component, a hash map.
  */

#include "map.h"
#include <stdlib.h>

#include "value.h"
// Only using for testing, can get rid of this _---__-_-_-_-----______--______--____----__-
#include <stdio.h>

typedef struct MapPairStruct MapPair;

/** Key/Value pair to put in a hash map. */
struct MapPairStruct {
  /** Key part of this node, stored right in the node to improve locality. */
  Value key;
  
  /** Value part of this node, stored right in the node to improve locality. */
  Value val;
  
  /** Pointer to the next node at the same element of this table. */
  MapPair *next;
};

/** Representation of a hash table implementation of a map. */
struct MapStruct {
  /** Table of key / value pairs. */
  MapPair **table;

  /** Length of the table. */
  int tlen;
  
  /** Number of key / value pairs in the map. */
  int size;
};

Map *makeMap( int len )
{
  Map *map_ptr = ( Map * ) malloc( sizeof( Map ) );

  map_ptr->table = ( MapPair ** ) malloc( len * sizeof( MapPair * ) );
  for ( int i = 0; i < len; i++ ) {
    *( map_ptr->table + i ) = NULL;
  }
  map_ptr->tlen = len;
  map_ptr->size = 0;

  return map_ptr;
}

int mapSize( Map *m )
{
  // int cum_size = 0;

  // for ( int i = 0; i < m->tlen; i++ ) {
  //   MapPair *temp_mapPair = *( m->table );

  //   while ( temp_mapPair != NULL ) {
  //     cum_size++;
  //     temp_mapPair = temp_mapPair->next;
  //   }
  // }

  // return cum_size;

  return m->size;
}

void mapSet( Map *m, Value *key, Value *val )
{
  unsigned int hash_val = key->hash( key );
  hash_val = hash_val % m->tlen;

  // Replace the value for the given value if the key already exists in the map.
  MapPair **cur_pair = &( m->table[ hash_val ] ) ;
  while ( *cur_pair && !( *cur_pair )->key.equals( &( *cur_pair )->key, key ) ) {
    cur_pair = &( *cur_pair )->next;
  }

  // Replace the value for the given value if the key already exists in the map.
  if ( *cur_pair ) {
    ( *cur_pair )->val = *val;
    // ( *cur_pair )->val.move( &( *cur_pair )->val, val );
  }
  // Create a new map pair and add it to the table if the key does not exist.
  else {
    MapPair *new_pair = ( MapPair * ) malloc( sizeof( MapPair ) );
    new_pair->key = *key;
    new_pair->val = *val;
    new_pair->next = *cur_pair;
    *cur_pair = new_pair;
    m->size++;
    
    // printf( "\nHERE\n" );

    // free( key );
    // free( val );
  }

  // while ( cur_pair->next != NULL ) {
  //   if ( cur_pair->key.equals( &( cur_pair->key ), key ) ) {
  //     cur_pair->val = *val;

  //     cur_pair = cur_p
  //     break;
  //   }
  // }
}

Value *mapGet( Map *m, Value *key )
{
  // Us electure 15 for linked lists, maybe check lecture 21 to see if it has a similar data structure

  unsigned int hash_val = key->hash( key );
  hash_val = hash_val % m->tlen;

  MapPair **cur_pair = &( m->table[ hash_val ] );

  // printf( "\n%d\n", (*cur_pair)->val.ival );

  while ( *cur_pair && !( *cur_pair )->key.equals( &( *cur_pair )->key, key ) ) {
    cur_pair = &( *cur_pair )->next;
  }

  if ( *cur_pair ) {
    return &( *cur_pair )->val;
  }
  else {
    return NULL;
  }
}

bool mapRemove( Map *m, Value *key )
{
  unsigned int hash_val = key->hash( key );
  hash_val = hash_val % m->tlen;

  MapPair **cur_pair = &( m->table[ hash_val ] );

  while ( *cur_pair && !( *cur_pair )->key.equals( &( *cur_pair )->key, key ) ) {
    cur_pair = &( *cur_pair )->next;
  }

  if ( *cur_pair ) {
    MapPair *remove_pair = *cur_pair;
    *cur_pair = ( *cur_pair )->next;

    free( remove_pair );
    m->size--;
    return true;
  }
  else {
    return false;
  }
}

void freeMap( Map *m )
{
  // may need to free memory for key/values, these aren't heap though???? )___---__----__--_----_____--_-------__-_--_-_-_-_-_--_---

  // Free the memory within each table item.
  for ( int i = 0; i < m->tlen; i++ ) {
    // Maybe should be table + i --_____--_----_-_-_-_-_-_---
    MapPair **cur_pair = &( m->table[ i ] );

    while ( *cur_pair ) {
      MapPair *free_pair = *cur_pair;
      *cur_pair = ( *cur_pair )->next;

      free( free_pair );
    }
  }

  // Free memory for the table.
  free( m->table );

  // Free the map struct;
  free( m );
}
