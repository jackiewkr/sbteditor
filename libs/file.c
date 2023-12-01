/* file.c
 * ======
 * Representation of file on disk. Used for loading, saving and writing to
 * files.
 * Each line is stored as a linked list of characters to reduce complexity
 * of operations such as splitting a line or deleting the start of a line to
 * append the contents to the prev line.
 * 
 * By Jacqueline W.
 * See LICENSES for licensing
 */

#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define report( msg ) fprintf( stderr, msg )

static char* freadall( FILE* fptr )
{
        char* buf = "";
        fseek( fptr, 0, SEEK_END );

        long file_len = ftell( fptr );
        if ( file_len == -1 )
                return NULL;

        fseek( fptr, 0, SEEK_SET );
        buf = malloc( (unsigned long)file_len + 1 );
        if ( buf == NULL )
                return NULL;
        
        fread( buf, (unsigned long)file_len, 1, fptr );

        buf[ file_len ] = '\0';
        
        return buf;
}

//double linked list
struct LineChar
{
        struct LineChar* next;

        char data;
};

static struct LineChar* lc_init( struct LineChar* next, char data )
{
        struct LineChar* lc = malloc( sizeof(struct LineChar) );
        lc->next = next;
        lc->data = data;

        return lc;
}

static struct LineChar* lc_traverse( struct LineChar* lc )
{
        while ( lc->next != NULL )
                lc = lc->next;

        return lc;
}

static char* lc_toStr( struct LineChar* lc )
{
        char* buf = NULL;
        unsigned int sz = 0;

        while ( lc != NULL )
        {
                buf = realloc( buf, ++sz );
                buf[sz-1] = lc->data;
                
                lc = lc->next;
        }

        buf = realloc( buf, ++sz );
        buf[sz-1] = '\0';
        return buf;
}

struct File
{
        struct LineChar** lines;        //lines in file
        unsigned int sz;                //number of lines in file

        char* loc;                      //location on disk
};

//TODO: resize more efficiently, allow resize to be smaller
static void file_resize( struct File* file )
{
        file->lines = realloc( file->lines, 
                               sizeof(struct LineChar*) * ++file->sz );
        if ( file->lines == NULL )
                report( "Failed to realloc() file!\n" );

}


/* __file_populate() : STATIC
 * ==========================
 * Populates the file with the contents given by file->loc.
 */
static void file_populate( struct File* file )
{
        FILE* fptr = fopen( file->loc, "r" );
        char* file_buf = freadall( fptr );
        
        //iterate through each line in file
        char* line = strtok( file_buf, "\n" );
        do
        {
                file_resize( file );
                //add each char to file struct
                for ( int i = 0; i < strlen( line ); i++ )
                {
                        struct LineChar* head = file->lines[file->sz - 1];
                        if ( head != NULL )
                        {
                                //if line isnt empty, traverse to last node
                                head = lc_traverse( head );

                                head->next = lc_init( NULL, line[i] );

                        } 
                        else
                        {
                                file->lines[file->sz - 1] = lc_init( NULL, 
                                                            line[i] );
                        }
                }
        }
        while ( ( line = strtok( NULL, "\n" ) ) != NULL  );
}

struct File* file_init( const char* loc )
{
        struct File* file = malloc( sizeof(struct File) );
        if ( file == NULL)
                report( "File failed to malloc()!\n" );

        file->lines = NULL;
        file->sz = 0;

        //check if loc is given
        if ( loc != NULL )
        {
                //if file exists, copy content to file
                file->loc = (char*)loc;
                if ( access( loc, F_OK ) == 0 )
                        file_populate( file );
        }
        else
                file->loc = NULL;

        return file;
}

void file_free( struct File* file )
{

        for ( int i = 0; i < file->sz; i++ )
        {
                struct LineChar* lc = file->lines[i];
                struct LineChar* lc_next = lc->next;
                while ( lc_next != NULL )
                {
                        free( lc );
                        lc = lc_next;
                }
        }
        free( file );
}

char* file_getLine( struct File* file, unsigned int line_num )
{
        if ( file == NULL || line_num >= file->sz )
                return NULL;
        return lc_toStr( file->lines[line_num] );
}

unsigned int file_getLength( struct File* file )
{
        return file->sz;
}


