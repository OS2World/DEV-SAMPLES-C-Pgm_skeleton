
/*
 * This program can serve as a template for a routine that searches for
 * keywords in CONFIG.SYS and writes out a new version with a given token.
 * It assumes you're booting off drive C, it will make the change on EVERY
 * matching line found and generally needs to be tuned for your requirements.

 *  Copyright (C) 1991 IBM Corporation
 *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *      sample code created by IBM Corporation. This sample code is not
 *      part of any standard or IBM product and is provided to you solely
 *      for  the purpose of assisting you in the development of your
 *      applications.  The code is provided "AS IS", without
 *      warranty of any kind.  IBM shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.                                                    *
 */
#include <stdio.h>
#include <string.h>

#define KEY_LEN 32
#define LINE_LEN 256

typedef unsigned short BOOL;
#define TRUE   1
#define FALSE  0

main( int argc, char **argv )
{
	FILE    *fOld;
	FILE    *fNew;
	BOOL	bDone;
	char	pszKeyword[ KEY_LEN ];
	char	pszLine[ LINE_LEN ];
	short   sSearchLen;

	if ( argc != 3 )
	{
		printf("Please enter CONFIG.SYS keyword and token\n" );
		return( -1 );
	}

	sprintf( pszKeyword, "SET %s", (char *)argv[1] );
	sSearchLen = strlen( pszKeyword );
    printf("Searching for '%s' in CONFIG.SYS\n", pszKeyword );

	fOld = fopen("C:\\CONFIG.SYS","r" );
	if ( ferror( fOld ) )
	{
		printf("Error opening C:\\CONFIG.SYS");
		return( -2 );
	}

	fNew = fopen("C:\\CONFIG.NEW","w" );
	if ( ferror( fNew ) )
	{
		printf("Error opening C:\\CONFIG.NEW");
		return( -3 );
	}

	bDone = FALSE;
	while ( !bDone )
	{
		if ( fgets( pszLine, LINE_LEN, fOld ) != NULL )
		{
			/* got a line of text */
			if ( strncmp( pszLine, pszKeyword, sSearchLen ) == 0 )
			{
				/* we matched the keyword, so now scan the string for token */
				if ( strcmp( pszLine, (char *)argv[2] ) == 0 )
				{
					printf("Token already on line.\n");
				}
				else
				{
					printf("Adding token to line\n");
					pszLine[ strlen(pszLine) - 1 ] = '\0';  /* nuke \n */
					strcat( pszLine, (char *)argv[2] );
				}
			    printf( "%s\n", pszLine );
				
			}
			if ( fputs( pszLine, fNew ) <= 0 )
			{
				printf("Error during write of CONFIG.NEW\n" );
				bDone = TRUE;
			}
		}
		else
		{
			/* could be EOF or file error */
			if ( ferror( fOld ) )
				printf("Error during read of CONFIG.SYS\n" );
			bDone = TRUE;
		}
	}
	fclose( fOld );
	fclose( fNew );
	return( 0 );
}

