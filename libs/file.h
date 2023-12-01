#ifndef FILE_H_
#define FILE_H_

struct File;

/* file_init()
 * ===========
 * Initializes a file struct.
 * If file_loc is not NULL, copy contents of file to the file struct. 
 * If file_loc does not exist, initialize blank file that will save at given 
 * file_loc. 
 * If file_loc is NULL, then initialize blank file with no save location.
 *
 * Returns NULL on error. Additional details are printed to stderr.
 */
struct File* file_init( const char* file_loc );

/* file_free()
 * ===========
 * Frees the File struct and any memory associated with it.
 */
void file_free( struct File* );

/* file_getLine()
 * ==============
 * Returns the given line at the given line_num as a string literal.
 *
 * Returns NULL on error. Additional details are printed to stderr.
 */ 
char* file_getLine( struct File*, unsigned int line_num );

/* file_getLength()
 * ==============
 * Returns the number of lines in the file.
 */
unsigned int file_getLength( struct File* );

#endif
