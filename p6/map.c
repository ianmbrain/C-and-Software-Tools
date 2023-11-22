/**
    @file map.c
    @author
    Implementation for the map component, a hash map.
  */

#include "map.h"
#include <stdlib.h>

#include "value.h"

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
