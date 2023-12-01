#ifndef WINDOW_H_
#define WINDOW_H_

#include "file.h"

struct Window;

struct Window* win_init( void );

void win_free( struct Window* );

void win_draw( struct Window* win );

void win_setFile( struct Window* win, struct File* );

void win_setCursor( struct Window* win, unsigned int col, unsigned int row );

void win_setBarMsg( struct Window* win, char* msg );

#endif
