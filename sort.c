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
char process_flags( int argc, char *argv[], int* reverse );
void sort( char* lines[], int line_count, int reverse, char flag );

// Static functions
static void test_functions();
static void quick_sort( char *lines[], int left, int right, int(*compar)(const void*, const void*), int r_flag );

int main( int argc, char *argv[] ) {

  #ifdef DEBUG
  test_functions();
  #endif
  
  // Allocates memory for the array
  char **lines = malloc( MAX_LINES * sizeof(char*) );
  
  int reverse = 1;
  
  // Processes flags and sorts accordingly
  char flag = process_flags( argc, argv, &reverse );
 
  // Populate the array
  int line_num = populate_array( lines );

  // Sort
  sort( lines, line_num, reverse, flag );
  
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

/** Swap two elements of an array
 */
static void swap( char *lines[], int index1, int index2 ) {

  // Temp string
  char *temp;

  #ifdef DEBUG
  // Print progress
  printf("Swapping entry %d, '%s' to %d, and entry %d, '%s' to %d.\n",
         index1, lines[index1], index2, index2, lines[index2], index1 );
  #endif
  
  // Swap the strings
  temp = lines[index1];
  lines[index1] = lines[index2];
  lines[index2] = temp;
  
} 

// Helper function for quick sort
static void quick_sort( char *lines[], int left, int right, int(*compar)(const void*, const void*), int r ) {
  
  int pivot, i, j;
  const char * key;

  // Catch the simple case
  if( right - left == 1 ) {
    if( r*compar( lines[left], lines[right]) > 0 ) {
      #ifdef DEBUG
      printf("Only one entry: ");
      #endif
      swap( lines, left, right );
    }
    return;
  }
  
  // Establishes the pivot and marker values
  pivot = ( left + right ) / 2;
  key = lines[pivot];
  #ifdef DEBUG
  printf("Sorting from %d to %d: pivot is at %d, '%s'\n",
         left, right, pivot, key);
  #endif
  swap( lines, left, pivot );
  
  i = left + 1;
  j = right;

  // Increases the marker values until the leftmost marker surpasses the rightmost marker
  // Each time these values are incremented, their corresponding values in the array are swaped
  while( i < j ) {  
    while(  i <= right && r*compar( lines[i], key ) < 0 ) 
      ++i;
    while( j >= left && r*compar( lines[j], key ) > 0 )
      --j;
    
    if( i < j ) {
      #ifdef DEBUG
      printf("Out of order: '%s' > '%s', but %d < %d: ",
             lines[i], lines[j], i, j);
      #endif
      swap( lines, i, j );
    }
  }

  #ifdef DEBUG
  printf("Putting the pivot back: %d", j - 1 );
  #endif
  
  swap( lines, left, j );

  if( left < j - 1 ) {
    #ifdef DEBUG
    printf("Sub-sorting lower entries\n");
    #endif
    // Sort the left half
    quick_sort( lines, left, j - 1, compar, r );
  }

  if( j + 1 < right ) {
    #ifdef DEBUG
    printf("Sub-sorting upper entries\n");
    #endif
    quick_sort( lines, j + 1, right, compar, r );
  }
  
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
char process_flags( int argc, char *argv[], int *reverse ) {
  // Flags to indicate which method of sorting to use
  char flag = 0;

  int c;
  while( (c = getopt( argc, argv, "rnf")) != EOF ) {
    switch( c ) {
    case 'f': case 'n':
      flag = c;
      break;
    case 'r':
      *reverse = -1;
      break;
    default:
      printf("You have entered an invalid flag: %c\n", c );
      break;
    }
  }
  return flag;
}

void sort( char* lines[], int line_count, int reverse, char flag ) {

  // Handle the scenario where there is no input
  if( line_count == 0 )
    return;
  
  // Determines which sorting method to use based on the flags
  if ( flag == 'f' )
    quick_sort( lines, 0, line_count - 1, alpha_compar, reverse );      
  else if ( flag == 'n' )
    quick_sort( lines, 0, line_count - 1, numeric_compar, reverse );
  else
    quick_sort( lines, 0, line_count - 1, ascii_compar, reverse );
  
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
  return strcmp( (char*)c1, (char*)c2 );
}

/** Compare the alphabetical value of two characters (case insensitive)
 *
 */
int alpha_compar( const void *c1, const void *c2 ) {
  // Allocate to the temporary lower case string copies
  char *l1 = malloc( strlen((char*)c1) + 1 ),
    *l2 = malloc( strlen((char*)c2) + 1 ),
    *start1 = l1,
    *start2 = l2;
  
  // Copy the case sensitive strings
  l1 = strcpy( l1, (char*)c1 );
  l2 = strcpy( l2, (char*)c2 );

  // Make each character in the first string lower case
  for( int i = 0; i < (int)strlen((char*)c1); ++i ){
    l1[i] = tolower( l1[i] );
  }

  // Make each character in the second string lower case
  for( int i = 0; i < (int)strlen((char*)c2); ++i ){
    l2[i] = tolower( l2[i] );
  }

  // Make the pointers start at the first character again
  l1 = start1;
  l2 = start2;
  
  // Run ascii_compar on altered character pointers
  int comp =  ascii_compar( l1, l2 );

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
  long num1 = mystrtol( (char*)c1, &rest1 );
  long num2 = mystrtol( (char*)c2, &rest2 );
  // Comparisons
  if( num1 < num2 )
    return -1;
  else if( num1 > num2 )
    return 1;
  else
    return ascii_compar( rest1, rest2 );
}
