/**
  @file shopping.c
  @author Jessica Horn (jphorn)

  Defines the main function and is responsible for reading user commands and delegating the
  commands to other functions.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"

/** The initial capacity for a command line read from user or file input. */
#define INIT_CAP 20
/** The maximum length of a valid command. */
#define CMD_LEN 6
/** The length of the add command. */
#define ADD_LEN 3
/** Position from stdin - used by addItem to know if reason is invalid command or item. */
#define POS_STDIN 0
/** The number of arguments needed by the user after the add command. */
#define ADD_ARG 3

/** A pointer to a ShoppingList. */
ShoppingList *list;

/**
  Reads a line of input.

  @param stream An input stream.
  @return A pointer to a dynamically-allocated string containing the input line or NULL.
*/
static char *getLine( FILE *stream )
{
  int capacity = INIT_CAP;
  int length = 0;
  char *line = ( char * )malloc( INIT_CAP );
  char c = fgetc( stream );
  while ( c != EOF && c != '\n' ) {
    if ( length >= capacity ) {
      capacity *= INC_BY_FACTOR;
      line = ( char * )realloc( line, capacity );
    }
    line[ length++ ] = c;
    c = fgetc( stream );
  }
  if ( length >= capacity ) {
    capacity++;
    line = ( char * ) realloc( line, capacity );
  }
  line[ length ] = '\0';
  if ( c == EOF ) {
    free( line );
    return NULL;
  }
  return line;
}

/** Adds an item to the shopping list.

  @param line A pointer to a string containing information about the item.
*/
static void addItem( char *line, int location )
{
  Item *i = readItem( line );
  if ( i != NULL ) {
    shoppingListAdd( list, i );
  } else {
    if ( location != POS_STDIN ) {
      printf( "\nInvalid item, line %d", location );
    }
  }
}

/**
  Loads an input file.

  @param fileName A pointer to a string containing the name of the input file.
*/
static void loadInput( char *fileName )
{
  FILE *f = fopen( fileName, "r" );
  if ( f == NULL ) {
    printf( "\nCan't open file" );
  } else {
    int lineNumber = 1;
    char *line = getLine( f );
    while ( line != NULL ) {
      addItem( line, lineNumber);
      free( line );
      lineNumber++;
      line = getLine( f );
    }
    fclose( f );
    printf( "\n" );
  }
}

/**
  Saves the shopping list to the output file.

  @param fileName A pointer to a string containing the name of the output file.
*/
static void saveOutput( char *fileName )
{
  FILE *f = fopen( fileName, "w" );
  if ( f == NULL ) {
    printf( "\nCan't open file" );
  } else {
    for ( int i = 0; i < list->length ; i++ ) {
      char *store = list->items[ i ]->store;
      double price = list->items[ i ]->price;
      char *name = list->items[ i ]->name;
      fprintf( f, "%s %.2f %s\n", store, price, name );
    }
    fclose( f );
    printf( "\n" );
  }
}

/**
  Tests whether the item is in the list, which will always be true.

  @param it A list item.
  @param arg An uneccessary argument.
  @return true If the item is in the list.
*/
static bool isInList( Item *it, void *arg )
{
  return true;
}

/**
  Tests whether the item will be bought at the given store.

  @param it A list item.
  @param arg The store name.
  @return true if the item will be bought at the given store.
*/
static bool isAtStore( Item *it, void *arg )
{
  char *storeName = ( char * )arg;
  if ( strcmp( it->store, storeName ) == 0 ) {
    return true;
  }
  return false;
}

/**
  Tests whether the item costs more than the given amount.

  @param A list item.
  @param A price with which to compare the cost of the item.
  @return true if the item costs more than the given amount.
*/
static bool isGreaterThan( Item *it, void *arg )
{
  double *cost = ( double * )arg;
  if ( it->price > *cost ) {
    return true;
  }
  return false;
}

/**
  Tests whether the item costs less than the given amount.

  @param it A list item.
  @param arg A price with which to compare the cost of the item.
  @return true if the item costs less than the given amount.
*/
static bool isLessThan( Item *it, void *arg )
{
  double *cost = ( double * )arg;
  if ( it->price < *cost ) {
    return true;
  }
  return false;
}

/**
  Tests whether the item any of the report specifications are true for the given item.

  @param it A list item.
  @param arg A pointer to an array of strings containing the various report specifications.
  @return true if the any of the report specifications are true for the item.
*/
static bool selectUnion ( Item *it, void *arg )
{
  char **reportCommands = ( char ** )arg;
  char *p = reportCommands[ 0 ];
  int count = 1;
  while ( p != NULL ) {
    char store[ STORE_MAX + 1 ];
    double price = 0;
    if ( sscanf( p, "store %s", store ) == 1 ) {
      if ( store[ strlen( store ) ] != '\0' ) {
        printf( "\nInvalid command" );
      } else {
        if ( strcmp( it->store, store ) == 0 ) {
          return true;
        }
      }
    } else if ( sscanf( p, "less %lf", &price ) == 1 ) {
      if ( it->price < price ) {
        return true;
      }
    } else if ( sscanf( p, "greater %lf", &price ) == 1 ) {
      if ( it->price > price ) {
        return true;
      }
    }
    p = reportCommands[ count ];
    count++;
  }
  return false;
}

