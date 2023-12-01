#include "libs/file.h"
#include "libs/window.h"

#include <stdio.h>
#include <curses.h>

unsigned int close = 0;

void parse_keys( struct Window* win, int ch )
{
        switch( ch )
        {
        case KEY_UP:
                win_moveCursor( win, 0, -1 );
                break;
        case KEY_DOWN:
                win_moveCursor( win, 0, 1 );
                break;
        case KEY_LEFT:
                win_moveCursor( win, -1, 0 );
                break;
        case KEY_RIGHT:
                win_moveCursor( win, 1, 0 );
                break;
        case KEY_F(10):
                close++;
                break;
        default:
                break;
        }
}

int main( void )
{
        struct File* file = file_init( "Makefile" );
        struct Window* win = win_init();
        win_setFile( win, file );

        #ifdef DEBUG
        win_setBarMsg( win, "Debug mode!" );
        #endif
        int ch = 0;
        while ( !close )
        {
                win_draw( win );
                ch = getch();
                
                parse_keys( win, ch );
        }

        win_free( win );

        return 0;
}
