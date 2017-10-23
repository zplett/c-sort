/*
Ezra Goss Zac Plett
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX_LINES 1024*1024


// Static function declarations
static void populate_array(char *lines[]);

int main(int argc, char *argv[]) {
  char **lines = malloc( MAX_LINES );
  populate_array(lines);
  /*  for(char* ptr = lines[0]; *ptr != '\0'; ++ptr){
    printf("%c", *ptr);
  }
  */
}

/** Populate the array with line input

 */
void populate_array(char *lines[]){
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
      // Insert the character
      *line = c;
      // Point to the next character slot
      ++line;
    }
  }
}
