#ifndef SCREEN_H__
#define SCREEN_H__

/**
 * screen.h
 * ========
 * Abstraction of the screen and parts of the editor logic.
 *
 * Licensed under the GNU GPL v3 license. See LICENSE for more details.
 * ----------------------------------------------------------------------------
 */

struct Screen;

struct Screen* s_construct();

void s_destruct( struct Screen* s );

int s_update( struct Screen* s, struct File* f );

void s_moveCursor( struct Screen* s, struct File* f, int y, int x );

void s_writeChar( struct Screen* s, struct File* f, int ch );

#endif //SCREEN_H__
