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

#ifndef	__MY_SCOPEHASHTABLE_H
#define	__MY_SCOPEHASHTABLE_H

#ifdef __cplusplus
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS   }
#else /* !__cplusplus */
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif /* __cplusplus */

/*
#define HT_SIZE 251
#define HT_SIZE 509
#define HT_SIZE 1021
#define HT_SIZE 2039
#define HT_SIZE 4093
#define HT_SIZE 8191
#define HT_SIZE 16381
#define HT_SIZE 32749
*/
#define HT_SIZE 251

//#define SCOPE_SIZE 4096
//#define SCOPE_SIZE 1024
#define SCOPE_SIZE 32

#define BLOCK_SIZE 4096

#define UNUSED(x) (void)(x)

#include <stdint.h>

typedef struct tagHashTable* HTptr;

typedef int (*pfnHashFunc)(HTptr p, const void* pKey, uint32_t keysize);

typedef int (*pfnCompareFunc)(const void* pKey1, uint32_t keysize1, const void* pKey2, uint32_t keysize2);

typedef int (*pfnOnTraverse)(const void* pKey, uint32_t keysize, void* pData, uint32_t dataSize);


typedef struct tagHashTableItem
{
	void*    pKey;
	uint32_t keysize;
	void*    pData;
	uint32_t datasize;
	struct tagHashTableItem *next;
} HashTableItem_t;

typedef struct tagHashTable
{
	HashTableItem_t** pHashTable;
	uint32_t          htSize;
	pfnHashFunc       HashFunc;
	pfnCompareFunc    CompareFunc;
} HashTable_t;

typedef struct tagScope
{
	int top;
	HashTable_t** stack[SCOPE_SIZE];
	uint32_t          htSize;
	pfnHashFunc       HashFunc;
	pfnCompareFunc    CompareFunc;	
} Scope;

BEGIN_C_DECLS

// ----------------------------------------------- HashTable Functions -------------------------------------------------

int  htInit(HashTable_t* p, uint32_t htSize, pfnHashFunc HashFunc, pfnCompareFunc CompareFunc);
int  htSetHashFunc(HashTable_t* p, pfnHashFunc HashFunc);
int  htSetCompareFunc(HashTable_t* p, pfnCompareFunc CompareFunc);
int  htFind(HashTable_t* p, const void* pKey, uint32_t keysize, void* pData, uint32_t* datasize);
int  htUpdateData(HashTable_t* p, const void* pKey, uint32_t keysize, const void* pData, uint32_t datasize);
int  htDeleteKey(HashTable_t* p, const void* pKey, uint32_t keysize);
int  htInsert(HashTable_t* p, const void* pKey, uint32_t keysize, const void* pData, uint32_t datasize);
void htFree(HashTable_t* p);
void htTraverse(HashTable_t* p, pfnOnTraverse OnTraverse);

HashTableItem_t* htNewNode(const void* pKey, uint32_t keysize, const void* pData, uint32_t datasize);
void htFreeList(HashTableItem_t* first);

//int StringHashFunc(HashTable_t* p, const void* pKey, uint32_t keysize);
//int StringCompareFunc(const void* pKey1, uint32_t keysize1, const void* pKey2, uint32_t keysize2);

// ----------------------------------------------- Scope Functions -------------------------------------------------

int  scopeInit(Scope* pScope, uint32_t htSize, pfnHashFunc HashFunc, pfnCompareFunc CompareFunc);
void scopeFree(Scope* pScope);
int  scopePush(Scope *pScope);
int  scopePop(Scope* pScope);
int  scopeFind(Scope* pScope, void* pKey, uint32_t keysize, void* pData, uint32_t* datasize, int bOnlyTop);
int  scopeInsert(Scope* pScope, void* pKey, uint32_t keysize, void* pData, uint32_t datasize);
int  scopeUpdateValue(Scope* pScope, const void* pKey, uint32_t keysize, const void* pData, uint32_t datasize, int bOnlyTop);
void scopeTraverse(Scope* pScope, pfnOnTraverse OnTraverse, int bOnlyTop);

END_C_DECLS

#endif /* __MY_SCOPEHASHTABLE_H */
