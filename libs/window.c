#include "window.h"

#include <curses.h>
#include <stdlib.h>

struct Window
{
        //cursor pos
        unsigned int col;
        unsigned int row;

        //term dims
        unsigned int width;
        unsigned int height;

        struct File* file;

        //file window
        WINDOW* file_win;
        WINDOW* bar_win;

        char* bar_msg;
};

static void _update_dims( struct Window* win )
{
        getmaxyx( stdscr, win->height, win->width );

        if ( win->file_win != NULL || win->bar_win != NULL )
        {
                //update sub window dims
                wresize( win->file_win, win->row-2, win->col );
                wresize( win->bar_win, 2, win->col );
        }
}

static void nc_enter( void )
{
        initscr();
        noecho();
        keypad( stdscr, TRUE );
        cbreak();
}

static void nc_exit( void )
{
        echo();
        nocbreak();
        endwin();
}

struct Window* win_init( void )
{
        nc_enter();

        struct Window* win = malloc( sizeof(struct Window) );
        win->file_win = NULL;
        win->bar_win = NULL;

        win->file = NULL;

        win->bar_msg = NULL;

        _update_dims( win );

        win->file_win = newwin( win->height - 2, win->width,
                                0, 0 );
        win->bar_win = newwin( 2, win->width, 
                               win->height - 2, 0 );

        return win;
}

void win_free( struct Window* win )
{
        free( win->bar_msg );
        free( win );

        nc_exit();
}

void win_setFile( struct Window* win, struct File* f )
{
        win->file = f;
}

void win_setCursor( struct Window* win, unsigned int col, unsigned int row )
{
        win->row = row;
        win->col = col;
}

void win_setBarMsg( struct Window* win, char* msg )
{
        win->bar_msg = msg;
}

static void win_drawFile( struct Window* win )
{
        wclear( win->file_win );

        //TODO: fancy shit
        unsigned int i = 0;
        while ( i < file_getLength( win->file ) && i < win->height - 2 )
        {
                char* buf = file_getLine( win->file, i );
                wmove( win->file_win, i, 0 );
                wprintw( win->file_win, "%s", buf );
                i++;
        }
        wmove( win->file_win, win->row, win->col );

        wrefresh( win->file_win );
}

static void win_drawBar( struct Window* win )
{
        wclear( win->bar_win );
        wmove( win->bar_win, 0, 0 );

        whline( win->bar_win, ACS_HLINE, win->width );
        wmove( win->bar_win, 1, 0 );
        wprintw( win->bar_win, "%d:%d - %s", win->row, win->col, win->bar_msg );
        
        wrefresh( win->bar_win );
}

void win_draw( struct Window* win )
{
        win_drawFile( win );
        win_drawBar( win );
}
