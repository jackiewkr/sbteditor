/**
 * main.c
 * ----------
 * Main file for Simple Basic Text Editor (SBTeditor)
 * A text editor written with the goal of being easily understood and modified.
 * By Jacqueline W. (jackiewalkergeneral@gmail.com)
 * ----------
 * See LICENSE.txt for licensing information
 */

#include <curses.h>
#include <string.h>

#include "file.h"
#include "screen.h"

/*
 * void help()
 * ----------
 * Print help information to the terminal. Invoked when an argument is
 * '-h' or '--help' or when an invalid argument is given.
 */
void help()
{
        printf( "SBTeditor - Simple Basic Text Editor\n----------\nA text "  );
        printf( "editor written with the goal of being easily understood "   );
        printf( "and modified.\n----------\n    ./sbtedit <filename>: "      );
        printf( "Opens the file called filename if it exists. Else, create " );
        printf( "a file called filename and opens it.\n" );
}

int main( int argc, char* argv[] )
{
        /* Parse args */
        if ( argc < 2 || argc > 2 )
        {
                help();
                return 0;
        }
        char* filename = argv[1];
        /* Open file */
        struct File* f = f_import( filename, strlen( filename ) );

        /* Initialize screen */
        struct Screen* s = s_construct();

        int ch;
        unsigned int close = 0;
        while ( !close )
        {
                ch = s_update( s, f );

                /* Input parsing */
                switch ( ch )
                {
                case KEY_UP:
                        s_moveCursor( s, f, -1, 0 );
                        break;
                case KEY_DOWN:
                        s_moveCursor( s, f, 1, 0 );
                        break;
                case KEY_LEFT:
                        s_moveCursor( s, f, 0, -1 );
                        break;
                case KEY_RIGHT:
                        s_moveCursor( s, f, 0, 1 );
                        break;
                case KEY_F(10):
                        close++;
                        break;
                case KEY_F(1):
                        //TODO: save
                        break;
                case 10:
                        s_writeChar( s, f, '\n' );
                        break;
                case KEY_BACKSPACE:
                        s_writeChar( s, f, 127 );
                        break;
                default:
                        s_writeChar( s, f, ch );
                        break;
                }
        }
        s_destruct( s );
        f_destruct( f );
        fprintf( stderr, "Closing...\n" );
        return 0;
}

