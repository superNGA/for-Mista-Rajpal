#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


// Data
#define T int32_t


typedef struct Node_t
{
    T   m_data;
    int m_iParentIndex; // For parent nodes, parent index is negative, indicating height of set.
} Node_t;


Node_t       g_data[10];
unsigned int nData = sizeof(g_data) / sizeof(Node_t);


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
inline void AssertValidIndex(int iIndex, const char* szFuncName)
{
    if(iIndex >= nData || iIndex < 0)
    {
        printf("[%s] Trying to access index [ %d / %d ]SegFault!", szFuncName, iIndex, nData - 1);
        exit(1);
    }
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int MakeSet(int iIndex)
{
    AssertValidIndex(iIndex, __FUNCTION__); 

    if(iIndex >= nData)
        exit(1);

    g_data[iIndex].m_iParentIndex = -1;
    return iIndex;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int FindParent(int iIndex)
{
    AssertValidIndex(iIndex, __FUNCTION__); 

    if(g_data[iIndex].m_iParentIndex < 0)
        return iIndex;
    else
        return FindParent(g_data[iIndex].m_iParentIndex);
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int Union(int iParentNodeIndex1, int iParentNodeIndex2)
{
    AssertValidIndex(iParentNodeIndex1, __FUNCTION__); AssertValidIndex(iParentNodeIndex2, __FUNCTION__);

    int iHeightSet1 = abs(g_data[iParentNodeIndex1].m_iParentIndex);
    int iHeightSet2 = abs(g_data[iParentNodeIndex2].m_iParentIndex);

    if(iHeightSet1 > iHeightSet2)
    {
        g_data[iParentNodeIndex2].m_iParentIndex = iParentNodeIndex1;
        return iParentNodeIndex1;
    }
    else if(iHeightSet2 > iHeightSet1)
    {
        g_data[iParentNodeIndex1].m_iParentIndex = iParentNodeIndex2;
        return iParentNodeIndex2;
    }
    else 
    {
        g_data[iParentNodeIndex2].m_iParentIndex = iParentNodeIndex1;
        g_data[iParentNodeIndex1].m_iParentIndex -= 1; // Increment height
        return iParentNodeIndex1;
    }
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    memset((void*)g_data, 0, sizeof(g_data));

    for(int i = 0; i < nData; i++) MakeSet(i);

    Union(0, 2);
    Union(0, 3);
    Union(0, 6);
    Union(0, 1);

    for(int i = 0; i < nData; i++) printf("%d, ", g_data[i].m_iParentIndex);
    printf("\n\n");

    for(int i = 0; i < nData; i++)
    {
        printf("Parent for index [ %d ] -> [ %d ]\n", i, FindParent(i));
    }


    // Printing output.
    printf("Data size : %u\n", nData);
    for(int i = 0; i < nData; i++) printf("%d, ", g_data[i].m_iParentIndex);

    return 0;
}
