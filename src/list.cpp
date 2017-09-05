/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */
#include <stdlib.h>
#include <string.h>
#include "list.h"

list_handle* list_init( size_t data_size )
{
	list_handle* lh = (list_handle*) calloc( 1, sizeof(list_handle) );
	if( lh == 0 ) return 0;

	lh->list = (void*) calloc( LIST_INIT_LEN, data_size );
	if( lh->list == 0 )
	{
		free( lh );
		return 0;
	}
	lh->list_length = LIST_INIT_LEN;
	lh->data_size = data_size;

	return lh;
}

void list_free( list_handle* handle )
{
	if( handle )
	{
		if( handle->list ) free( handle->list );
		free( handle );
	}
}

int list_add( list_handle* handle, void* data )
{
	if( handle == 0 ) return 0;

	if( handle->list_used >= handle->list_length )
	{
		void* list_new;

		list_new = realloc( handle->list,
						    handle->list_length * 2 * handle->data_size );
		if( list_new == 0 ) return 0;

		handle->list = list_new;
		handle->list_length *= 2;
	}

	memcpy( handle->list + handle->list_used * handle->data_size, data,
		    handle->data_size );
	handle->list_used++;

	return 1;
}

void* list_get_by_index( list_handle* handle, size_t index )
{
	if( handle == 0 ) return 0;
	if( index < 0 || index >= handle->list_used ) return 0;

	return handle->list + handle->data_size * index;
}

int list_get_item( list_handle* handle, void** data )
{
	if( handle == 0 ) return 0;
	if( handle->index < handle->list_used )
	{
        *data = handle->list + handle->index * handle->data_size;
		handle->index++;
		return 1;
	}
	else
	{
		handle->index = 0;
		return 0;
	}
}

size_t list_get_amount( list_handle* handle )
{
	return handle->list_used;
}
