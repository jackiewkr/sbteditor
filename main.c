#include "libs/file.h"

#include <stdio.h>

int main( void )
{
        struct File* file = file_init( "Makefile" );
        for ( unsigned int i = 0; i < file_getLength( file ); i++ )
        {
                printf( "%s\n", file_getLine( file, i ) );
        }

        return 0;
}
