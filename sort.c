/*
Ezra Goss Zac Plett
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX_LINES 1024*1024
#define LINE_LENGTH 1024

// Static function declarations
static void populate_array(char *lines[]);

int main( int argc, char *argv[] ) {
  char **lines = malloc( MAX_LINES * sizeof(char*) );
  for( int i = 0; i < MAX_LINES; ++i ){
    lines[i] = malloc( LINE_LENGTH * sizeof(char) + 1 );
  }
  populate_array(lines);
  for( int i = 0; i < MAX_LINES; ++i) {
    for( char* ptr = lines[i] ; *ptr == '\0' ; ++ptr ){
      printf("%c", *ptr);
    }
  }
    
  for(int i = 0; i < MAX_LINES; ++i) {
    free(lines[i]);
  }
  free(lines);
}
  
/** Populate the array with line input

 */
void populate_array( char *lines[] ){
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
}
