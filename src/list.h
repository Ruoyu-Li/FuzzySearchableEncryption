/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */
#ifndef HEADER_LIST_
#define HEADER_LIST_

#define LIST_INIT_LEN 250

typedef struct _list_handle {
	void*  list;
	size_t list_length;
	size_t list_used;
	size_t data_size;
	size_t index;
} list_handle;

extern list_handle* list_init( size_t data_size );
extern void list_free( list_handle* handle );
extern int list_add( list_handle* handle, void* data );
extern void* list_get_by_index( list_handle* handle, size_t index );
extern int list_get_item( list_handle* handle, void** data );
extern size_t list_get_amount( list_handle* handle );

#endif
