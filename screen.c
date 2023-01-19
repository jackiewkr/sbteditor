/**
 * screen.c
 * ========
 * Abstraction of the screen and parts of the editor logic.
 *
 * Licensed under the GNU GPL v3 license. See LICENSE for more details.
 * ----------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <ncurses.h>

#include "file.h"

static int is_printable( char ch )
{
        if ( ch < 32 || ch > 126 )
                return 0;
        return 1;
}

/*
 * clamp()
 * =======
 * Simple function to clamp an integer between two bounds (inclusive).
 */
static int clamp( int val, int min,int max )
{
        if ( val < min )
                return min;
        if ( val > max )
                return max;
        return val;
}

static void enter_ncurses()
{
        initscr();
        noecho();
        keypad( stdscr, TRUE );
        cbreak();        
}

static void exit_ncurses()
{
        echo();
        nocbreak();
        endwin();
}

struct Screen
{
        int w_rows;
        int w_cols;
        int c_x;
        int c_y;
        int offset;
        int mode; //0 for insert, 1 for overwrite
};

struct Screen* s_construct()
{
        struct Screen* s = malloc( sizeof(struct Screen) );
        s->c_x = 0;
        s->c_y = 0;
        s->offset = 0;
        
        /* Enter ncurses magic */
        enter_ncurses();
        
        /* Get initial window dims */
        
        return s;
}

void s_destruct( struct Screen* s )
{
        /* Exit ncurses magic */
        exit_ncurses();

        free( s );
}

static void draw_file( struct Screen* s, struct File* f )
{
        move( 0, 0 );
        int y = 0;
        for ( int i = s->offset;
              i < f_getMaxLines( f ) && i < ( s->w_rows + s->offset - 1 );
              i++ )
        {
                printw( "%3i %s", i, f_getLine( f, i ) );
                move( ++y, 0 );
        }

        move( s->w_rows, 0 );
        printw( "CURSOR: (%i,%i)", s->c_x, s->c_y + s->offset );
        
}

int s_update( struct Screen* s, struct File* f )
{
        getmaxyx( stdscr, s->w_rows, s->w_cols );

        erase();
        draw_file( s, f );

        move( s->c_y, s->c_x + 4 );
        
        refresh();
        return getch();
}

void s_moveCursor( struct Screen* s, struct File* f, int y, int x )
{
        /* move cursor */
        if( y > 0 )
        {
                s->c_y++;
                if ( s->c_y >= s->w_rows )
                {
                        s->offset++;
                        s->c_y--;
                }
        } else if ( y < 0 )
        {
                s->c_y--;
                if ( s->c_y < 0 )
                {
                        s->offset--;
                        s->c_y++;
                }
        }

        if ( x > 0 )
        {
                s->c_x++;
        } else if ( x < 0 && s->c_x != 0 )
        {
                s->c_x--;
        }

        /* fix offset and x-pos */
        
        s->offset = clamp( s->offset, 0, f_getMaxLines( f ) - 1 );
        s->c_x = clamp( s->c_x, 0, f_getLineLen( f, s->offset + s->c_y ) );

}

void s_writeChar( struct Screen* s, struct File* f, int ch )
{
        if ( is_printable( ch ) )
        {
                f_insertChar( f, s->offset + s->c_y, s->c_x, ch);
                s_moveCursor( s, f, 0, 1 );
        } else if ( ch == 10 )
        {
                f_insertChar( f, s->offset + s->c_y, s->c_x, ch );
                s->c_x = 0;
                s_moveCursor( s, f, 1, 0 );
        } else if ( ch == 127 )
        {
                f_insertChar( f, s->offset + s->c_y, s->c_x, ch );
                if ( s->c_x == 0 )
                {
                        s_moveCursor( s, f, -1, 0 );
                        s->c_x = f_getLineLen( f, s->offset + s->c_y ) - 1;
                } else
                {
                        s_moveCursor( s, f, 0, -1 );
                }
        }
}
