/**
 * file.c
 * ----------
 * Library that handles all file interactions. Stores an object that all file
 * interactions must go through.
 */

/*
TODO: change file data to be stored as a linked list??? maybe 
 */

#define FILENAME_LIM ( 255 ) /* Max length of filenames in most filesystems */
#define FILE_MIN_SZ  ( 512 ) /* Initial size allocated to f.data (in chars) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * unsigned int is_printable()
 * ----------
 * Given a char <c>, return 1 if the character is a valid printable character
 * or LF ('\n').
 * Else, return 0 if the character is a control code.
 */
static unsigned int is_printable( char c )
{
        if ( c >= 32 || c == 10 )
                return 1;
        return 0;
}

/**
 * struct File
 * ----------
 * Abstract of a file for use with the program. Stores data about the file
 * which can be modified with functions described below.
 * ----------
 * Initialize with init_file(), free with free_file()
 * After initialization, populate file with data with populate_file()
 * Modify file contents with insert_char() and overwrite_char().
 * save file contents to disk with export_file()
 */
struct File
{
        char* filename;
        char* data;
        unsigned int sz;
};

/*
 * struct File init_file()
 * ----------
 * Initializes and returns a File object with an allocated filename <filename>.
 */
struct File init_file( char* filename )
{
        struct File f;

        /* Allocate some memory to f.filename and f.data */
        if (  ( f.filename = (char*)malloc( FILENAME_LIM * sizeof(char) ) )
              == NULL  )
        {
                fprintf( stderr, "FATAL: Failed malloc() in init_file()!\n" );
        }

        if (  ( f.data = (char*)malloc( FILE_MIN_SZ * sizeof(char) ) )
              == NULL  )
        {
                fprintf( stderr, "FATAL: Failed malloc() in init_file()!\n" );
        }
        f.sz = FILE_MIN_SZ;

        /* Copy filename to object */
        strcpy( f.filename, filename );

        return f;
}

/*
 * void free_file()
 * ----------
 * Frees memory allocated to a given File object <f>.
 */
void free_file( struct File* f )
{
        free( f->data );
        free( f->filename );
        f->sz = 0;
}

/*
 * void resize_file()
 * ----------
 * Given a pointer to a File object <f>, this function doubles the size of the
 * memory allocated to f.data.
 */
void resize_file( struct File* f )
{
        f->sz >>= 1;
        if (  ( f->data = (char*)realloc( f->data, f->sz * sizeof(char) ) )
              == NULL  )
        {
                fprintf( stderr, "FATAL: Failed malloc() in resize_file()!\n" );
        }
}

/*
 * void populate_file()
 * ----------
 * Populates the data for a file given a <fptr> and a pointer to
 * a File object <f>. Copies the data character by character into f.data,
 * ignoring any non-printable characters (except for newline).
 */
void populate_file( struct File* f, char* filename )
{
        FILE* fptr = fopen( filename, "r" );

        char c;
        unsigned int index = 0;
        while ( !feof( fptr ) )
        {
                c = fgetc( fptr );
                if ( is_printable( c ) )
                {
                        f->data[index++] = c;
                        if ( index == f->sz )
                                resize_file( f );
                }
        }
}