/**
  Determines which report to print and feeds the appropriate function to
shoppingListReport.

  @param command The command containing the report specifications.
*/
static void selectReport ( char *command ) {
  bool ( *fp )( Item *it, void *arg );
  void *arg;
  int length = 0;
  int capacity = INIT_CAP;
  char **reportCommands = ( char ** )malloc( INIT_CAP * sizeof( char * ) );
  char *p = strstr( command, "store" );
  if ( p == NULL ) {
    p = strstr( command, "greater" );
  }
  if ( p == NULL ) {
    p = strstr( command, "less" );
  }
  while ( p != NULL ) {
    if ( length >= capacity ) {
      capacity *= INC_BY_FACTOR;
      reportCommands = ( char ** )realloc( reportCommands, capacity * sizeof( char * ) );
    }
    reportCommands[ length++ ] = p;
    char *q = strstr( p + 1, "store" );
    if ( q == NULL ) {
      q = strstr( p + 1, "greater" );
    }
    if ( q == NULL ) {
      q = strstr( p + 1, "less" );
    }
    p = q;
  }
  if ( length >= capacity ) {
      capacity++;
      reportCommands = ( char ** )realloc( reportCommands, capacity * sizeof( char * ) );
  }
  reportCommands[ length ] = NULL;
  if ( length > 1 ) {
    fp = selectUnion;
    arg = reportCommands;
  } else {
    char store[ STORE_MAX + 1 ];
    double price = 0;
    if ( sscanf( command, "report store %s", store ) == 1 ) {
      if ( store[ strlen( store ) ] != '\0' ) {
        printf( "\nInvalid command" );
      } else {
        fp = isAtStore;
        arg = store;
      }
    } else if ( sscanf( command, "report less %lf", &price ) == 1 ) {
      fp = isLessThan;
      arg =& price;
    } else if ( sscanf( command, "report greater %lf", &price ) == 1 ) {
      fp = isGreaterThan;
      arg = &price;
    } else if ( strcmp( command, "report" ) == 0 ){
      fp = isInList;
      arg = NULL;
    } else {
      printf( "\nInvalid command" );
      fp = NULL;
    }
  }
  if ( fp != NULL ) {
    shoppingListReport( list, fp, arg );
  }
  free( reportCommands );
}
/**
  Starts the program.
  Reads commands and directs them to the appropriate functions.

  @return The program exit status.
*/
int main( void )
{
  list = makeShoppingList();

  int count = 1;
  printf( "%d> ", count );
  char *command = getLine( stdin );
  char cmd[ CMD_LEN + 1 ];
  while ( command != NULL && strcmp( command, "quit" ) != 0 ) {
    count++;
    sscanf( command, "%s", cmd );
    if ( cmd[ strlen( cmd ) ] != '\0' ) {
      printf( "\nInvalid command" );
      printf( "\n%d> ", count );
    } else {
      char filename[ strlen( command ) - strlen( cmd ) + 1 ];
      char store[ STORE_MAX + 1 ];
      double price;
      char product[ strlen( command ) - strlen( cmd ) + 1 ];
      int id;
      if ( sscanf( command, "load %s", filename ) == 1 ) {
        loadInput( filename );
        printf( "%d> ", count );
      } else if ( sscanf( command, "save %s", filename ) == 1 ) {
        saveOutput( filename );
        printf( "%d> ", count );
      } else if ( sscanf( command, "add %s %lf %s", store, &price, product ) == ADD_ARG ) {
        if ( strlen( store ) > STORE_MAX ) {
          printf( "\nInvalid command" );
        }
        addItem( ( command + strlen( cmd ) ), POS_STDIN );
        printf( "\n%d> ", count );
      } else if ( sscanf( command, "remove %d", &id ) == 1 ) {
        bool removeSuccess = shoppingListRemove( list, id );
        if ( !removeSuccess ) {
          printf( "\nInvalid command" );
        }
        printf( "\n%d> ", count );
      } else if ( strcmp ( cmd, "report" ) == 0 ) {
          selectReport( command );
          printf( "\n%d> ", count );
      } else if ( strcmp( command, "help" ) == 0 ) {
        printf( "\nload <file>\nsave <file>\nadd <store> <price> <name>\nremove <id>\nreport\n" );
        printf( "report store <store>\nreport less <price>\nreport greater <price>\nhelp\nquit" );
        printf( "\n%d> ", count );
      } else {
        printf( "\nInvalid command" );
        printf( "\n%d> ", count );
      }
    }
    free( command );
    command = getLine( stdin );
  }
  if ( command != NULL ) {
    free( command );
    printf( "\n" );
  }
  freeShoppingList( list );
  return EXIT_SUCCESS;
}
