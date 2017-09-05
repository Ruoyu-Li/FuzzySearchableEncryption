/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */

#include <string.h>
#include "global.h"
#include "crypto.h"
#include "databace.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"

/* check whether the encrypted file system existed*/
int checkflag( char *flag, unsigned int len)
{
        FILE *flag_file;
        if( ( flag_file = fopen( "../cipher/flag.txt", "r" )) == NULL)
        {
             printf( "open flag.txt failed!\n" );
             return 0;
        }
        if( fgets( flag, len, flag_file ) == 0 )
        {
             printf( "read flag.txt failed!\n" );
             return -1;
        }
        fclose( flag_file );
        return 1;
}

int setflag( unsigned int n,long m )
{
        FILE *flag_file;
        if( ( flag_file = fopen( "../cipher/flag.txt", "w" )) == NULL)
        {
             print_error();
             printf( "open flag.txt failed!\n" );
             return 0;
        }
        char flag[10] = "Y";
         fprintf(flag_file,"%s\n",flag);
         fprintf(flag_file,"%d\n",n);
         if(m!=-1)
               fprintf(flag_file,"%ld",m);
         fclose( flag_file );
        return 1;
 }

/* encrypte original file */
int encryption( unsigned char key[] )
{
        FILE *original1, *original2;
        char line1[256] = { 0 }, line2[80] = { 0 };
        char cname[50];
        unsigned int idnumber = 0;
        if( (original1 = fopen( "../original/original1.txt", "r" )) == NULL)
        {
             print_error();
             printf( "open original1.txt failed!\n" );
             return 0;
        } 
       if( (original2 = fopen( "../original/original2.txt", "r" )) == NULL)
        {
             print_error();
             printf( "open original2.txt failed!\n" );
             return 0;
        }
       FILE* ffile = fopen( "../original/keyword_id.txt", "r" );
       unsigned int m;
       fscanf( ffile, "%u\n", &m );
       fclose(ffile);
        while(  fgets( line1, 80, original1 ) != NULL )
        {
             fgets( line2, 80, original2 );
             line1[strlen(line1)-1] = ' ';
             strcat( line1, line2);
             //sprintf( plain, "%s  %s", line1, line2);        
             sprintf( cname, "../cipher/%d.txt", idnumber );
             //printf( "line%d = %s\n", idnumber, line1);
             if( crypto_aes_cryp( key, idnumber, strlen(line1), (unsigned char*)line1, (unsigned char*)cname ) == 0 )
             {
 		    print_error();
                    printf( "encryption line%d failed!\n", idnumber);
                    return 0;
             }
             memset( line1, 0, 256);
             memset( line2, 0, 80);
             idnumber++;
             MainWindow::getMainWindowHandle()->ShowfprogressBar(idnumber,m);
        }
        fclose( original1 );
        fclose( original2 );
        return idnumber;
}

/* decipher cipher */
int deciphering( unsigned char key[],
                         unsigned int chain[],
                         int len)
{
       int i = 0;
       char cname[50];
      // char pname[50];
       unsigned char out2[2048];
       for( ; i < len; i++ )
       {
              sprintf( cname, "../cipher/%d.txt", chain[i] );
             // sprintf( pname, "../plain/%d.txt", chain[i] );
              if( crypto_aes_deci( key, (unsigned char*)cname, out2 ) == 0 )
              {

                    QMessageBox::warning( MainWindow::getMainWindowHandle(),"错误", QString("解密")+cname+QString("出错"), QMessageBox::Yes);
                    printf( "deciphering ../cipher/%d.txt failed!\n", chain[i] );
                    return 0;
              }
              MainWindow::getMainWindowHandle()->ShowResult(i,chain[i],(char*)out2);
       }
       return 1;
}
