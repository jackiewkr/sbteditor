/**
 * screen.c
 * ----------
 * Library that handles all drawing to the screen using curses.
 * 
 */

#include "screen.h"

/*
 * struct Screen init_screen()
 * ----------
 * Initializes a Screen object and initializes all necessary ncurses options.
 */
struct Screen init_screen()
{
        struct Screen s;
        
        /* Ncurses stuff */
        initscr();
        noecho();
        keypad( stdscr, TRUE );
        cbreak();

        /* Ncurses colour stuff */
        //TODO: colour highlighting

        /* Initialize Screen object */
        getmaxyx( stdscr, s.rows, s.cols );

        return s;
}

void free_screen( struct Screen* s )
{
        /* Set screen dims to 0  */
        s->rows = 0;
        s->cols = 0;

        /* Undoing ncurses stuff  */
        echo();
        nocbreak();
        endwin();
}

/** DRAWING FUNCS BELOW */

#if 0
static void draw_file( struct File* f )
{
        /* Move to top-left of terminal */
        move( 0, 0 );
        for ( unsigned int i = 0; i < f->sz_actual; i++ )
        {
                addch( f->data[i] );       
        }
}
#endif

static void draw_file( struct File* f, unsigned int start_line,
                       unsigned int end_line )
{
        unsigned int line_counter = 0;
        unsigned int index = 0;

        while (start_line > line_counter && index < f->sz_actual )
        {
                if ( f->data[index++] == '\n' )
                {
                        line_counter++;
                }
        }

        /* Move to top-left of terminal */
        move( 0, 0 );
        for ( unsigned int i = index; i < f->sz_actual; i++ )
        {
                if (line_counter < end_line )
                {
                        addch( f->data[i] );

                        if ( f->data[i] == '\n' )
                        {
                                line_counter++;
                        }
                }
        }
}

int draw_screen( struct Screen* s, struct File* f, unsigned int offset )
{
        getmaxyx( stdscr, s->rows, s->cols );

        clear();
        draw_file( f, offset, ( s->rows + offset - 1 ) );
        refresh();

        int ch;
        ch = getch();

        return ch;
}
