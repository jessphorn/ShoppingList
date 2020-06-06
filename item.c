/**
  @file item.c
  @author Jessica Horn (jphorn)

  Defines the item struct and functions for working with items in the shopping list.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "item.h"

/** The number of item descriptors that should be read from the supplied string. */
#define NUM_DESC 3

/** The id assigned to each item. */
static int itemID = 0;

Item *readItem( char *str )
{
  char store[ STORE_MAX + 1 ];
  double price = 0;
  char name[ strlen(str) ];
  int descriptors = sscanf( str, "%13s %lf %[^\n]s", store, &price, name );
  if ( descriptors != NUM_DESC || store[ strlen( store ) ] != '\0' ) {
    return NULL;
  }
  itemID++;
  Item *i = ( Item * )malloc( sizeof ( Item ) );
  ( *i ).id = itemID;
  strcpy( ( *i ).store, store );
  ( *i ).price = price;
  ( *i ).name = ( char * )malloc( strlen( name ) + 1 );
  strcpy( ( *i ).name, name );
  return i;
}

void freeItem( Item *it )
{
  free( it->name );
  free( it );
}
