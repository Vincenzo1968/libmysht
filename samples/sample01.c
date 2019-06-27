/*
   Copyleft (C) 2019 Vincenzo Lo Cicero

   Author: Vincenzo Lo Cicero.
   e-mail: vincenzolocicero68@gmail.com
       
   libmysht is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   libmysht is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with libmysht.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myScopeHashTable.h"

/*
gcc -Wall -W -pedantic -O3 -std=c99 -D_GNU_SOURCE sample01.c -o example01 -lmysht
*/

int myOnTraverse(const void* key, uint32_t keySize, void* data, uint32_t dataSize)
{
	if ( NULL != data )
		printf("'%s' keysize = %d <-> '%s' dataSize = %d\n", (char*)key, keySize, (char*)data, dataSize);
	else
		printf("'%s' keysize = %d\n", (char*)key, keySize);
				
	return 1;
}

int main()
{
	Scope myScope;
	
	char szKey[256];
	char szData[256];
	
	uint32_t sizekey;
	uint32_t sizedata;
	
	int res;
		
	if ( !scopeInit(&myScope) )
	{
		printf("Errore scopeInit.\n");
		return -1;		
	}
		
	printf("\n");
		
	strcpy(szKey, "Prova");
	sizekey = strlen(szKey) + sizeof(char);
	strcpy(szData, "data ");
	strcat(szData, szKey);
	sizedata = strlen(szData) + sizeof(char);
	printf("SCOPE INSERT Key -> '%s'\n", szKey);
	scopeInsert(&myScope, szKey, sizekey, szData, sizedata);
	
	strcpy(szKey, "Ecco");
	sizekey = strlen(szKey) + sizeof(char);
	strcpy(szData, "data ");
	strcat(szData, szKey);
	sizedata = strlen(szData) + sizeof(char);
	printf("SCOPE INSERT Key -> '%s'\n", szKey);
	scopeInsert(&myScope, szKey, sizekey, szData, sizedata);

	strcpy(szKey, "Vediamo");
	sizekey = strlen(szKey) + sizeof(char);
	strcpy(szData, "data ");
	strcat(szData, szKey);
	sizedata = strlen(szData) + sizeof(char);
	printf("SCOPE INSERT Key -> '%s'\n", szKey);
	scopeInsert(&myScope, szKey, sizekey, szData, sizedata);
	
	printf("SCOPE PUSH\n");
	scopePush(&myScope);
	
	strcpy(szKey, "Ciao a tutti");
	sizekey = strlen(szKey) + sizeof(char);
	strcpy(szData, "data ");
	strcat(szData, szKey);
	sizedata = strlen(szData) + sizeof(char);
	printf("SCOPE INSERT Key -> '%s'\n", szKey);
	scopeInsert(&myScope, szKey, sizekey, szData, sizedata);
	
	strcpy(szKey, "Salve");
	sizekey = strlen(szKey) + sizeof(char);
	strcpy(szData, "data ");
	strcat(szData, szKey);
	sizedata = strlen(szData) + sizeof(char);
	printf("SCOPE INSERT Key -> '%s'\n", szKey);
	scopeInsert(&myScope, szKey, sizekey, szData, sizedata);
	
	
	printf("----------------------------------------------------------------------------------------\n");
	
	
	strcpy(szKey, "Sabbenerica");
	sizekey = strlen(szKey) + sizeof(char);	
	res = scopeFind(&myScope, szKey, sizekey, szData, &sizedata, 0);
	if ( res >= 0 )
		printf("Stringa '%s' trovata: data -> '%s' datasize = %d\n", szKey, szData, sizedata);
	else
		printf("Stringa '%s' non trovata.\n", szKey);

	strcpy(szKey, "Ciao a tutti");
	sizekey = strlen(szKey) + sizeof(char);	
	res = scopeFind(&myScope, szKey, sizekey, szData, &sizedata, 0);
	if ( res >= 0 )
		printf("Stringa '%s' trovata: data -> '%s' datasize = %d\n", szKey, szData, sizedata);
	else
		printf("Stringa '%s' non trovata.\n", szKey);

	strcpy(szKey, "Cioa a tutti");
	sizekey = strlen(szKey) + sizeof(char);	
	res = scopeFind(&myScope, szKey, sizekey, szData, &sizedata, 0);
	if ( res >= 0 )
		printf("Stringa '%s' trovata: data -> '%s' datasize = %d\n", szKey, szData, sizedata);		
	else
		printf("Stringa '%s' non trovata.\n", szKey);
	
	strcpy(szKey, "Ecco");
	sizekey = strlen(szKey) + sizeof(char);	
	res = scopeFind(&myScope, szKey, sizekey, szData, &sizedata, 0);
	if ( res >= 0 )
		printf("Stringa '%s' trovata: data -> '%s' datasize = %d\n", szKey, szData, sizedata);
	else
		printf("Stringa '%s' non trovata.\n", szKey);
		
	printf("----------------------------------------------------------------------------------------\n");
		
	
	printf("SCOPE POP\n");
	scopePop(&myScope);
		
	printf("After scopePop:\n");
	
	strcpy(szKey, "Sabbenerica");
	sizekey = strlen(szKey) + sizeof(char);	
	res = scopeFind(&myScope, szKey, sizekey, szData, &sizedata, 0);
	if ( res >= 0 )
		printf("Stringa '%s' trovata: data -> '%s' datasize = %d\n", szKey, szData, sizedata);
	else
		printf("Stringa '%s' non trovata.\n", szKey);

	strcpy(szKey, "Ciao a tutti");
	sizekey = strlen(szKey) + sizeof(char);	
	res = scopeFind(&myScope, szKey, sizekey, szData, &sizedata, 0);
	if ( res >= 0 )
		printf("Stringa '%s' trovata: data -> '%s' datasize = %d\n", szKey, szData, sizedata);
	else
		printf("Stringa '%s' non trovata.\n", szKey);

	strcpy(szKey, "Cioa a tutti");
	sizekey = strlen(szKey) + sizeof(char);	
	res = scopeFind(&myScope, szKey, sizekey, szData, &sizedata, 0);
	if ( res >= 0 )
		printf("Stringa '%s' trovata: data -> '%s' datasize = %d\n", szKey, szData, sizedata);
	else
		printf("Stringa '%s' non trovata.\n", szKey);
	
	strcpy(szKey, "Ecco");
	sizekey = strlen(szKey) + sizeof(char);	
	res = scopeFind(&myScope, szKey, sizekey, szData, &sizedata, 0);
	if ( res >= 0 )
		printf("Stringa '%s' trovata: data -> '%s' datasize = %d\n", szKey, szData, sizedata);
	else
		printf("Stringa '%s' non trovata.\n", szKey);
	
	printf("\n");
				
	scopeFree(&myScope);
			
	return 0;
}
