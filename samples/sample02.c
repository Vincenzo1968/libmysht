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
gcc -Wall -W -pedantic -O2 -std=c99 -D_GNU_SOURCE sample02.c -o example02 -lmysht
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
	HashTable_t myTable;
	
	char szKey[256];
	char szData[256];
	
	uint32_t sizekey;
	uint32_t sizedata;
		
	int res;
	
	if ( !htInit(&myTable, 8191, StringHashFunc, StringCompareFunc) )
	{
		printf("Errore htInit.\n");
		return -1;
	}
	
	printf("\n");
		
	strcpy(szKey, "Prova");
	sizekey = strlen(szKey) + sizeof(char);
	strcpy(szData, "data ");
	strcat(szData, szKey);
	sizedata = strlen(szData) + sizeof(char);
	htInsert(&myTable, szKey, sizekey, szData, sizedata);
	
	strcpy(szKey, "Ecco");
	sizekey = strlen(szKey) + sizeof(char);
	strcpy(szData, "data ");
	strcat(szData, szKey);
	sizedata = strlen(szData) + sizeof(char);
	htInsert(&myTable, szKey, sizekey, szData, sizedata);

	strcpy(szKey, "Vediamo");
	sizekey = strlen(szKey) + sizeof(char);
	strcpy(szData, "data ");
	strcat(szData, szKey);
	sizedata = strlen(szData) + sizeof(char);
	htInsert(&myTable, szKey, sizekey, szData, sizedata);
	
	strcpy(szKey, "Ciao a tutti");
	sizekey = strlen(szKey) + sizeof(char);
	strcpy(szData, "data ");
	strcat(szData, szKey);
	sizedata = strlen(szData) + sizeof(char);
	htInsert(&myTable, szKey, sizekey, szData, sizedata);
	
	strcpy(szKey, "Salve");
	sizekey = strlen(szKey) + sizeof(char);
	strcpy(szData, "data ");
	strcat(szData, szKey);
	sizedata = strlen(szData) + sizeof(char);
	htInsert(&myTable, szKey, sizekey, szData, sizedata);
	

	strcpy(szKey, "Sabbenerica");
	sizekey = strlen(szKey) + sizeof(char);	
	res = htFind(&myTable, szKey, sizekey, szData, &sizedata);
	if ( res >= 0 )
		printf("Stringa '%s' trovata: data -> '%s' datasize = %d\n", szKey, szData, sizedata);		
	else
		printf("Stringa '%s' non trovata.\n", szKey);

	strcpy(szKey, "Ciao a tutti");
	sizekey = strlen(szKey) + sizeof(char);	
	res = htFind(&myTable, szKey, sizekey, szData, &sizedata);
	if ( res >= 0 )
		printf("Stringa '%s' trovata: data -> '%s' datasize = %d\n", szKey, szData, sizedata);
	else
		printf("Stringa '%s' non trovata.\n", szKey);

	strcpy(szKey, "Cioa a tutti");
	sizekey = strlen(szKey) + sizeof(char);	
	res = htFind(&myTable, szKey, sizekey, szData, &sizedata);
	if ( res >= 0 )
		printf("Stringa '%s' trovata: data -> '%s' datasize = %d\n", szKey, szData, sizedata);
	else
		printf("Stringa '%s' non trovata.\n", szKey);
	
	strcpy(szKey, "Ecco");
	sizekey = strlen(szKey) + sizeof(char);	
	res = htFind(&myTable, szKey, sizekey, szData, &sizedata);
	if ( res >= 0 )
		printf("Stringa '%s' trovata: data -> '%s' datasize = %d\n", szKey, szData, sizedata);
	else
		printf("Stringa '%s' non trovata.\n", szKey);
	
	printf("----------------------------------------------------------------------------------------\n");
	
	printf("ON TRAVERSE:\n");
	
	htTraverse(&myTable, myOnTraverse);	
	
	printf("----------------------------------------------------------------------------------------\n");
	
	htDeleteKey(&myTable, szKey, sizekey);
	
	printf("ON TRAVERSE After deleting key '%s':\n", szKey);
	
	htTraverse(&myTable, myOnTraverse);	
	
	printf("\n");
	
	htFree(&myTable);
	
	return 0;
}
