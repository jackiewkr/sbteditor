#ifndef SCREEN_H__
#define SCREEN_H__

/**
 * screen.c
 * ----------
 * Library that handles all drawing to the screen using curses.
 * 
 */

#include <curses.h>
#include "file.h"

/**
 * struct Screen
 * ----------
 * Abstraction of a screen. Only stores the dimensions of the current screen.
 * All other bits of the screen are handled by ncurses
 */
struct Screen
{
        int rows;
        int cols;
};

/*
 * struct Screen init_screen()
 * ----------
 * Initializes a Screen object and initializes all necessary ncurses options.
 */
struct Screen init_screen();

void free_screen( struct Screen* s );

/** DRAWING FUNCS BELOW */

int draw_screen( struct Screen* s, struct File* f, unsigned int offset );

#endif //SCREEN_H__
