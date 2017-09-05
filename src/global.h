/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */
#ifndef HEADER_GLOBAL_
#define HEADER_GLOBAL_

#include <stdio.h>
#include <stdlib.h>

/* print status in the program */
#define print_log()   \
fprintf( stdout, "\x1b[34m[LOG %s:%d %s]\x1b[0m ",  \
		 __FILE__, __LINE__, __func__ )
#define print_debug() \
fprintf( stdout, "\x1b[33m[DEBUG %s:%d %s]\x1b[0m ",\
		 __FILE__, __LINE__, __func__ )
#define print_error() \
fprintf( stderr, "\x1b[31m[ERROR %s:%d %s]\x1b[0m ",\
		 __FILE__, __LINE__, __func__ )

#endif
