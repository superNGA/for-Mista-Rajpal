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

    g_data[iIndex].m_iParentIndex = iIndex;
    return iIndex;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int FindParentWithPathComp(int iIndex)
{
    AssertValidIndex(iIndex, __FUNCTION__); 

    int iParent = g_data[iIndex].m_iParentIndex;
    
    // if this node is not its parent. ( i.e. root node. ) find check its parent & recurse till we get to peak parent.
    if(iParent != iIndex)
        g_data[iIndex].m_iParentIndex = FindParentWithPathComp(g_data[iIndex].m_iParentIndex);

    return iParent;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int Union(int iParentIndex, int iChildIndex)
{
    AssertValidIndex(iParentIndex, __FUNCTION__); AssertValidIndex(iChildIndex, __FUNCTION__);

    g_data[iChildIndex].m_iParentIndex = iParentIndex;
    return iParentIndex;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    memset((void*)g_data, 0, sizeof(g_data));

    for(int i = 0; i < nData; i++) MakeSet(i);

    Union(0, 2);
    Union(2, 3);
    Union(3, 6);
    Union(6, 1);

    for(int i = 0; i < nData; i++) printf("%d, ", g_data[i].m_iParentIndex);
    printf("\n\n");

    for(int i = 0; i < nData; i++)
    {
        printf("Parent for index [ %d ] -> [ %d ]\n", i, FindParentWithPathComp(i));
    }


    // Printing output.
    printf("Data size : %u\n", nData);
    for(int i = 0; i < nData; i++) printf("%d, ", g_data[i].m_iParentIndex);

    return 0;
}
