/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */
#ifndef HEADER_PAIR_
#define HEADER_PAIR_

#define WORD_LEN 32

typedef struct _word
{
	char         word[WORD_LEN];
	unsigned int count;
} word;

typedef struct _id
{
	unsigned int id;
	unsigned int count;
} id;

typedef struct _pair
{
	unsigned int id;
	char        word[WORD_LEN];
} pair;

typedef struct _pair_handle
{
	unsigned int pair_amount;
	char*        file_path;
	unsigned int word_amount;
	unsigned int pair_index;
	word*        word_list;
	pair*        pair_list;
} pair_handle;

extern pair_handle* pair_init
(
	unsigned int amount,
	char*        path
);
extern void pair_free( pair_handle* handle );
extern int pair_generate( pair_handle* handle );
extern int pair_get_item( pair_handle* handle, char* word[], unsigned int* id );

#endif
