OB/*
Ezra Goss Zac Plett
 */

#include <string.h>
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
static void print_lines( char *lines[] );
static void free_lines( char *lines[] );
static void process_flags ( int argc, char *argv[], char *lines[], int line_count );

int main( int argc, char *argv[] ) {

  // Allocates memory for the array
  char **lines = malloc( MAX_LINES * sizeof(char*) );;
  for ( int i = 0; i < MAX_LINES; ++i ) {
    lines[i] = malloc( LINE_LENGTH * sizeof(char) );
  }
  
  // Populate the arrays
  int line_count = populate_array(lines);

  // Processes flags
  process_flags( argc, argv, lines, line_count );
  
  // Prints the lines
  print_lines( lines );
  // Frees allocated memory
  free_lines( lines );

}

// Processes the flags input by the user
void process_flags( int argc, char *argv[], char *lines[], int line_count ) {
  // Flags to indicate which method of sorting to use
  int fold_flag = 0, num_flag = 0;

  // Checks if flag was entered, if not uses default sorting based on ASCII values
  if ( argc > 1) {

    if ( strcmp(argv[1], "-f") == 0 )
      fold_flag = 1;
    else if ( strcmp(argv[1], "-n") == 0 )
      num_flag = 1;
    else {
      printf( "Error, you entered an invalid flag.\n" );
      exit(-1);
    }

  }

  // Determines which sorting method to use based on the flags
  if ( fold_flag == 0 && num_flag == 0 )
    qsort( (void*)lines, line_count, sizeof(lines[0]), ascii_compar );
  else if ( fold_flag == 1 )
    qsort( (void*)lines, line_count, sizeof(lines[0]), alpha_compar );

  //else if ( num_flag == 1 )
    //TODO: Call qsort with num
    
}

// Prints the lines
void print_lines( char *lines[] ) {
  
  // Prints the sorted buffer
  for ( int i = 0; i < MAX_LINES; ++i ) {
    for ( char *ptr = lines[i]; *ptr != '\0'; ++ptr ) {
      printf ( "%c", *ptr);
    } printf( "\n" );
    if ( *lines[i + 1] == '\0' )
      break;
  }

}

// Frees the array's allocated memory 
void free_lines( char *lines[] ) {

  // Frees inner array
  for ( int i = 0; i < MAX_LINES; ++i ) {
    free( lines[i] );
  }
  // Frees outer array
  free( lines );

}

/* Populate the array with line input
 *
 */
int populate_array( char *lines[] ){
  // Line number
  unsigned int i = 0;
  // Pointer to the first character of line i
  char *line = lines[i];
  // Input loop
  for( int c = getchar(); c != EOF; c = getchar()) {
    // If new line, point to the next line in the array
    if( c == '\n' )
      line = lines[++i];
    else {
      *line = c;
      // Point to the next character slot
      ++line;
    }
  }
  return i + 1;
}

/** Compare the ascii values of two characters
 *
 */
int ascii_compar( const void *c1, const void *c2 ) {
  return strcmp( *(char**)c1, *(char**)c2 );
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
