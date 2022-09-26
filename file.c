/**
 * file.c
 * ----------
 * Library that handles all file interactions. Stores an object that all file
 * interactions must go through.
 */

#include "file.h"

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

/** File object methods below */

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
        f.sz_alloc = FILE_MIN_SZ;
        f.sz_actual = 0;

        /* Copy filename to object */
        strcpy( f.filename, filename );

        return f;
}

/*
 * void free_file()
 * ----------
 * Frees memory allocated to a given File object <f> and set all other values
 * to 0.
 */
void free_file( struct File* f )
{
        free( f->data );
        free( f->filename );
        f->sz_alloc = 0;
        f->sz_actual = 0;
}

/*
 * void resize_file()
 * ----------
 * Given a pointer to a File object <f>, this function doubles the size of the
 * memory allocated to f.data.
 */
void resize_file( struct File* f )
{
        f->sz_alloc >>= 1;
        if (  ( f->data = realloc( f->data, f->sz_alloc * sizeof(char) ) )
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
void populate_file( struct File* f )
{
        FILE* fptr = fopen( f->filename, "w+" );

        char c;
        while ( !feof( fptr ) )
        {
                c = fgetc( fptr );
                if ( is_printable( c ) )
                {
                        f->data[f->sz_actual++] = c;
                        if ( f->sz_actual == f->sz_alloc )
                                resize_file( f );
                }
        }

        fclose( fptr );
}

/*
 * void export_file()
 * ----------
 * For a given pointer to a File object <f>, write f.data to a file called
 * f.filename.
 */
void export_file( struct File* f )
{
        FILE* fptr = fopen( f->filename, "w" );

        for ( unsigned int i = 0; i < f->sz_actual; i++ )
        {
                fputc( f->data[i], fptr );
        }
        fclose( fptr );
}

/*
 * void overwrite_char()
 * ----------
 * Set the character at a given index <index> in the File <f> to the character
 * <ch>. Counterpart to insert_char().
 */
void overwrite_char( struct File* f, unsigned int index, char ch )
{
        if ( index == f->sz_actual )
        {
                f->sz_actual++;
                if ( f->sz_actual >= f->sz_alloc )
                        resize_file( f );
                
                f->data[index] = ch;
        }
        else if ( index > f->sz_actual )
        {} //do nothing, too far oob
        else
        {
                f->data[index] = ch;
        }
}

/*
 * void insert_char()
 * ----------
 * 
 */
void insert_char( struct File* f, unsigned int index, char ch )
{
        //TODO: this function
}
