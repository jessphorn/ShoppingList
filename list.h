/**
  @file list.h
  @author Jessica Horn (jphorn)

  Defines the INC_BY_FACTOR constant and the ShoppingList struct. Provides prototypes for the
  makeShoppingList, freeShoppingList, shoppingListAdd, shoppingListRemove, and shoppingListReport
  functions.
*/

#include "item.h"
#include <stdbool.h>

/** The amount to increase the size of the array of Items by. */
#define INC_BY_FACTOR 2

/** Representation for a shopping list, an arbitrary-length list of Items. */
typedef struct {
  /** Resizable array of pointers to Items. */
  Item **items;
  
  /** Current number of items on the list. */
  int length;
  
  /** Current capacity of the list, how many pointers we have room for. */
  int capacity;
} ShoppingList;

/**
  Creates an empty shopping list.

  @return A pointer to a ShoppingList.
*/
ShoppingList *makeShoppingList();

/**
  Frees the memory for the given ShoppingList.

  @param list The list to be removed.
*/
void freeShoppingList( ShoppingList *list );

/**
  Adds an Item to a ShoppingList.

  @param list The ShoppingList to which an Item will be added.
  @param it The Item to add to a ShoppingList.
*/
void shoppingListAdd( ShoppingList *list, Item *it );

/**
  Removes an Item from a ShoppingList.

  @param list The ShoppingList from which an Item will be removed.
  @param id The id of the Item to be removed from the ShoppingList.
  @return true if the Item was removed from the ShoppingList.
*/
bool shoppingListRemove( ShoppingList *list, int id );

/**
  Reports the selected Items and computes their total cost.

  @param list The ShoppingList from which to create the report.
  @param test A function pointer used to to determine which Items to report on.
  @param arg An argument with which to compare item details.
*/
void shoppingListReport( ShoppingList *list, bool test( Item *it, void *arg ), void *arg );
