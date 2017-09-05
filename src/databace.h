/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */



#ifndef HEADER_DATABACE_
#define HEADER_DATABACE_

extern int checkflag( char *flag, unsigned int len);
extern int  setflag(unsigned int n,long m );
extern int encryption( unsigned char key[] );
extern int  deciphering ( unsigned char key[], unsigned int chain[], int len );

#endif
