/**
 * main.c
 * ----------
 * Main file for Simple Basic Text Editor (SBTeditor)
 * A text editor written with the goal of being easily understood and modified.
 * By Jacqueline W. (jackiewalkergeneral@gmail.com)
 * ----------
 * See LICENSE.txt for licensing information
 */

#include "file.h"

/*
 * void help()
 * ----------
 * Print help information to the terminal. Invoked when an argument is
 * '-h' or '--help' or when an invalid argument is given.
 */
void help()
{
        printf( "SBTeditor - Simple Basic Text Editor\n----------\nA text " );
        printf( "editor written with the goal of being easily understood " );
        printf( "and modified.\n----------\n    ./sbtedit <filename>: " );
        printf( "Opens the file called filename if it exists. Else, create " );
        printf( "a file called filename and opens it." );
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
        struct File f = init_file( filename );

        populate_file( &f );

        unsigned int close = 0;

        while ( !close )
        {
                draw_screen();
        }
        
        return 0;
}

