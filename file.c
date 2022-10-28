/**
 * file.c
 * ======
 * Abstract Data Type of a file used within the editor. Split into 'lines' for
 * ease of use. Also handles file I/O.
 *
 * Licensed under GNU GPL v3 license. See LICENSE for more details. 
 * ----------------------------------------------------------------------------
 */

#include "file.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Max size of a filename on most filesystems */
#define FILENAME_BUF_SZ (255)

static int is_printable( char ch )
{
        if ( ch < 32 || ch > 126 )
                return 0;
        return 1;
}

static char* sanitize_string( char* dirty_buf, int dirty_sz )
{
        char* clean_buf = malloc( sizeof(char) * dirty_sz );
        int clean_idx = 0;
        for ( int i = 0; i < dirty_sz; i++ )
        {
                if ( is_printable( dirty_buf[i] ) )
                        clean_buf[clean_idx++] = dirty_buf[i];
        }

        return clean_buf;
}

enum HL_Type
{
        HL_NONE,
        HL_NUMBER,
        HL_DATATYPE,
        HL_KEYWORD,
        HL_FUNC,
        HL_VARIABLE,
        HL_STRING,
        HL_COMMENT
};

/**
 * struct String
 * =============
 * Dynamic size string that allows appending to a char buffer in memory. 
 */
struct String
{
        char* buf;
        int sz;
};

/*
 * str_construct()
 * ===============
 * Returns a pointer to a String object with a NULL ptr and of size zero.
 */
struct String* str_construct()
{
        struct String* str = malloc( sizeof(struct String) );

        str->buf = NULL;
        str->sz = 0;

        return str;
}

static void str_insert( struct String* str, char* buf, int buf_sz, int index )
{
        if ( index > str->sz )
                return;

        /* resize string  */
        str->buf = realloc( str->buf, sizeof(char) * (str->sz + buf_sz) );

        /* copy contents from index - (str->sz-1) to end of string  */
        for( int i = str->sz + buf_sz - 1; i > index -1; i-- )
        {
                memmove( &str->buf[i], &str->buf[i - buf_sz], sizeof(char) );
        }
        /* copy contens of buf to empty space in string */
        memcpy( &str->buf[index], buf, buf_sz );
        str->sz += buf_sz;
}

/*
 * str_del()
 * =========
 * Delete characters from s_index onwards and resize string to fit.
 */
static void str_del( struct String* str, int s_index, int e_index )
{
        /* If not at the end of the buffer, move memory after e_index up */
        if ( str->sz != e_index )
        {
                memmove( &str->buf[s_index], &str->buf[e_index],
                         sizeof(char) * (str->sz-(s_index-e_index)) );
        }

        /* resize buffer and sz */
        str->buf = realloc( str->buf,
                            sizeof(char) * (str->sz - (e_index - s_index)) );
        str->sz -= (e_index - s_index);
}

/*
 * str_destruct()
 * ==============
 * Frees the memory associated with a String object and frees the object
 * itself.
 */
static void str_destruct( struct String* str )
{
        str->sz = 0;
        free( str->buf );
        free( str );
}


struct Line
{
        int index;                  /* Index of line */
        struct String* data;        /* Characters stored in the line */
        int* hl_data;               /* Highlighting data */
};

static struct Line* l_construct( int index )
{
        struct Line* l = malloc( sizeof(struct String*) );

        l->index = index;
        l->data = str_construct();

        return l;
}

static void l_destruct( struct Line* l )
{
        if ( l == NULL )
                return;

        str_destruct( l->data );
        free( l );
}

struct File
{
        int sz;
        struct Line** lines;
        char* filename;
};

static struct File* f_construct( char* buf, unsigned int sz )
{
        struct File* f = malloc( sizeof(struct File) );
        f->sz = 0;
        f->lines = NULL;
        f->filename = malloc( sizeof(char) * FILENAME_BUF_SZ );
        memcpy( &f->filename[0], buf, sz );
        
