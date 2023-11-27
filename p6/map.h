/** 
    @file map.h
    @author Ian M Brain (imbrain)
    Header for the map component, a hash map.
    Provides functionality to dynamically allocate the map, return the map size,
    set values within the map, get values from the map, remove values from the map,
    and free the memory used in the map.
    The functionality is used in driver to work with the hash map and free the memory when finished.
*/

#ifndef MAP_H
#define MAP_H

#include "value.h"
#include <stdbool.h>

/** Incomplete type for the Map representation. */
typedef struct MapStruct Map;

/** Make an empty map.
    Comments were provided by teaching staff for project 6.
    @param len Initial length of the hash table.
    @return pointer to a new map.
*/
Map *makeMap( int len );

/** Get the size of the given map.
    Comments were provided by teaching staff for project 6.
    @param m Pointer to the map.
    @return Number of key/value pairs in the map. */
int mapSize( Map *m );
  
/** Add a new key / value pair to the map, or replace the value
    associeted with the given key.  The map will take ownership of the
    given key and value objects.
    Comments were provided by teaching staff for project 6.
    Functionality to parse through the linked list is based on code for lecture 15 on 10/19/23.
    @param m Map to add a key/value pair to.
    @param k Key to add to map.
    @param v Value to associate with the key.
*/
void mapSet( Map *m, Value *key, Value *val );

/** Return the value associated with the given key. The returned Value
    is still owned by the map.
    Comments were provided by teaching staff for project 6.
    Functionality to parse through the linked list is based on code for lecture 15 on 10/19/23.
    @param m Map to query.
    @param k Key to look for in the map.
    @return Value associated with the given key, or NULL if the key
    isn't in the map.
*/
Value *mapGet( Map *m, Value *key );

/** Remove a key / value pair from the given map.
    Comments were provided by teaching staff for project 6.
    Functionality to parse through the linked list is based on code for lecture 15 on 10/19/23.
    @param m Map to remove a key from
    @param k Key to look for and remove in the map.
    @return true if the key was successfully removed (i.e., it was in
    the map)
*/
bool mapRemove( Map *m, Value *key );

/** Free all the memory used to store a map, including all the
    memory in its key/value pairs.
    Comments were provided by teaching staff for project 6.
    Functionality to parse through the linked list is based on code for lecture 15 on 10/19/23.
    @param m The map to free.
*/
void freeMap( Map *m );
  
#endif
