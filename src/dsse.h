/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */
#ifndef HEADER_DSSE_
#define HEADER_DSSE_

#define SKEY_LEN 16
#define BASE_LEN 10

#include "list.h"
#include "avltree.h"

/* client data structure */

typedef struct _random_word {
	char*         word;
	unsigned char random[BASE_LEN];
} random_word;

typedef struct _random_id {
	unsigned int  id;
	unsigned char random[BASE_LEN];
} random_id;

typedef struct _secret_keys {
	unsigned char key1[SKEY_LEN];
	unsigned char key2[SKEY_LEN];
} secret_keys;

typedef struct _private_states {
	avl_handle* word;
	avl_handle* id;
} private_states;

typedef struct _dsse_client_handle {
	secret_keys    skeys;
	private_states pstates;
} dsse_client_handle;

/* server data structure */

typedef struct _cipher_word {
	unsigned char part1[BASE_LEN];
	unsigned char part2[BASE_LEN*2+1];
	unsigned char part3[BASE_LEN];
} cipher_word;

typedef struct _cipher_id {
	unsigned char part1[BASE_LEN];
	unsigned char part2[BASE_LEN*3+1];
	unsigned char part3[BASE_LEN];
} cipher_id;

typedef struct _cipher_mix {
	unsigned char part1[BASE_LEN];
	unsigned char part2[BASE_LEN*2+1];
	unsigned char part3[BASE_LEN];
} cipher_mix;

typedef struct _dsse_server_handle {
	avl_handle* word;
	avl_handle* id;
	avl_handle* mix;
} dsse_server_handle;

/* token */

typedef struct _dsse_token {
	unsigned char token1[BASE_LEN];
	unsigned char token2[BASE_LEN];
} dsse_token;

/* functions */

extern dsse_client_handle* dsse_client_init( void );
extern void dsse_client_free( dsse_client_handle* handle );
extern int dsse_client_add
(
	dsse_client_handle* handle,
	unsigned int        id,
	char                word[],
	cipher_word*        cword,
	cipher_id*          cid,
	cipher_mix*         cmix
);
extern int dsse_client_search
(
	dsse_client_handle* handle,
	char                word[],
	dsse_token*         token
);
extern int dsse_client_delete_id
(
	dsse_client_handle* handle,
	unsigned int        id,
	dsse_token*         token
);
extern int dsse_client_delete_mix
(
	dsse_client_handle* handle,
	unsigned int        id,
	char                word[],
	dsse_token*         token
);

extern dsse_server_handle* dsse_server_init( void );
extern void dsse_server_free( dsse_server_handle* handle );
extern int dsse_server_add
(
	dsse_server_handle* handle,
	cipher_word*        cword,
	cipher_id*          cid,
	cipher_mix*         cmix
);
extern int dsse_server_search
(
	dsse_server_handle* handle,
	dsse_token*         token,
	list_handle*        I,
	list_handle*        J
);
extern int dsse_server_delete_id
(
	dsse_server_handle* handle,
	dsse_token*         token
);
extern int dsse_server_delete_mix
(
	dsse_server_handle* handle,
	dsse_token*         token
);

#endif
