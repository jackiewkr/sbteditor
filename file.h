#ifndef FILE_H__
#define FILE_H__
/**
 * file.h
 * ======
 * Abstract Data Type of a file used within the editor. Split into 'lines' for
 * ease of use. Also handles file I/O.
 *
 * Licensed under GNU GPL v3 license. See LICENSE for more details. 
 * ----------------------------------------------------------------------------
 */


struct File;

void f_destruct( struct File* f );

char* f_getLine( struct File* f, int index );

int f_getLineLen( struct File* f, int index );

int f_getMaxLines( struct File* f );

void f_insertChar( struct File* f, int index, int l_index, int ch );

/** FILE I/O  **/
struct File* f_import( char* filename, int filename_sz );

void f_export( struct File* f );
#endif //FILE_H__
