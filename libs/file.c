/** file.c
 ** ======
 ** This file stores the objects used for storing the file during use, along
 ** with loading and saving to/from disk.
 **
 ** Jacqueline W. (2023)
 **/
#include <stdlib.h>
#include <string.h>

#include "file.h"

/* strsan()
 * =================
 * Given a character buffer, sanitizes the string by removing any non-readable
 * ASCII characters and appending a null terminator.
 *
 * - input: input char buffer
 * - sz: size of input
 *
 * Returns a string literal
 */
static char* strsan( char* input, unsigned int sz )
{
        if ( input == NULL )
                return NULL;
        unsigned int output_sz = 0;
        char* output = NULL;

        for ( unsigned int i = 0; i < sz; i++ )
        {
                char ch = input[i];

                /* only copy char over if it is human readable */
                if ( ch >= 32 && ch < 127 )
                {
                        output = realloc( output, ++output_sz );
                        output[ output_sz - 1 ] = ch;
                }
        }

        /* add nullchar at end of buffer */
        output_sz++;
        output = realloc( output, output_sz );
        output[ output_sz - 1 ] = '\0';

        return output;
}

struct Line
{
        int linenum; /* copy of linenum in file */
        
        char* line; /* char buffer of line contents ( no control characters) */
        unsigned int size;
};

static struct Line* l_init( void )
{
        struct Line* l = malloc( sizeof(struct Line) );
        if ( l == NULL )
                return NULL;

        l->line = NULL;
        l->size = 0;

        return l;
}

static void l_free( struct Line* l )
{
        if ( l == NULL )
                return;
        
        if ( l->line != NULL )
                free( l->line );

        free( l );
}

/* l_addChar()
 * ===========
 * Adds a char to the given index in the line. If not at the end of the line,
 * move the contents of the line one index ahead.
 *
 * Returns 1 on success, 0 on fail
 */
static unsigned int l_addChar( struct Line* l, char ch, unsigned int index )
{
        if ( ch >= 32 && ch < 127 && l != NULL &&
             index < l->size+1 && index > 0 )
        {
                l->line = realloc( l->line, l->size+1 );
        
                if ( index != l->size )
                {
                        memmove( &l->line[index+1], &l->line[index],
                                 l->size + 1 - index );
                }

                l->line[index] = ch;
                l->size++;
                
                return 1;
        }
        return 0;
}

/* l_getContents()
 * ===============
 * Copies the contents of the line to a newly allocated char buffer and
 * converts char buffer to string literal. Free buffer after use
 *
 * Returns a string literal
 */
char* l_getContents( struct Line* l )
{
        char* string = strsan( l->line, l->size );

        return string;
}


