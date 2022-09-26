#ifndef FILE_H__
#define FILE_H__

/**
 * file.h
 * ----------
 * Header file for library that handles all file interactions.
 * Stores an object that all file interactions must go through.
 */

#define FILENAME_LIM ( 255 ) /* Max length of filenames in most filesystems */
#define FILE_MIN_SZ  ( 512 ) /* Initial size allocated to f.data (in chars) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * struct File
 * ----------
 * Abstract of a file for use with the program. Stores data about the file
 * which can be modified with functions described below.
 * ----------
 * Initialize with init_file(), free with free_file()
 * After initialization, populate file with data with populate_file()
 * Modify file contents with insert_char() and overwrite_char().
 * save file contents to disk with export_file()
 */
struct File
{
        char* filename;
        char* data;
        unsigned int sz_alloc; /* Size allocated to f.data (in chars) */
        unsigned int sz_actual; /* Size actually used of f.data (in chars) */
};

/*
 * struct File init_file()
 * ----------
 * Initializes and returns a File object with an allocated filename <filename>.
 */
struct File init_file( char* filename );

/*
 * void free_file()
 * ----------
 * Frees memory allocated to a given File object <f> and set all other values
 * to 0.
 */
void free_file( struct File* f );

/*
 * void resize_file()
 * ----------
 * Given a pointer to a File object <f>, this function doubles the size of the
 * memory allocated to f.data.
 */
void resize_file( struct File* f );

/*
 * void populate_file()
 * ----------
 * Populates the data for a file given a <fptr> and a pointer to
 * a File object <f>. Copies the data character by character into f.data,
 * ignoring any non-printable characters (except for newline).
 */
void populate_file( struct File* f );
/*
 * void export_file()
 * ----------
 * For a given pointer to a File object <f>, write f.data to a file called
 * f.filename.
 */
void export_file( struct File* f );

/*
 * void overwrite_char()
 * ----------
 * Set the character at a given index <index> in the File <f> to the character
 * <ch>. Counterpart to insert_char().
 */
void overwrite_char( struct File* f, unsigned int index, char ch );

/*
 * void insert_char()
 * ----------
 * Move all characters after index <index> to the next index, then set the
 * character at <index> in the File object <f> to <ch>.
 */
void insert_char( struct File* f, unsigned int index, char ch );

#endif //FILE_H__
