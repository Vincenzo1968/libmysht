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

#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myScopeHashTable.h"

int htInit(HashTable_t* p, uint32_t htSize, pfnHashFunc HashFunc, pfnCompareFunc CompareFunc)
{
	uint32_t x;
	
	if ( NULL == p )
	{
		printf("ERRORE htInit -> p È NULL\n");
		return 0;
	}
	
	p->pHashTable = NULL;
	
	p->htSize = htSize;
	if ( p->htSize == 0 )
		p->htSize = HT_SIZE;
				
	p->pHashTable = (HashTableItem_t**)malloc(sizeof(HashTableItem_t*) * p->htSize);
	if ( !p->pHashTable )
		return 0;
				
	for ( x = 0; x < p->htSize; x++ )
		p->pHashTable[x] = NULL;
		
	p->HashFunc = HashFunc;
	p->CompareFunc = CompareFunc;
	
	return 1;
}

int htSetHashFunc(HashTable_t* p, pfnHashFunc HashFunc)
{
	p->HashFunc = HashFunc;
	
	return 1;
}

int htSetCompareFunc(HashTable_t* p, pfnCompareFunc CompareFunc)
{
	p->CompareFunc = CompareFunc;
	
	return 1;
}

HashTableItem_t* htNewNode(const void* pKey, uint32_t keysize, const void* pData, uint32_t datasize)
{
	HashTableItem_t* n = NULL;

	n = (HashTableItem_t*)malloc(sizeof(HashTableItem_t));

	if ( NULL == n )
		return NULL;
				
	n->pKey = (uint8_t*)malloc(sizeof(uint8_t) * keysize);
	if ( NULL == n->pKey )
	{
		printf("ERRORE htNewNode 1: memoria insufficiente\n");
		free(n);
		n = NULL;
		return NULL;
	}
	memcpy(n->pKey, pKey, keysize);
	n->keysize = keysize;
		
	if ( NULL != pData )
	{
		n->pData = (uint8_t*)malloc(sizeof(uint8_t) * datasize);
		if ( NULL == n->pData )
		{
			printf("ERRORE htNewNode 2: memoria insufficiente\n");
			free(n->pKey);
			n->pKey = NULL;
			free(n);
			n = NULL;
			return NULL;
		}
		memcpy(n->pData, pData, datasize);
		n->datasize = datasize;		
	}
	else
	{
		n->pData = NULL;
		n->datasize = 0;
	}
		
	n->next = NULL;

	return n;
}

int htFind(HashTable_t* p, const void* pKey, uint32_t keysize, void* pData, uint32_t* datasize)
{	
	int index = 0;
	HashTableItem_t* t = NULL;
		
	if ( NULL == p )
	{
		printf("\n\nERRORE htFind -> p È NULL\n\n");
		return -2;
	}	
				
	index = p->HashFunc(p, pKey, keysize);

	t = p->pHashTable[index];
	while ( t != NULL )
	{
		if ( p->CompareFunc(t->pKey, t->keysize, pKey, keysize) == 0 )
		{
			if ( NULL != pData )
			{
				if ( NULL != t->pData )
				{
					memcpy(pData, t->pData, t->datasize);
					*datasize = t->datasize;
					
					return index;
				}
				else
				{
					pData = NULL;
					*datasize = 0;					
				}
			}
			
			return index;
		}
					
		t = t->next;
	}
	
	return -1;
}

