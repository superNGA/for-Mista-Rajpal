#include "Heap.h"
#include <stdbool.h>
#include <malloc.h>


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void Heapify(T* pData, int nElements, T_Comparision pComparisionFn)
{
    int iStart = (nElements / 2) - 1; 
    for(int iIndex = iStart; iIndex >= 0; iIndex--)
    {
        HeapifyFrom(pData, iIndex, nElements, pComparisionFn);
    }
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void HeapifyFrom(T* pData, int iParentIndex, int nElements, T_Comparision pComparisionFn)
{
    int iLeftChild  = (iParentIndex * 2) + 1;
    int iRightChild = (iParentIndex * 2) + 2;

    int iLargestNodeIndex = iParentIndex;

    // Is left child greater than parent ?
    if(iLeftChild < nElements)
    {
        if(pComparisionFn(&pData[iParentIndex], &pData[iLeftChild]) == false)
        {
            iLargestNodeIndex = iLeftChild;
        }
    }

    // Is right child greater than parent ?
    if(iRightChild < nElements)
    {
        if(pComparisionFn(&pData[iLargestNodeIndex], &pData[iRightChild]) == false)
        {
            iLargestNodeIndex = iRightChild;
        }
    }


    // No swapping needed
    if(iLargestNodeIndex == iParentIndex)
        return;

    // Swap
    T temp                   = pData[iParentIndex];
    pData[iParentIndex]      = pData[iLargestNodeIndex];
    pData[iLargestNodeIndex] = temp;

    HeapifyFrom(pData, iLargestNodeIndex, nElements, pComparisionFn);
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int Pop(T* pData, int iRootIndex, int iLastIndex, T_Comparision pComparisionFn, T* pDataOut)
{
    if(pData == NULL || pDataOut == NULL || pComparisionFn == NULL)
        return -1;

    if(iRootIndex > iLastIndex)
        return -1;

    // Copy Poped data.
    *pDataOut = pData[iRootIndex];

    if(iLastIndex != iRootIndex) 
        pData[iRootIndex] = pData[iLastIndex];

    // NOTE : for iLastIndex = nElements - 1 here. thats what we want.
    HeapifyFrom(pData, iRootIndex, iLastIndex, pComparisionFn); 

    return iLastIndex - 1;
}
