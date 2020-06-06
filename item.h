/**
  @file item.h
  @author Jessica Horn (jphorn)

  Defines the STORE_MAX constant and the Item struct. Provides prototypes for readItem and
  freeItem.
*/

/** Maximum length of a store name */
#define STORE_MAX 12

/** Representation for an item to be purchased. */
typedef struct {
  /** Unique ID for this particular item.  */
  int id;
  
  /** Store where we're supposed to buy the item.  */
  char store[ STORE_MAX + 1 ];
  
  /** Price of this item in dollars (but you might normally store this
      as an integer number of cents). */
  double price;
  
  /** Name of this item.  Pointer to a string of arbitrary length. */
  char *name;
} Item;

/**
  Creates a new Item.
  
  @param str A pointer to a string containing information about the item.
  @return A newly created item or NULL.
*/
Item *readItem( char * );

/**
  Frees the memory for the given item.
  
  @param it The item to be removed.
*/
void freeItem( Item * );
