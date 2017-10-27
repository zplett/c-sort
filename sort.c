/*
Ezra Goss Zac Plett
 */

#include "sort.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <assert.h>
//#define DEBUG

// Static function declarations
long mystrtol( char *start, char **rest);
int populate_array( char *lines[] );
int ascii_compar( const void*, const void* );
int alpha_compar( const void*, const void* );
int numeric_compar( const void*, const void* );
void print_lines( char *lines[], int line_num );
void free_lines( char *lines[], int line_num );
void process_flags( int argc, char *argv[], char *lines[], int line_count );

// Static functions
static void test_functions();
static void quick_sort( char *lines[], int left, int right, int(*compar)(const void*, const void*), int r_flag );
static int quick_sort_helper( char *lines[], int left, int right, int(*compar)(const void*, const void*) );

int main( int argc, char *argv[] ) {

  #ifdef DEBUG
  test_functions();
  #endif
  
  // Allocates memory for the array
  char **lines = malloc( MAX_LINES * sizeof(char*) );
  
  // Processes flags and sorts accordingly
  int line_num = populate_array( lines );
  process_flags( argc, argv, lines, line_num );
  
  // Prints the lines
  print_lines( lines, line_num );
  // Frees allocated memory
  free_lines( lines, line_num );

}

// Test function used to validate mystrtol
static void test_functions() {

  // Sets up an array with the same input as a line of -n input
  char* line1 = " 5289328 Cards in a standard deck";
  char* line2 = " 58 Cards in a standard deck";
  char* line3 = " 5289328 in a standard deck";
  long value;
  char *rest;

  // Calls mystrol
  value = mystrtol( line1, &rest );

  // Prints the returned long to verify that it gives the correct number
  printf("%ld%s\n", value, rest);
  
  assert( numeric_compar( &line1, &line2 ) > 0 );
  assert( numeric_compar( &line1, &line3 ) > 0 );
  printf("All test cases passed!\n");
}

// Our implementation of quick sort
static void quick_sort( char *lines[], int left, int right,  int(*compar)(const void*, const void*), int r_flag ) {

  // Checks whether the array has enough elements to sort
  if ( right > 1 ) {
    // Sets the pivot and then calls quick_sort() recursively using this value
    int pivot = quick_sort_helper( lines, left, right, compar );
    quick_sort( lines, left,  pivot - 1, compar, r_flag );
    quick_sort( lines, pivot + 1, right, compar, r_flag );
  }

}

// Helper function for quick sort
static int quick_sort_helper( char *lines[], int left, int right, int(*compar)(const void*, const void*) ) {

  char *pivot, *temp;
  int i, j;

  // Establishes the pivot and marker values
  pivot = lines[left];
  i = left;
  j = right + 1;

  // Increases the marker values until the leftmost marker surpasses the rightmost marker
  // Each time these values are incremented, their corresponding values in the array are swaped
  for(;;) {
    while( compar( &lines[i], &pivot ) <= 0 && i <= right )
      ++i;
    while( compar( &lines[j], &pivot ) > 0 )
      ++j;
    if( i >= j )
      break;
    temp = lines[i];
    lines[i] = lines[j];
    lines[j] = temp;
  }

  // Since the for loop will break before the final swap, this takes care of the last swap needed
  temp = lines[i];
  lines[i] = lines[j];
  lines[j] = temp;

  // Returns the value used for the pivot
  return j;

}

// Converts String representation of a number to a long
long mystrtol( char *start, char **rest) {

  char *pos = start;
  long runsum = 0;
  long sign = 1;
  
  // Skips leading whitespace
  while( isspace( *pos ) ) {
    ++pos;
    continue;
  }
  // Determines whether or not the number is negative
  if( *pos == '-' ) {
    sign = -1;
    ++pos;
  }
  // Checks if the characters after leading whitespace are digits
  if( isdigit( *pos ) != 0 ) {
    // Loops over characters that are digits and combines their values to get the long int representation
    while( isdigit( *pos ) != 0 ) {
      long thisnum = *pos - '0';
      runsum *= 10;
      runsum += thisnum;
      ++pos;
    }
    // Reasses value based on sign variable
    if( sign == -1 )
    runsum *= -1;
  }
  // Returns 0 when there aren't digits after leading whitespace
  else {
    *rest = start;
    return 0;
  }
  // Sets rest to the correct position (either after the digits or at the start of the line) depending on the value of the input *rest
    if ( *pos != '\0' )
      *rest = pos;
    else
      *rest = start;
  
  return runsum;
  
}

