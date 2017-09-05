/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */
#ifndef HEADER_WRAPPER_
#define HEADER_WRAPPER_

typedef struct _dsse_handle {
	struct _dsse_client_handle* client;
	struct _dsse_server_handle* server;
} dsse_handle;

extern dsse_handle* dsse_init( void );
extern void dsse_free( dsse_handle* handle );
extern int dsse_add( dsse_handle* handle, unsigned int id, char word[] );
extern int dsse_search
(
	dsse_handle* handle,
	char word[],
	list_handle* I,
	list_handle* J
);
extern int dsse_delete_id( dsse_handle* handle, unsigned int id );
extern int dsse_delete_mix(	dsse_handle* handle, unsigned int id, char word[] );

#endif
