#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>


#define T Test_t 


typedef struct Test_t
{
    int m_iData;
}Test_t;


// Template for Comparision function.
typedef bool (* T_Comparision)(T*, T*);

void Heapify(T* pData, int nElements, T_Comparision pComparisionFn);
void HeapifyFrom(T* pData, int iParentIndex, int nElements, T_Comparision pComparisionFn);
bool OneGreaterThanTwo(T* pData1, T* pData2)
{
    return pData1->m_iData >= pData2->m_iData;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    T   data[10] = {{1}, {3}, {543}, {12}, {45}, {1}, {3}, {543}, {12}, {45}};
    int nElements = sizeof(data) / sizeof(T);
    
    for(int i = 0; i < nElements; i++) printf("%d, ", data[i].m_iData);
    printf("\n\n");

    Heapify(data, nElements, OneGreaterThanTwo);

    for(int i = 0; i < nElements; i++) printf("%d, ", data[i].m_iData);

    return 0;
}


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
