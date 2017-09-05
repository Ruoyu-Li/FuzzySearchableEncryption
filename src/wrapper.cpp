/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */
#include "global.h"
#include "dsse.h"
#include "list.h"
#include "wrapper.h"

dsse_handle* dsse_init( void )
{
    dsse_handle* dh = (dsse_handle*)calloc( 1, sizeof(dsse_handle) );
	if( dh == 0 ) return 0;

	dh->client = dsse_client_init();
	dh->server = dsse_server_init();
	if( dh->client == 0 || dh->server == 0 )
	{
		print_error();
		printf( "init dsse error\n" );
		if( dh->client ) dsse_client_free( dh->client );
		if( dh->server ) dsse_server_free( dh->server );
		return 0;
	}

	return dh;
}

void dsse_free( dsse_handle* handle )
{
	if( handle )
	{
		dsse_client_free( handle->client );
		dsse_server_free( handle->server );
		free( handle );
	}
}

int dsse_add( dsse_handle* handle, unsigned int id, char word[] )
{
	cipher_word cword;
	cipher_id   cid;
	cipher_mix  cmix;

	if( dsse_client_add(handle->client, id, word, &cword, &cid, &cmix) == 0
	 || dsse_server_add(handle->server, &cword, &cid, &cmix) == 0 )
	{
		print_error();
		printf( "dsse add failed\n" );
		return 0;
	}

	return 1;
}

int dsse_search( dsse_handle* handle,
				 char word[],
				 list_handle* I,
				 list_handle* J )
{
	dsse_token token;

	if( dsse_client_search(handle->client, word, &token) == 0
	 || dsse_server_search(handle->server, &token, I, J) == 0 )
	{
		print_error();
		printf( "dsse search failed\n" );
		return 0;
	}

	return 1;
}

int dsse_delete_id( dsse_handle* handle, unsigned int id )
{
	dsse_token token;

	if( dsse_client_delete_id(handle->client, id, &token) == 0
	 || dsse_server_delete_id(handle->server, &token) == 0 )
	{
		print_error();
		printf( "dsse delete id failed\n" );
		return 0;
	}

	return 1;
}

int dsse_delete_mix( dsse_handle* handle, unsigned int id, char word[] )
{
	dsse_token token;

	if( dsse_client_delete_mix(handle->client, id, word, &token) == 0
	 || dsse_server_delete_mix(handle->server, &token) == 0 )
	{
		print_log();
		printf( "dsse delete word failed\n" );
		return 0;
	}

	return 1;
}
