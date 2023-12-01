#include "libs/file.h"
#include "libs/window.h"

#include <stdio.h>
#include <curses.h>

int main( void )
{
        struct File* file = file_init( "Makefile" );
        struct Window* win = win_init();
        win_setFile( win, file );
        int ch = 0;
        while ( ch != (int)'q' )
        {
                win_draw( win );

                ch = getch();
        }

        win_free( win );

        return 0;
}