// Processes the flags input by the user
void process_flags( int argc, char *argv[], char *lines[], int line_count ) {
  // Flags to indicate which method of sorting to use
  int fold_flag = 0, num_flag = 0;
  
  // Checks if flag was entered, if not uses default sorting based on ASCII values
  if ( argc > 1) {
    
    if ( strcmp(argv[1], "-f") == 0 ) {
      fold_flag = 1;
    }
    else if ( strcmp(argv[1], "-n") == 0 )
      num_flag = 1;
    else {
      printf( "Error, you entered an invalid flag.\n" );
      exit(-1);
    }
    
  }

  // Determines which sorting method to use based on the flags
  if ( fold_flag == 0 && num_flag == 0 )
    qsort( lines, line_count, sizeof(lines[0]), ascii_compar );
  else if ( fold_flag == 1 )
    qsort( lines, line_count, sizeof(lines[0]), alpha_compar );      
  else if ( num_flag == 1 )
    qsort( lines, line_count, sizeof(lines[0]), numeric_compar );
    
}

// Prints the lines
void print_lines( char *lines[], int line_num ) {
  
  // Prints the sorted buffer
  for ( int i = 0; i < line_num; ++i ) {
    printf("%s\n", lines[i]);
  }

}

// Frees the array's allocated memory 
void free_lines( char *lines[], int line_num ) {
  
  // Frees inner array
  for( int i = 0; i < line_num; ++i ) {
    free( lines[i] );
  }
  
  // Frees outer array
  free( lines );

}

/* Populate the array with line input
 *
 */
int populate_array( char *lines[] ){

  // Metrics
  int num_lines = 0;
  int word_length = 0;
  
  // Temporary buffer for each word
  char *temp = malloc( LINE_LENGTH );

  // Input loop
  for( int c = getchar(); c != EOF; c = getchar() ) {

    /** If there's a new line, allocate a new array
     *  with the size as the length of the word and
     *  copy the word
     */
    if( c == '\n' ){
      // Add null character
      temp[word_length++] = '\0';
      // Allocate new word length space
      lines[num_lines] = malloc( word_length );
      //Copy the word
      strcpy( lines[num_lines], temp );
      // Free the word
      free( temp );
      // Reset the word_length
      word_length = 0;
      // Allocate new memory to the recently freed temp word
      temp = malloc( LINE_LENGTH );
      // Increment line counter
      ++num_lines;
    }
    else {
      temp[word_length++] = c;
    }
    
  }
  // Final free for temporary variable
  free( temp );

  // Return the number of lines
  return num_lines;

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
  // Allocate to the temporary lower case string copies
  char *l1 = malloc( strlen(*(char**)c1) + 1 ),
    *l2 = malloc( strlen(*(char**)c2) + 1 ),
    *start1 = l1,
    *start2 = l2;
  
  // Copy the case sensitive strings
  l1 = strcpy( l1, *(char**)c1 );
  l2 = strcpy( l2, *(char**)c2 );

  // Make each character in the first string lower case
  for( int i = 0; i < (int)strlen(*(char**)c1); ++i ){
    l1[i] = tolower( l1[i] );
  }

  // Make each character in the second string lower case
  for( int i = 0; i < (int)strlen(*(char**)c2); ++i ){
    l2[i] = tolower( l2[i] );
  }

  // Make the pointers start at the first character again
  l1 = start1;
  l2 = start2;
  
  // Run ascii_compar on altered character pointers
  int comp =  ascii_compar( &l1, &l2 );

  // Free
  free(l1);
  free(l2);
  
  // Return
  return comp;
}

/** Numeric Comparator
 *
 */
int numeric_compar( const void *c1, const void *c2 ) {
  // The rest of the word after the first number
  char *rest1, *rest2;
  
  // Number conversions of the string
  long num1 = mystrtol( *(char**)c1, &rest1 );
  long num2 = mystrtol( *(char**)c2, &rest2 );
  // Comparisons
  if( num1 < num2 )
    return -1;
  else if( num1 > num2 )
    return 1;
  else
    return ascii_compar( &rest1, &rest2 );
}
