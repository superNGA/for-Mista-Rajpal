#include "Deque.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>

#define UTIL_DEQUE_DEFAULT_BLOCK_SIZE 64ui64
#define UTIL_DEQUE_INIT_KEY           0xDEADC0DEui32


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static inline bool IsDequeInitialized(Deque_t* pQ)
{
    // NOTE : if all of these matrices are valid, then this deque is probably initialized.
    if (pQ->m_bInitkey != UTIL_DEQUE_INIT_KEY)
        return false;

    if (pQ->m_pMemory == NULL)
        return false;

    if (pQ->m_iDataSize == 0 || pQ->m_iDataSize == UINT64_MAX)
        return false;

    if (pQ->m_iStartIndex < 0 || pQ->m_iEndIndex < 0)
        return false;

    printf("Deque is initialized\n");
    return true;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
bool UtilDeque_InitDeque(Deque_t* pQ, uint64_t iDataSize)
{
    if (pQ == NULL || iDataSize == 0 || iDataSize == UINT64_MAX)
        return false;

    // data size
    pQ->m_iDataSize = iDataSize;

    // Allocating memory
    uint64_t iDefaultSize = UTIL_DEQUE_DEFAULT_BLOCK_SIZE;
    pQ->m_pMemory         = malloc(iDataSize * iDefaultSize);
    
    if (pQ->m_pMemory == NULL)
        return false;
    memset(pQ->m_pMemory, 0, iDataSize * iDefaultSize);

    // Start & end indexes are in the center for better push pop times.
    pQ->m_iStartIndex = iDefaultSize / 2;
    pQ->m_iEndIndex   = iDefaultSize / 2;
    pQ->m_nElements   = 0;
    pQ->m_bInitkey    = UTIL_DEQUE_INIT_KEY;

    return true;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
bool UtilDeque_PopFront(Deque_t* pQ, void* pDataOut)
{
    if (IsDequeInitialized(pQ) == false)
        return false;

    if (pQ->m_nElements == 0)
        return false;

    memcpy(pDataOut, (uintptr_t)(pQ->m_pMemory) + ((uintptr_t)(pQ->m_iStartIndex) + pQ->m_iDataSize), pQ->m_iDataSize);
    pQ->m_iStartIndex += 1; // move towards end
    pQ->m_nElements   -= 1;

    return true;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
bool UtilDeque_PushFront(Deque_t* pQ, void* pData)
{
    if (IsDequeInitialized(pQ) == false)
        return false;

    void* pTargetAdrs = (void*)((uintptr_t)(pQ->m_pMemory) + ((uintptr_t)(pQ->m_iStartIndex) * pQ->m_iDataSize));
    pTargetAdrs       = (void*)((uintptr_t)(pTargetAdrs)-(uintptr_t)(pQ->m_iDataSize));

    memcpy(pTargetAdrs, pData, pQ->m_iDataSize);
    pQ->m_iStartIndex -= 1; // move towards start
    pQ->m_nElements   += 1;

    printf("Successfully pushed @ front. Start index : %d, elements : %lu @ adrs : %p\n", pQ->m_iStartIndex, pQ->m_nElements, pTargetAdrs);
    return true;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
bool UtilDeque_PopBack(Deque_t* pQ, void* pDataOut)
{
    if (IsDequeInitialized(pQ) == false)
        return false;

    if (pQ->m_nElements == 0)
        return false;

    memcpy(pDataOut, (uintptr_t)(pQ->m_pMemory) + (uintptr_t)(pQ->m_iEndIndex + pQ->m_iDataSize), pQ->m_iDataSize);
    pQ->m_iEndIndex -= 1; // move towards start
    pQ->m_nElements -= 1;

    return true;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
bool UtilDeque_PushBack(Deque_t* pQ, void* pData)
{
    if (IsDequeInitialized(pQ) == false)
        return false;

    void* pTargetAdrs = (void*)((uintptr_t)(pQ->m_pMemory) + ((uintptr_t)(pQ->m_iEndIndex) * pQ->m_iDataSize));
    pTargetAdrs       = (void*)((uintptr_t)(pTargetAdrs)+(uintptr_t)(pQ->m_iDataSize));

    memcpy(pTargetAdrs, pData, pQ->m_iDataSize);
    pQ->m_iEndIndex += 1; // move towards end
    pQ->m_nElements += 1;

    printf("Successfully pushed @ front. Start index : %d, elements : %lu @ adrs : %p\n", pQ->m_iStartIndex, pQ->m_nElements, pTargetAdrs);
    return true;
}
