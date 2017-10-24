/*
Ezra Goss Zac Plett
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_LINES 1024*1024
#define LINE_LENGTH 1024

// Static function declarations
static int populate_array( char *lines[] );
static int ascii_compar( const void*, const void* );
static int alpha_compar( const void*, const void* );
static int numeric_compar( const char*, const char* );

int main( int argc, char *argv[] ) {
  // Allocate for outer array
  char **lines = malloc( MAX_LINES * sizeof(char*) );
  // Allocate for inner array
  for( int i = 0; i < MAX_LINES; ++i ) {
    lines[i] = malloc( LINE_LENGTH * sizeof(char) );
  }
  // Populate the arrays
  int line_count = populate_array(lines);
  // Sort the lines
  // TODO: not calling comparator
  qsort( (void*)lines, line_count, sizeof(lines[0]), ascii_compar );
  
  for( int i = 0; i < MAX_LINES; ++i) {
    for( char* ptr = lines[i] ; *ptr != '\0'; ++ptr ){
      printf( "%c", *ptr );
    }
    if( *lines[i + 1] == '\0' )
      break;
  }
   
  // Free inner arrays
  for( int i = 0; i < MAX_LINES; ++i ) {
    free( lines[i] );
  }
  // Free outer array
  free( lines );
}
  
/** Populate the array with line input

 */
int populate_array( char *lines[] ){
  // Line number
  unsigned int i = 0;
  // Pointer to the first character of line i
  char *line = lines[i];
  // Input loop
  for( int c = getchar(); c != EOF; c = getchar()) {
    // Initialize character no matter what
    *line = c;
    // If new line, point to the next line in the array
    if( c == '\n' )
      line = lines[++i];
    else 
      // Point to the next character slot
      ++line;
  }
  return i;
}

/** Compare the ascii values of two characters
 *
 */
int ascii_compar( const void *c1, const void *c2 ) {
  // If the ascii value is greater
  if( **(char**)c1 > **(char**)c2 ) return 1;
  // If the ascii value is less than
  else if( **(char**)c1 < **(char**)c2 ) return -1;

  // If the ascii values are equal
  return 0;
}

/** Compare the alphabetical value of two characters (case insensitive)
 *
 */
int alpha_compar( const void *c1, const void *c2 ) {
  // Dummy variables
  char l1, l2, *p1 = &l1, *p2 = &l2;
  // Make the comparison case insensitive with new variables
  if( isalpha(**(char**)c1) && (**(char**)c2) ) {
    l1 = tolower( **(char**)c1 );
    l2 = tolower( **(char**)c2 );
  } else {
    l1 = **(char**)c1;
    l2 = **(char**)c2;
  }
  
  // Run ascii_compar on altered characters
  return ascii_compar( &p1, &p2 );
}
