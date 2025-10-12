#pragma once
#include <stdint.h>
#include <stdbool.h>


///////////////////////////////////////////////////////////////////////////
typedef struct Deque_t
{
    uint64_t m_iDataSize;   // size of data we wanna make the deque of.
    void*    m_pMemory;     // data will be stored here.
    int32_t  m_iStartIndex; // first element
    int32_t  m_iEndIndex;   // last element
    uint32_t m_nElements;   // Number of elements currently stored.
    uint32_t m_bInitkey;    // set once initialized
}Deque_t;
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
bool UtilDeque_InitDeque(Deque_t* pQ, uint64_t iDataSize);

bool UtilDeque_PopFront(Deque_t* pQ, void* pDataOut);
bool UtilDeque_PushFront(Deque_t* pQ, void* pData);
bool UtilDeque_PopBack(Deque_t* pQ, void* pDataOut);
bool UtilDeque_PushBack(Deque_t* pQ, void* pData);