int htUpdateData(HashTable_t* p, const void* pKey, uint32_t keysize, const void* pData, uint32_t datasize)
{	
	int index = 0;
	HashTableItem_t* t = NULL;
		
	if ( NULL == p )
	{
		printf("\n\nERRORE htUpdateData -> p È NULL\n\n");
		return -2;
	}	
				
	index = p->HashFunc(p, pKey, keysize);
	
	t = p->pHashTable[index];
	while ( t != NULL )
	{
		if ( p->CompareFunc(t->pKey, t->keysize, pKey, keysize) == 0 )
		{
			if ( NULL != pData )
			{
				if ( NULL != t->pData )
				{
					free(t->pData);
					t->pData = NULL;
					
					t->pData = (uint8_t*)malloc(sizeof(uint8_t) * datasize);
					if ( NULL == t->pData )
					{
						printf("ERRORE htUpdateData: memoria insufficiente\n");
						return -2;
					}					
					
					memcpy(t->pData, pData, t->datasize);
					t->datasize = datasize;
					
					return index;
				}
			}
						
			return index;
		}
					
		t = t->next;
	}
	
	return -1;
}

int htDeleteKey(HashTable_t* p, const void* pKey, uint32_t keysize)
{
	int index = 0;
	HashTableItem_t* t = NULL;
	
	HashTableItem_t* prev = NULL;
				
	index = p->HashFunc(p, pKey, keysize);

	t = p->pHashTable[index];
	while ( t != NULL )
	{
		if ( p->CompareFunc(t->pKey, t->keysize, pKey, keysize) == 0 )
		{
			if ( NULL != t->pData )
			{
				free(t->pData);
				t->pData = NULL;				
			}
			
			free(t->pKey);
			t->pKey = NULL;
			
			if ( NULL == prev )
			{
				p->pHashTable[index] = NULL;
			}
			else
			{
				prev->next = t->next;	
			}
			
			free(t);
			t = NULL;
			
			return index;
		}

		prev = t;
		t = t->next;
	}

	return -1;	
}

int htInsert(HashTable_t* p, const void* pKey, uint32_t keysize, const void* pData, uint32_t datasize)
{	
	int index = 0;
	HashTableItem_t* t = NULL;
		
	index = p->HashFunc(p, pKey, keysize);

	t = p->pHashTable[index];
	if ( t == NULL )
	{
		p->pHashTable[index] = htNewNode(pKey, keysize, pData, datasize);
		return index;
	}

	while ( t != NULL )
	{
		if ( p->CompareFunc(t->pKey, t->keysize, pKey, keysize) == 0 )
		{
			return -1;
		}
		if ( t->next == NULL )
		{
			t->next = htNewNode(pKey, keysize, pData, datasize);
			return index;
		}
		t = t->next;
	}
	
	return index;
}

void htFreeList(HashTableItem_t* first)
{
	HashTableItem_t *n1 = first, *n2 = NULL;
	
	while ( n1 != NULL )
	{
		n2 = n1->next;	
				
		free(n1->pKey);
		n1->pKey = NULL;
				
		if ( NULL != n1->pData )
		{
			free(n1->pData);
			n1->pData = NULL;
		}
				
		free(n1);
				
		n1 = NULL;
		
		n1 = n2;		
	}
}

void htFree(HashTable_t* p)
{
	uint32_t x;
	
	for( x = 0; x < p->htSize; x++ )
	{
		if ( p->pHashTable[x] )
		{
			htFreeList(p->pHashTable[x]);
			p->pHashTable[x] = NULL;
		}
	}	
	
	free(p->pHashTable);
		
	p->pHashTable = NULL;
	
	p->htSize = 0;
}

void htTraverse(HashTable_t* p, pfnOnTraverse OnTraverse)
{
	uint32_t x;
	
	for( x = 0; x < p->htSize; x++ )
	{
		if ( p->pHashTable[x] )
		{
			HashTableItem_t *n1 = p->pHashTable[x];
	
			while ( n1 != NULL )
			{
				OnTraverse(n1->pKey, n1->keysize, n1->pData, n1->datasize);
				n1 = n1->next;
			}
		}
	}		
}

/*
int StringHashFunc(HashTable_t* p, const void* pKey, uint32_t keysize)
{
	int index = 0;
	int a = 31415;
	const char* s = (const char*)pKey;
	
	for(; *s; s++)
		index = (a*index + *s) % p->htSize;
				
	return index;	
}
*/

