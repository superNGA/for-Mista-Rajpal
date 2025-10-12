#include "Heap.h"
#include <stdbool.h>
#include <malloc.h>
#include <string.h>


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static inline void* GetAdrsAtIndex(void* pFirstElement, int iIndex, size_t iSize)
{
    return (void*)((uintptr_t)(pFirstElement) + (iIndex * iSize));
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static inline void Swap(void* pFirst, void* pSecond, void* pTemp, size_t iSize)
{
    memcpy(pTemp,   pFirst,  iSize);
    memcpy(pFirst,  pSecond, iSize);
    memcpy(pSecond, pTemp,   iSize);
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void Heapify(void* pData, size_t iElementSize, int nElements, T_Comparision pComparisionFn)
{
    int iStart = (nElements / 2) - 1;
    void* pTempVar = malloc(iElementSize);
    for(int iIndex = iStart; iIndex >= 0; iIndex--)
    {
        HeapifyFrom(pData, iElementSize, iIndex, nElements, pComparisionFn, pTempVar);
    }
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void HeapifyFrom(void* pData, size_t iElementSize, int iParentIndex, int nElements, T_Comparision pComparisionFn, void* pTempVar)
{
    int iLeftChild = (iParentIndex * 2) + 1;
    int iRightChild = (iParentIndex * 2) + 2;

    int iLargestNodeIndex = iParentIndex;

    // Is left child greater than parent ?
    if (iLeftChild < nElements)
    {
        if (pComparisionFn(GetAdrsAtIndex(pData, iParentIndex, iElementSize), GetAdrsAtIndex(pData, iLeftChild, iElementSize)) == false)
        {
            iLargestNodeIndex = iLeftChild;
        }
    }

    // Is right child greater than parent ?
    if (iRightChild < nElements)
    {
        if (pComparisionFn(GetAdrsAtIndex(pData, iLargestNodeIndex, iElementSize), GetAdrsAtIndex(pData, iRightChild, iElementSize)) == false)
        {
            iLargestNodeIndex = iRightChild;
        }
    }


    // No swapping needed
    if (iLargestNodeIndex == iParentIndex)
        return;

    // Swap
    Swap(GetAdrsAtIndex(pData, iLargestNodeIndex, iElementSize), GetAdrsAtIndex(pData, iParentIndex, iElementSize), pTempVar, iElementSize);

    HeapifyFrom(pData, iElementSize, iLargestNodeIndex, nElements, pComparisionFn, pTempVar);
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int Pop(void* pData, size_t iElementSize, int iRootIndex, int iLastIndex, T_Comparision pComparisionFn, void* pDataOut)
{
    if (pData == NULL || pDataOut == NULL || pComparisionFn == NULL)
        return -1;

    if (iRootIndex > iLastIndex)
        return -1;

    // Copy Poped data.
    memcpy(pDataOut, GetAdrsAtIndex(pData, iRootIndex, iElementSize), iElementSize);

    void* pTempVar = malloc(iElementSize);

    if (iLastIndex != iRootIndex)
        memcpy(GetAdrsAtIndex(pData, iRootIndex, iElementSize), GetAdrsAtIndex(pData, iLastIndex, iElementSize), iElementSize);

    // NOTE : for iLastIndex = nElements - 1 here. thats what we want.
    HeapifyFrom(pData, iElementSize, iRootIndex, iLastIndex, pComparisionFn, pTempVar);

    return iLastIndex - 1;
}
