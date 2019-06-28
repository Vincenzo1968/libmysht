/*
   Copyright (C) 2019 Vincenzo Lo Cicero

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
#include <ctype.h>

#include "myScopeHashTable.h"
//#include "../src/myScopeHashTable.h"

typedef struct tagPerson_t
{
	char szFirstName[128];
	char szLastName[128];
	int  nAge;
} Person_t;

int myHashFunc(HashTable_t* p, const void* pKey, uint32_t keysize)
{
	unsigned n = 0;
	const Person_t* pPers = (const Person_t*)pKey;
	
	char szKey[256];
	const char* s;
	
	UNUSED(keysize);
	
	strncpy(szKey, pPers->szLastName, 128);
	strncat(szKey, pPers->szFirstName, 128);
	
	s = (const char*)szKey;
	
	for ( ; *s; s++)
		n = 31 * n + *s;
		
	return n % p->htSize;
}


int myCompareFunc(const void* pKey1, uint32_t keysize1, const void* pKey2, uint32_t keysize2)
{
	UNUSED(keysize1);
	UNUSED(keysize2);
	
	const Person_t* pPers1 = (const Person_t*)pKey1;
	const Person_t* pPers2 = (const Person_t*)pKey2;
	
	int res;
	
	res = strncmp(pPers1->szLastName, pPers2->szLastName, 127);
	if ( 0 == res )
		res = strncmp(pPers1->szFirstName, pPers2->szFirstName, 127);
		
	return res;
}

uint32_t MakeKey(const char* szFirstName, const char* szLastName, char* szKey)
{
	uint32_t k = 0;
	
	strncpy(szKey, szLastName, 127);
	strncat(szKey, szFirstName, 127);
		
	while ( '\0' != szKey[k] )
	{
		szKey[k] = tolower( (int)szKey[k] );
		k++;
	}
	
	return k;
}

/*
gcc -Wall -W -pedantic -O3 -std=c99 -D_GNU_SOURCE ../src/myScopeHashTable.c sample03.c -o example03

gcc -Wall -W -pedantic -O3 -std=c99 -D_GNU_SOURCE -I/$HOME/MyLibs/libmysht/include -L/$HOME/MyLibs/libmysht/lib sample03.c -o example03 -lmysht
export LD_LIBRARY_PATH=$HOME/MyLibs/libmysht/lib:$LD_LIBRARY_PATH
./example03

gcc -Wall -W -pedantic -O3 -std=c99 -D_GNU_SOURCE sample03.c -o example03 -lmysht
*/

int main()
{
	Scope myScope;
	
	char szKey[256];
	//char szData[256];
	
	Person_t myPers;
	
	Person_t aPersons[5] = {
		{"Johnny" , "Depp", 35},
		{"Ben"    , "Affleck", 31},
		{"Jessica", "Alba", 27},
		{"Anne"   , "Baxter", 21},
		{"Robert" , "De Niro", 55}
		};
		
	int k;
	
	uint32_t sizekey;
	uint32_t sizedata;
	
	int res;
		
	if ( !scopeInit(&myScope, 0, myHashFunc, myCompareFunc) )
	{
		printf("Errore scopeInit.\n");
		return -1;		
	}
		
	printf("\n");
	
	for ( k = 0; k < 5; k++ )
	{
		sizekey = MakeKey(aPersons[k].szFirstName, aPersons[k].szLastName, szKey);
		printf("SCOPE INSERT Key -> ('%s'), First Name = '%s', Last Name = '%s', Age = %d\n", szKey, aPersons[k].szFirstName, aPersons[k].szLastName, aPersons[k].nAge);
		scopeInsert(&myScope, szKey, sizekey, &(aPersons[k]), sizeof(Person_t));
		
		if ( 2 == k )
		{
			printf("SCOPE PUSH\n");
			scopePush(&myScope, 0, myHashFunc, myCompareFunc);
		}
	}	
	
	printf("----------------------------------------------------------------------------------------\n");
	
	
	sizekey = MakeKey("Johnny", "Depp", szKey);
	res = scopeFind(&myScope, szKey, sizekey, &myPers, &sizedata, 0);
	if ( res >= 0 )
		printf("Record Key('%s') found: data -> First Name = '%s', Last Name = '%s', Age = %d; datasize = %d\n", szKey, myPers.szFirstName, myPers.szLastName, myPers.nAge, sizedata);
	else
		printf("Record Key('%s') not found.\n", szKey);

	sizekey = MakeKey("Tom", "Cruise", szKey);
	res = scopeFind(&myScope, szKey, sizekey, &myPers, &sizedata, 0);
	if ( res >= 0 )
		printf("Record Key('%s') found: data -> First Name = '%s', Last Name = '%s', Age = %d; datasize = %d\n", szKey, myPers.szFirstName, myPers.szLastName, myPers.nAge, sizedata);
	else
		printf("Record Key('%s') not found.\n", szKey);
		
	sizekey = MakeKey("Robert", "De Niro", szKey);
	res = scopeFind(&myScope, szKey, sizekey, &myPers, &sizedata, 0);
	if ( res >= 0 )
		printf("Record Key('%s') found: data -> First Name = '%s', Last Name = '%s', Age = %d; datasize = %d\n", szKey, myPers.szFirstName, myPers.szLastName, myPers.nAge, sizedata);
	else
		printf("Record Key('%s') not found.\n", szKey);		

		
	printf("----------------------------------------------------------------------------------------\n");
		
	
	printf("SCOPE POP\n");
	scopePop(&myScope);
		
	printf("After scopePop:\n");
	
	sizekey = MakeKey("Johnny", "Depp", szKey);
	res = scopeFind(&myScope, szKey, sizekey, &myPers, &sizedata, 0);
	if ( res >= 0 )
		printf("Record Key('%s') found: data -> First Name = '%s', Last Name = '%s', Age = %d; datasize = %d\n", szKey, myPers.szFirstName, myPers.szLastName, myPers.nAge, sizedata);
	else
		printf("Record '%s' not found.\n", szKey);

	sizekey = MakeKey("Tom", "Cruise", szKey);
	res = scopeFind(&myScope, szKey, sizekey, &myPers, &sizedata, 0);
	if ( res >= 0 )
		printf("Record Key('%s') found: data -> First Name = '%s', Last Name = '%s', Age = %d; datasize = %d\n", szKey, myPers.szFirstName, myPers.szLastName, myPers.nAge, sizedata);
	else
		printf("Record Key('%s') not found.\n", szKey);
		
	sizekey = MakeKey("Robert", "De Niro", szKey);
	res = scopeFind(&myScope, szKey, sizekey, &myPers, &sizedata, 0);
	if ( res >= 0 )
		printf("Record Key('%s') found: data -> First Name = '%s', Last Name = '%s', Age = %d; datasize = %d\n", szKey, myPers.szFirstName, myPers.szLastName, myPers.nAge, sizedata);
	else
		printf("Record Key('%s') not found.\n", szKey);
	
	printf("\n");
				
	scopeFree(&myScope);
			
	return 0;
}
