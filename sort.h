#define MAX_LINES 10

#define LINE_LENGTH 1024

#ifndef POPULATE_ARRAY
#define POPULATE_ARRAY
int populate_array( char *lines[] );
#endif

#ifndef ASCII_COMPAR
#define ASCII_COMPAR
int ascii_compar( const void*, const void* );
#endif

#ifndef ALPHA_COMPAR
#define ALPHA_COMPAR
int alpha_compar( const void*, const void* );
#endif

#ifndef NUMERIC_COMPAR
#define NUMERIC_COMPAR
int numeric_compar( const char*, const char* );
#endif

#ifndef PRINT_LINES
#define PRINT_LINES
void print_lines( char *lines[], int line_num );
#endif

#ifndef FREE_LINES
#define FREE_LINES
void free_lines( char *lines[], int line_num );
#endif

#ifndef PROCESS_FLAGS
#define PROCESS_FLAGS
void process_flags( int argc, char *argv[], char *lines[], int line_count );
#endif