/*
int StringHashFunc(HashTable_t* p, const void* pKey, uint32_t keysize)
{
    const unsigned char *s = pKey;
    unsigned h = 0;
    uint32_t i;

    for (i = 0; i < keysize - 1; i++)
    {
        h ^= (h << 5) + (h >> 2) + s[i];
    }

    return h % p->htSize;
}
*/

/*
int StringHashFunc(HashTable_t* p, const void* pKey, uint32_t keysize)
{
    const unsigned char *s = pKey;
    unsigned h = 0;
    uint32_t i;

    for (i = 0; i < keysize - 1; i++)
    {
        h = 33 * h ^ s[i];
    }

    return h % p->htSize;
}
*/

/*
int StringHashFunc(HashTable_t* p, const void* pKey, uint32_t keysize)
{
    const unsigned char *s = pKey;
    unsigned h = 2166136261;
    uint32_t i;

    for (i = 0; i < keysize - 1; i++)
    {
        h = (h * 16777619) ^ s[i];
    }    

    return h % p->htSize;
}
*/

/*
int StringHashFunc(HashTable_t* p, const void* pKey, uint32_t keysize)
{
    const unsigned char *s = pKey;    
    unsigned h = 0, g;
    uint32_t i;

    for (i = 0; i < keysize - 1; i++)
    {
        h = (h << 4) + s[i];
        g = h & 0xf0000000L;

        if (g != 0)
        {
            h ^= g >> 24;
        }

        h &= ~g;
    }

    return h % p->htSize;
}
*/

int StringHashFunc(HashTable_t* p, const void* pKey, uint32_t keysize)
{
	unsigned n = 0;
	const char* s = (const char*)pKey;
	
	UNUSED(keysize);
	
	for ( ; *s; s++)
		n = 31 * n + *s;
		
	return n % p->htSize;
}


int StringCompareFunc(const void* pKey1, uint32_t keysize1, const void* pKey2, uint32_t keysize2)
{
	UNUSED(keysize1);
	UNUSED(keysize2);
	
	return strncmp((char*)pKey1, (char*)pKey2, BLOCK_SIZE - 1);
}

// ----------------------------------------------------- Begin Scope Functions ---------------------------------------------------------

int scopeInit(Scope *pScope)
{
	HashTable_t **pHT = NULL;
	int x;

	pScope->top = 0;

	pHT = (HashTable_t**)malloc(sizeof(HashTable_t*) * HT_SIZE);
	if ( pHT != NULL )
	{
		for ( x = 0; x < HT_SIZE; x++ )
		{
			pHT[x] = (HashTable_t*)malloc(sizeof(HashTable_t));
			if ( pHT[x] == NULL )
			{
				printf("ERRORE scopeInit: memoria non sufficiente.\n");
				free(pHT);
				pHT = NULL;
				return 0; 
			}
			if ( !(htInit(pHT[x], HT_SIZE, StringHashFunc, StringCompareFunc)) )
			{
				printf("Errore scopeInit -> htInit.\n");
				return -1;
			}
		}
	}
	else
	{
		printf("ERRORE scopeInit: memoria non sufficiente.\n");
		return 0;
	}

	pScope->stack[pScope->top] = pHT;
	
	for ( x = 1; x < SCOPE_SIZE; x++ )
		pScope->stack[x] = NULL;
				
	return 1;
}

int scopePush(Scope *pScope)
{
	HashTable_t **pHT = NULL;
	int x;

	pScope->top++;
	if ( pScope->top > SCOPE_SIZE - 1 )
	{
		printf("ERRORE scopePush: scope stack pieno!\n");
		pScope->top = SCOPE_SIZE - 1;
		return 0;
	}

	pHT = (HashTable_t**)malloc(sizeof(HashTable_t*) * HT_SIZE);
	if ( pHT != NULL )
	{
		for ( x = 0; x < HT_SIZE; x++ )
		{
			pHT[x] = (HashTable_t*)malloc(sizeof(HashTable_t));
			if ( pHT[x] == NULL )
			{
				printf("ERRORE scopePush: memoria non sufficiente.\n");
				free(pHT);
				pHT = NULL;
				return 0; 
			}
			if ( !(htInit(pHT[x], HT_SIZE, StringHashFunc, StringCompareFunc)) )
			{
				printf("Errore scopePush -> htInit.\n");
				return -1;
			}
		}
	}
	else
	{
		printf("ERRORE scopePush: memoria non sufficiente.\n");
		return 0;
	}
			
	pScope->stack[pScope->top] = pHT;
	
	return 1;
}

