/**
  @file list.c
  @author Jessica Horn (jphorn)

  Defined the ShoppingList struct and functions for working with a ShoppingList.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"

/** Initial capacity for a shopping list. */
#define CAP 10
/** Initial length of a shopping list. */
#define LEN 0

ShoppingList *makeShoppingList()
{
  ShoppingList *s = ( ShoppingList * )malloc( sizeof( ShoppingList ) );
  s->items = ( Item ** )malloc( CAP * sizeof( Item * ) );
  s->length = LEN;
  s->capacity = CAP;
  return s;
}

void freeShoppingList( ShoppingList *list )
{
  for ( int i = 0; i < list->length; i++ ) {
    freeItem( list->items[ i ] );
  }
  free( list->items );
  free( list );
}

void shoppingListAdd( ShoppingList *list, Item *it )
{
  if ( list->length >= list->capacity ) {
    list->capacity *= INC_BY_FACTOR;
    list->items = ( Item ** )realloc( list->items, list->capacity * sizeof( Item * ) );
  }
  list->items[ list->length++ ] = it;
}

bool shoppingListRemove( ShoppingList *list, int id )
{
  for ( int i = 0; i < list->length; i++ ) {
    if ( id == list->items[ i ]->id ) {
      Item *it = list->items[ i ];
      for ( int j = i; j < list->length; j++ ) {
        list->items[ j ] = list->items[ j + 1 ];
      }
      freeItem( it );
      list->length--;
      return true;
    }
  }
  return false;
}

void shoppingListReport( ShoppingList *list, bool test( Item *it, void *arg ), void *arg )
{
  double total = 0;
  printf( "\n" );
  for ( int i = 0; i < list->length; i++ ) {
    bool toPrint = test( list->items[ i ], arg );
    if ( toPrint ) {
      Item **arrayOfPointers = list->items;
      Item *pointerToItem = arrayOfPointers[ i ];
      int id = ( *pointerToItem ).id;
      char *store = ( *pointerToItem ).store;
      double price = ( *pointerToItem ).price;
      char *name = ( *pointerToItem ).name;
      printf( "%4d %-12s %7.2f %s\n", id, store, price, name );
      total += price;
    }
  }
  printf( "                  %7.2f", total);
}
