//=========================================================================
//                      UTILITY DEQUE
//=========================================================================
// by      : Linus Torvalds wannabe
// created : 09/11/2025
// 
// purpose : Simple std::deque rip-off in C.
//-------------------------------------------------------------------------
#pragma once
#include <assert.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>


// Enable debugging logs.
#define UTILDEQUE_DEBUG 1
#if (UTILDEQUE_DEBUG != 0)

    #include <stdio.h>
    #define UTILDEQUE_LOG(...) printf(__VA_ARGS__);

#else

    #define UTILDEQUE_LOG(...) (void)(0);

#endif


#define UTILDEQUE_BLOCK_SIZE    64LLU
#define UTILDEQUE_BLOCKMAP_SIZE  8LLU



///////////////////////////////////////////////////////////////////////////
typedef struct UtilDequeBlock_t
{
    struct UtilDequeBlock_t* m_pNext;
    struct UtilDequeBlock_t* m_pPrev;
    int64_t m_iIndex;

    void*   m_pData;

} UtilDequeBlock_t;
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
typedef struct UtilDequeCore_t
{
    // NOTE : m_iFrontIndex & m_iEndIndex are relative indecies from the center of the block map.
    // For example, 0 means 0th element in the middle block ( block index = block map size / 2 )
    // and negative -1 means very last element in the block just before the middle most block.
    // 
    // Block size increase in multiple of 2 and start from 8 ( or whatever constant is set on top of
    // this header.
    // 
    // m_iFrontIndex & m_iEndIndex also tell at what relative index do we need to store the next pushFront
    // or pushBack data respectively, that means element before these indicies store the last data ( i.e.
    // m_iFrontIndex + 1 as it grows in negative direction and m_iEndIndex - 1 as it grows in positive direction.).
    //

    int64_t m_iFrontIndex;
    int64_t m_iBackIndex;


    // NOTE : m_blockMap is an array of block pointers. It grows in outerward direction from center.
    // At initialization, we allocate 2 blocks to it and m_iMapStart & m_iMapEnd are setup.
    // m_iMapStart & m_iMapEnd are expected to have valid block pointers.
    //
    UtilDequeBlock_t** m_blockMap;
    uint64_t           m_iBlockMapSize;
    int32_t m_iMapStart, m_iMapEnd;

    uint64_t m_iElementSize;

} UtilDequeCore_t;
///////////////////////////////////////////////////////////////////////////


////////////////////////  FN Declaration  /////////////////////////////////
static inline bool    UtilDequeCore_Init                        (UtilDequeCore_t* pQ, size_t iElementSize);
static inline void    UtilDequeCore_Destroy                     (UtilDequeCore_t* pQ);
static inline void*   UtilDequeCore_AllocateBlock               (uint64_t iElementSize);
static inline void    UtilDequeCore_FreeBlock                   (UtilDequeBlock_t* pBlock);

static inline int     FloorDivInt                               (int a, int b);
static inline bool    UtilDequeCore_DoubleBlockMap              (UtilDequeCore_t* pQ);
static inline bool    UtilDequeCore_AssertBlockForIndexRelative (UtilDequeCore_t* pQ, int iIndexRelative);
static inline void*   UtilDequeCore_GetDataAtIndexRelative      (UtilDequeCore_t* pQ, int iIndexRelative);

static inline int64_t UtilDequeCore_Size                        (UtilDequeCore_t* pQ);
static inline int64_t UtilDequeCore_Capacity                    (UtilDequeCore_t* pQ);

// Pushing & Poppin'
static inline void    UtilDequeCore_PushBack                    (UtilDequeCore_t* pQ, void* data);
static inline void*   UtilDequeCore_Back                        (UtilDequeCore_t* pQ);
static inline void    UtilDequeCore_PopBack                     (UtilDequeCore_t* pQ);
static inline void    UtilDequeCore_PushFront                   (UtilDequeCore_t* pQ, void* data);
static inline void*   UtilDequeCore_Front                       (UtilDequeCore_t* pQ);
static inline void    UtilDequeCore_PopFront                    (UtilDequeCore_t* pQ);
///////////////////////////////////////////////////////////////////////////