int scopePop(Scope *pScope)
{
	HashTable_t** pHT = NULL;
	int x;

	if ( pScope->top < 0 )
		return pScope->top;

	pHT = pScope->stack[pScope->top];
	for ( x = 0; x < HT_SIZE; x++)
	{
		htFree(pHT[x]);
		
		free(pHT[x]);
		pHT[x] = NULL;			
	}
		
	free(pHT);
	pHT = NULL;
				
	pScope->stack[pScope->top] = NULL;
	pScope->top--;

	return pScope->top;
}

void scopeFree(Scope* pScope)
{
	int x;
		
	while ( pScope->top >= 0 )
	{
		if ( NULL != pScope->stack[pScope->top] )
		{
			for ( x = 0; x < HT_SIZE; x++ )
			{
				htFree(pScope->stack[pScope->top][x]);
			
				free(pScope->stack[pScope->top][x]);
				pScope->stack[pScope->top][x] = NULL;
			}
		}
		
		free(pScope->stack[pScope->top]);
		pScope->stack[pScope->top] = NULL;
		
		pScope->top--;
	}
}

int scopeFind(Scope* pScope, void* pKey, uint32_t keysize, void* pData, uint32_t* datasize, int bOnlyTop)
{
	int retValue = -1; // non trovato
	
	HashTable_t** pHT = NULL;
	int x;

	for ( x = pScope->top; x >= 0; x-- )
	{
		pHT = pScope->stack[x];
		retValue = htFind(*pHT, pKey, keysize, pData, datasize);
		if ( retValue >= 0 )
			return retValue;
		
		if ( bOnlyTop )
			break;
	}

	return retValue;
}

void scopeTraverse(Scope* pScope, pfnOnTraverse OnTraverse, int bOnlyTop)
{
	HashTable_t** pHT = NULL;
	int x;

	for ( x = pScope->top; x >= 0; x-- )
	{
		pHT = pScope->stack[x];
		
		htTraverse(*pHT, OnTraverse);
				
		if ( bOnlyTop )
			break;
	}
}

int scopeInsert(Scope* pScope, void* pKey, uint32_t keysize, void* pData, uint32_t datasize)
{
	HashTable_t** pHT = NULL;
	HashTable_t Dati;
	uint32_t sizeDati;

	pHT = pScope->stack[pScope->top];
	
	if ( htFind(*pHT, pKey, keysize, &Dati, &sizeDati) >= 0 )
	{
		printf("Errore scopeInsert: la variabile '%s' e' gia' stata dichiarata nello scope %d\n", (char*)pKey, pScope->top);
		return 0;
	}

	htInsert(*pHT, pKey, keysize, pData, datasize);

	return 1;
}

int scopeUpdateValue(Scope* pScope, const void* pKey, uint32_t keysize, const void* pData, uint32_t datasize, int bOnlyTop)
{
	HashTable_t** pHT = NULL;
	
	int x;
	int index;

	for ( x = pScope->top; x >= 0; x-- )
	{
		pHT = pScope->stack[x];

		index = htUpdateData(*pHT, pKey, keysize, pData, datasize);
		
		if ( index >= 0 )
			return 1;
		
		if ( bOnlyTop )
			break;
	}

	printf("ERRORE scopeUpdateValue: variabile '%s' non presente nello scope\n", (char*)pKey);
	
	return 0;
}

// ----------------------------------------------------- End Scope Functions -----------------------------------------------------------
