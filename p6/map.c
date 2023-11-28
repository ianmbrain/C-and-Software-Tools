/**
    @file map.h
    @author Ian M Brain (imbrain)
    Implementation for the map component, a hash map.
    Provides functionality to dynamically allocate the map, return the map size,
    set values within the map, get values from the map, remove values from the map,
    and free the memory used in the map.
    The functionality is used in driver to work with the hash map and free the memory when finished.
  */

#include "map.h"
#include <stdlib.h>

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
  // Allocate memory for the map struct.
  Map *map_ptr = ( Map * ) malloc( sizeof( Map ) );

  // Allocate memory for the map table based on the size of len.
  map_ptr->table = ( MapPair ** ) malloc( len * sizeof( MapPair * ) );
  // Set each table index pointer to null.
  for ( int i = 0; i < len; i++ ) {
    *( map_ptr->table + i ) = NULL;
  }
  // Set the len and size fields of the map.
  map_ptr->tlen = len;
  map_ptr->size = 0;

  // Return the map pointer.
  return map_ptr;
}

int mapSize( Map *m )
{
  // Return the size of the map.
  return m->size;
}

void mapSet( Map *m, Value *key, Value *val )
{
  // Hash of the key value.
  unsigned int hash_val = key->hash( key );
  // Index of map table where the key hash should be stored.
  hash_val = hash_val % m->tlen;

  // Parse through the map table to get the pair corresponding to the given key.
  MapPair **cur_pair = &( m->table[ hash_val ] ) ;
  while ( *cur_pair && !( *cur_pair )->key.equals( &( *cur_pair )->key, key ) ) {
    cur_pair = &( *cur_pair )->next;
  }

  // Replace the value for the given value if the key already exists in the map.
  if ( *cur_pair ) {
    // Empty the old value of the pair before the new value is moved to it.
    ( *cur_pair )->val.empty( &( *cur_pair )->val );

    ( *cur_pair )->val.move( val, &( *cur_pair )->val );

    // If a new value is moved to the key, free the key used to parse the key value.
    key->empty( key );
  }
  // Create a new map pair and add it to the table if the key does not exist.
  else {
    MapPair *new_pair = ( MapPair * ) malloc( sizeof( MapPair ) );
    new_pair->key = *key;
    new_pair->val = *val;
    new_pair->next = *cur_pair;
    *cur_pair = new_pair;
    m->size++;
  }
}

Value *mapGet( Map *m, Value *key )
{
  // Hash of the key value.
  unsigned int hash_val = key->hash( key );
  // Index of map table where the key hash should be stored.
  hash_val = hash_val % m->tlen;

  // Parse through the map table to get the pair corresponding to the given key.
  MapPair **cur_pair = &( m->table[ hash_val ] );
  while ( *cur_pair && !( *cur_pair )->key.equals( &( *cur_pair )->key, key ) ) {
    cur_pair = &( *cur_pair )->next;
  }

  // Return pointer to the pair value if the key exists in the map.
  if ( *cur_pair ) {
    return &( *cur_pair )->val;
  }
  // Return null if the key does not exist in the map.
  else {
    return NULL;
  }
}

bool mapRemove( Map *m, Value *key )
{
  // Hash of the key value.
  unsigned int hash_val = key->hash( key );
  // Index of map table where the key hash should be stored.
  hash_val = hash_val % m->tlen;

  // Parse through the map table to get the pair corresponding to the given key.
  MapPair **cur_pair = &( m->table[ hash_val ] );
  while ( *cur_pair && !( *cur_pair )->key.equals( &( *cur_pair )->key, key ) ) {
    cur_pair = &( *cur_pair )->next;
  }

  // Remove the pair with the corresponding key from the map and return true indicating the removal.
  if ( *cur_pair ) {
    // Pair to remove from the map
    MapPair *remove_pair = *cur_pair;
    *cur_pair = ( *cur_pair )->next;

    // Empty the key and val memory. Free the memory used for the map pair.
    remove_pair->key.empty( &remove_pair->key );
    remove_pair->val.empty( &remove_pair->val );
    free( remove_pair );
    m->size--;
    return true;
  }
  // Return false if the key is not contained in the table.
  else {
    return false;
  }
}

void freeMap( Map *m )
{
  // Free the memory within each index of the table
  for ( int i = 0; i < m->tlen; i++ ) {
    // First pair of each index of the table.
    MapPair **cur_pair = &( m->table[ i ] );

    // Parse through the table index linked list and free each pair within it.
    while ( *cur_pair ) {
      // Pair to free from the table.
      MapPair *free_pair = *cur_pair;
      *cur_pair = ( *cur_pair )->next;

      // Empty the key and val memory. Free the memory used for the map pair.
      free_pair->key.empty( &free_pair->key );
      free_pair->val.empty( &free_pair->val );
      free( free_pair );
    }
  }

  // Free memory for the table.
  free( m->table );

  // Free the map struct;
  free( m );
}