/////////////////////  WRAPPERS  //////////////////////////////////////////
#define UTILDEQUE_DECLARE_TYPE(type, Tag)\
typedef struct {UtilDequeCore_t m_core;} UtilDeque##Tag##_t;\
static inline bool    UtilDeque##Tag##_Init      (UtilDeque##Tag##_t* pQ)             { return UtilDequeCore_Init(&pQ->m_core, sizeof(type));}\
static inline void    UtilDeque##Tag##_Destroy   (UtilDeque##Tag##_t* pQ)             { UtilDequeCore_Destroy(&pQ->m_core);                  }\
static inline int64_t UtilDeque##Tag##_Size      (UtilDeque##Tag##_t* pQ)             { return UtilDequeCore_Size(&pQ->m_core);              }\
static inline int64_t UtilDeque##Tag##_Capacity  (UtilDeque##Tag##_t* pQ)             { return UtilDequeCore_Capacity(&pQ->m_core);          }\
static inline void    UtilDeque##Tag##_PushBackP (UtilDeque##Tag##_t* pQ, type* data) { UtilDequeCore_PushBack(&pQ->m_core, data);           }\
static inline void    UtilDeque##Tag##_PushBack  (UtilDeque##Tag##_t* pQ, type data)  { UtilDequeCore_PushBack(&pQ->m_core, &data);          }\
static inline type*   UtilDeque##Tag##_Back      (UtilDeque##Tag##_t* pQ)             { return UtilDequeCore_Back(&pQ->m_core);              }\
static inline void    UtilDeque##Tag##_PopBack   (UtilDeque##Tag##_t* pQ)             { UtilDequeCore_PopBack(&pQ->m_core);                  }\
static inline void    UtilDeque##Tag##_PushFrontP(UtilDeque##Tag##_t* pQ, type* data) { UtilDequeCore_PushFront(&pQ->m_core, data);          }\
static inline void    UtilDeque##Tag##_PushFront (UtilDeque##Tag##_t* pQ, type data)  { UtilDequeCore_PushFront(&pQ->m_core, &data);         }\
static inline type*   UtilDeque##Tag##_Front     (UtilDeque##Tag##_t* pQ)             { return UtilDequeCore_Front(&pQ->m_core);             }\
static inline void    UtilDeque##Tag##_PopFront  (UtilDeque##Tag##_t* pQ)             { UtilDequeCore_PopFront(&pQ->m_core);                 }
///////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static inline bool UtilDequeCore_Init(UtilDequeCore_t* pQ, size_t iElementSize)
{
    pQ->m_iBlockMapSize = UTILDEQUE_BLOCKMAP_SIZE;
    pQ->m_iElementSize  = iElementSize;

    
    // Front Block...
    UtilDequeBlock_t* pFrontBlock = NULL;
    {
        pFrontBlock = UtilDequeCore_AllocateBlock(pQ->m_iElementSize);
        if (pFrontBlock == NULL)
            return false;

        pFrontBlock->m_iIndex = -1LL;
        pQ->m_iFrontIndex     = -1LL;
    }


    // Back Block...
    UtilDequeBlock_t* pBackBlock = NULL;
    {
        pBackBlock = UtilDequeCore_AllocateBlock(pQ->m_iElementSize);
        if (pBackBlock == NULL)
        {
            UtilDequeCore_FreeBlock(pFrontBlock);
            return false;
        }

        pBackBlock->m_iIndex = 0LL;
        pQ->m_iBackIndex     = 0LL;
    }


    // Initializing block map.
    {
        pQ->m_blockMap = (UtilDequeBlock_t**)malloc(sizeof(UtilDequeBlock_t*) * pQ->m_iBlockMapSize);
        if (pQ->m_blockMap == NULL)
        {
            UtilDequeCore_FreeBlock(pFrontBlock);
            UtilDequeCore_FreeBlock(pBackBlock);
            return false;
        }
        memset(pQ->m_blockMap, 0, pQ->m_iBlockMapSize * sizeof(UtilDequeBlock_t*));
    }

    
    // Initializing start & end index for block map.
    pQ->m_iMapEnd   =  pQ->m_iBlockMapSize / 2LLU;
    pQ->m_iMapStart = (pQ->m_iBlockMapSize / 2LLU) - 1LLU;
    
    pQ->m_blockMap[pQ->m_iMapStart] = pFrontBlock;
    pQ->m_blockMap[pQ->m_iMapEnd]   = pBackBlock;

    return true;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
inline void UtilDequeCore_Destroy(UtilDequeCore_t* pQ)
{
    if (pQ->m_blockMap == NULL)
        return;

    for (uint64_t i = 0; i < pQ->m_iBlockMapSize; i++)
    {
        if (pQ->m_blockMap[i] != NULL)
        {
            UtilDequeCore_FreeBlock(&pQ->m_blockMap[i]);
        }
    }

    free(pQ->m_blockMap);
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
inline void* UtilDequeCore_AllocateBlock(uint64_t iElementSize)
{
    // Allocate block & clear it.
    UtilDequeBlock_t* pBlock = (UtilDequeBlock_t*)malloc(sizeof(UtilDequeBlock_t));
    if (pBlock == NULL)
    {
        UTILDEQUE_LOG("Failed allocatoin for block\n");
        return NULL;
    }
    memset(pBlock, 0, sizeof(UtilDequeBlock_t));


    // Allocate memory for data in that block.
    size_t iSize = iElementSize * UTILDEQUE_BLOCK_SIZE;
    pBlock->m_pData = malloc(iSize);
    if (pBlock->m_pData == NULL)
    {
        UTILDEQUE_LOG("Failed allocatoin for block data.\n");
        return NULL;
    }
    memset(pBlock->m_pData, 0, iSize);


    return pBlock;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
inline void UtilDequeCore_FreeBlock(UtilDequeBlock_t* pBlock)
{
    if (pBlock == NULL)
        return;

    if (pBlock->m_pData != NULL)
    {
        free(pBlock->m_pData);
    }

    free(pBlock);
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static inline bool UtilDequeCore_DoubleBlockMap(UtilDequeCore_t* pQ)
{
    // Old index & the new desired index.
    uint64_t iOldSize = pQ->m_iBlockMapSize;
    uint64_t iNewSize = pQ->m_iBlockMapSize * 2LLU;


    // Allocate memory.
    UtilDequeBlock_t** pNewBlockMap = (UtilDequeBlock_t**)realloc(pQ->m_blockMap, sizeof(UtilDequeBlock_t*) * iNewSize);
    if (pNewBlockMap == NULL)
        return false;

    // Update block size & pointer.
    pQ->m_iBlockMapSize = iNewSize;
    pQ->m_blockMap      = pNewBlockMap;


    // Now we shift all elements by some offset, 
    // so the elements get middle of the new array.
    int iOffset = (iNewSize - iOldSize) / 2LLU;
    assert(iOffset > 0 && "offset is <= 0 ! Very BAD!!!");

    for (int i = pQ->m_iMapEnd; i >= pQ->m_iMapStart; i--)
    {
        pQ->m_blockMap[i + iOffset] = pQ->m_blockMap[i];
    }

    // No we update the start & end index for the block array.
    pQ->m_iMapStart += iOffset;
    pQ->m_iMapEnd   += iOffset;

    // Clean up is important.
    for (int i = 0; i < pQ->m_iBlockMapSize; i++)
    {
        if(i < pQ->m_iMapStart || i > pQ->m_iMapEnd)
        {
            pQ->m_blockMap[i] = NULL;
        }
    }

    return true;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static inline int FloorDivInt(int a, int b) 
{
    int q = a / b;
    int r = a % b;
    if (r != 0 && ((r < 0) != (b < 0)))  // remainder exists and signs differ
        q--;
    return q;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static inline int64_t UtilDequeCore_Size(UtilDequeCore_t* pQ)
{
    return (pQ->m_iBackIndex - pQ->m_iFrontIndex) - 1LL;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
inline int64_t UtilDequeCore_Capacity(UtilDequeCore_t* pQ)
{
    return (int64_t)(pQ->m_iMapEnd - pQ->m_iMapStart + 1LL) * UTILDEQUE_BLOCK_SIZE;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static inline bool UtilDequeCore_AssertBlockForIndexRelative(UtilDequeCore_t* pQ, int iIndexRelative)
{
    assert(pQ->m_blockMap != NULL && "Block map null!!");


    // Block index relative
    int iBlockIndexRelative = FloorDivInt(iIndexRelative, UTILDEQUE_BLOCK_SIZE);
    int iBlockIndexAbs      = (pQ->m_iBlockMapSize / 2LLU) + iBlockIndexRelative;


    // Check if block index is going out of bound. In which case we would need to 
    // allocate more space to our block map / array.
    if (iBlockIndexAbs >= pQ->m_iBlockMapSize || iBlockIndexAbs < 0)
    {
        if (UtilDequeCore_DoubleBlockMap(pQ) == true)
        {
            UTILDEQUE_LOG("Allocated more space to block array. [ Block Map Size : %llu, Start : %d, End : %d ]\n", 
                pQ->m_iBlockMapSize, pQ->m_iMapStart, pQ->m_iMapEnd);

            for (int i = 0; i < pQ->m_iBlockMapSize; i++)
                UTILDEQUE_LOG("[ %p ], ", pQ->m_blockMap[i]);
            UTILDEQUE_LOG("\n");
            
            // Since we updated the block map, we also need to recalculate the abs index. ( relative index is fine. )
            iBlockIndexAbs = (pQ->m_iBlockMapSize / 2LLU) + iBlockIndexRelative;
        }
        else
        {
            UTILDEQUE_LOG("Failed to double map size.\n");
            return false;
        }
    }


    UtilDequeBlock_t* pBlock = pQ->m_blockMap[iBlockIndexAbs];
    if (pBlock != NULL) // Block already ready to go.
        return true;

    // Just a sanity check, if the blockindex is even correct or not.
    assert((iBlockIndexAbs > pQ->m_iMapEnd || iBlockIndexAbs < pQ->m_iMapStart) &&
        "Block we just allocated was for an index that we already have a block for. Wtf is going on here again!?");


    UtilDequeBlock_t* pNewBlock = UtilDequeCore_AllocateBlock(pQ->m_iElementSize);
    if (pNewBlock == NULL)
        return false;

    pQ->m_blockMap[iBlockIndexAbs] = pNewBlock;


    // Update the map index.
    if (iBlockIndexAbs > pQ->m_iMapEnd)
    {
        pQ->m_iMapEnd++;
        UTILDEQUE_LOG("Added new block to the end. New block count : %d\n", pQ->m_iMapEnd - pQ->m_iMapStart + 1);
    }
    else if (iBlockIndexAbs < pQ->m_iMapStart)
    {
        pQ->m_iMapStart--;
        UTILDEQUE_LOG("Added new block to the start. New block count : %d\n", pQ->m_iMapEnd - pQ->m_iMapStart + 1);
    }

    return true;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static inline void* UtilDequeCore_GetDataAtIndexRelative(UtilDequeCore_t* pQ, int iIndexRelative)
{
    // Block index relative
    int iBlockIndexRelative = FloorDivInt(iIndexRelative, UTILDEQUE_BLOCK_SIZE);
    int iBlockIndexAbs      = (pQ->m_iBlockMapSize / 2LLU) + iBlockIndexRelative;

    UtilDequeBlock_t* pBlock = pQ->m_blockMap[iBlockIndexAbs];
    if (pBlock == NULL)
        return NULL;

    int iElementIndexRelative = iIndexRelative % UTILDEQUE_BLOCK_SIZE;
    if (iElementIndexRelative < 0)
        iElementIndexRelative = (int32_t)UTILDEQUE_BLOCK_SIZE + iElementIndexRelative;

    return (void*)((uintptr_t)pBlock->m_pData + (pQ->m_iElementSize * (uint64_t)iElementIndexRelative));
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static inline void UtilDequeCore_PushBack(UtilDequeCore_t* pQ, void* data)
{
    // Monkey check.
    assert(pQ->m_blockMap != NULL && "Deque not initialized!");


    // Make sure this block exists.
    if (UtilDequeCore_AssertBlockForIndexRelative(pQ, pQ->m_iBackIndex) == false)
    {
        UTILDEQUE_LOG("Failed pushback!\n");
        return;
    }


    // Get this block.
    void* pDestination = UtilDequeCore_GetDataAtIndexRelative(pQ, pQ->m_iBackIndex);
    assert(pDestination != NULL && "Bad destination pointer for pushback!");
    if (pDestination == NULL)
    {
        UTILDEQUE_LOG("Failed to pushback due to bad destination pointer!\n");
        return;
    }


    memcpy(pDestination, data, pQ->m_iElementSize);
    pQ->m_iBackIndex++;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static inline void* UtilDequeCore_Back(UtilDequeCore_t* pQ)
{
    // Monkey check.
    assert(pQ->m_blockMap != NULL && "Deque not initialized!");


    // No elements ?
    if (UtilDequeCore_Size(pQ) <= 0LL)
        return NULL;


    // Make sure this block exists.
    if (UtilDequeCore_AssertBlockForIndexRelative(pQ, pQ->m_iBackIndex - 1LL) == false)
    {
        UTILDEQUE_LOG("Failed pushback!\n");
        return NULL;
    }


    // Get this block.
    return UtilDequeCore_GetDataAtIndexRelative(pQ, pQ->m_iBackIndex - 1LL);
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static inline void UtilDequeCore_PopBack(UtilDequeCore_t* pQ)
{
    if (UtilDequeCore_Size(pQ) > 0Ll)
        pQ->m_iBackIndex--;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
inline void UtilDequeCore_PushFront(UtilDequeCore_t* pQ, void* data)
{
    // Monkey check.
    assert(pQ->m_blockMap != NULL && "Deque not initialized!");


    // Make sure this block exists.
    if (UtilDequeCore_AssertBlockForIndexRelative(pQ, pQ->m_iFrontIndex) == false)
    {
        UTILDEQUE_LOG("Failed pushback!\n");
        return;
    }


    // Get this block.
    void* pDestination = UtilDequeCore_GetDataAtIndexRelative(pQ, pQ->m_iFrontIndex);
    assert(pDestination != NULL && "Bad destination pointer for pushback!");
    if (pDestination == NULL)
    {
        UTILDEQUE_LOG("Failed to pushback due to bad destination pointer!\n");
        return;
    }


    memcpy(pDestination, data, pQ->m_iElementSize);
    pQ->m_iFrontIndex--;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
inline void* UtilDequeCore_Front(UtilDequeCore_t* pQ)
{
    // Monkey check.
    assert(pQ->m_blockMap != NULL && "Deque not initialized!");


    // No elements ?
    if (UtilDequeCore_Size(pQ) <= 0LL)
        return NULL;


    // Make sure this block exists.
    if (UtilDequeCore_AssertBlockForIndexRelative(pQ, pQ->m_iFrontIndex + 1LL) == false)
    {
        UTILDEQUE_LOG("Failed Front()!\n");
        return NULL;
    }


    // Get this block.
    return UtilDeque_GetDataAtIndexRelative(pQ, pQ->m_iFrontIndex + 1LL);
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
inline void UtilDequeCore_PopFront(UtilDequeCore_t* pQ)
{
    if (UtilDequeCore_Size(pQ) > 0Ll)
        pQ->m_iFrontIndex++;
}