        return f;
}

void f_destruct( struct File* f )
{
        if ( f == NULL )
                return;
        for ( unsigned int i = 0; i < f->sz; i++ )
        {
                l_destruct( f->lines[i] );
        }

        free( f->filename );
        free( f );
}

static void f_insertLine( struct File* f, int index )
{
        if ( index > f->sz )
                return;
        f->lines = realloc( f->lines, sizeof(struct Line*) * (f->sz + 1) );
        f->sz++;
        if ( index < f->sz - 1 )
        {
                for ( int i = f->sz -1; i > index - 1; i-- )
                {
                        memmove( &f->lines[i], &f->lines[i-1],
                                 sizeof(struct Line*) );
                        f->lines[i]->index++;
                }
        }

        f->lines[index] = l_construct( index );
}

static void f_removeLine( struct File* f, int index )
{
        if ( index > f->sz )
                return;
        if ( index < f->sz )
        {
                for ( int i = index ; i < f->sz -1; i++ )
                {
                        memmove( &f->lines[i], &f->lines[i+1],
                                 sizeof(struct Line*) );
                        f->lines[i]->index--;
                }
        }
        l_destruct( f->lines[--f->sz] );
        f->lines = realloc( f->lines, sizeof(struct Line*) * f->sz );
}

char* f_getLine( struct File* f, int index )
{
        if (index >= f->sz || index < 0 )
                return ""; /* return blank string if invalid index */
        return sanitize_string( f->lines[index]->data->buf,
                                f->lines[index]->data->sz );
}

int f_getLineLen( struct File* f, int index )
{
        return f->lines[index]->data->sz;
}

int f_getMaxLines( struct File * f )
{
        return f->sz;
}

/** FILE I/O  **/
struct File* f_import( char* filename, int filename_sz )
{
        struct File* f = f_construct( filename, filename_sz );

        FILE* fptr;
        if (  ( fptr = fopen( filename, "r" ) )  == NULL )
        {
                f_insertLine( f, 0 );
                return f;
        }
        int nread;
        char* buf = malloc( sizeof(char) * 255 );
        size_t buf_sz = 255;
        int index = 0;
        while (  ( nread = getline( &buf, &buf_sz, fptr ) ) != -1  ) {
                f_insertLine( f, index );
                str_insert( f->lines[index]->data, buf, nread, 0 );

                index++;
        }
        free( buf );
        return f;
}

void f_export( struct File* f );

void f_insertChar( struct File* f, int index, int l_index, int ch )
{
        char* buf;
        int sz;
        struct Line* curr_line = f->lines[index];
        switch ( ch )
        {
        case '\n':
                /* create new line with contents from l_index onwards */
                sz = curr_line->data->sz - l_index;
                if ( sz == 0 )
                {
                        f_insertLine( f, index + 1 );
                }
                else
                {
                        buf = malloc( sizeof(char) * sz );
                        memcpy( buf, &curr_line->data->buf[l_index], sz );
                        str_del( curr_line->data, l_index, curr_line->data->sz );
                        f_insertLine( f, index + 1 );

                        str_insert( f->lines[index+1]->data, buf, sz, 0 );
                }
                break;
        case 127:
                if ( l_index == 0 )
                {
                        if ( curr_line->data->sz > 0 )
                        {
                                buf = malloc( sizeof(char) *
                                              curr_line->data->sz );
                                memcpy( buf, curr_line->data->buf,
                                        curr_line->data->sz );
                                str_insert( f->lines[index-1]->data, buf,
                                            curr_line->data->sz,
                                            f->lines[index-1]->data->sz-1 );
                        }
                        f_removeLine( f, index );
                } else
                {
                        str_del( curr_line->data, l_index, l_index+1 );
                }
                break;
        default:
                buf = malloc( sizeof(char) );
                buf[0] = ch;
                str_insert( curr_line->data, buf, 1, l_index );
                break;
        }
}
