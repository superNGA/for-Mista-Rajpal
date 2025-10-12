#include <stdio.h>

#define EMPTY -1


///////////////////////////////////////////////////////////////////////////
int MakeSet(int* pSet, int iIndex);
int FindParent(int* pSet, int iIndex);
int Union(int* pSet, int iParentIndex1, int iParentIndex2);


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    int set[10]; int nElements = sizeof(set) / sizeof(int);
    for (int i = 0; i < nElements; i++) set[i] = 0;

    MakeSet(set, 0);
    MakeSet(set, 1);
    MakeSet(set, 2);
    MakeSet(set, 3);
    MakeSet(set, 4);
    MakeSet(set, 5);

    Union(set, 1, 5);
    Union(set, 1, 4);
    Union(set, 1, 3);
    Union(set, 1, 2);

    printf("Parent peak : %d\n", FindParent(set, 5));

    for (int i = 0; i < nElements; i++)
        printf("%d  ", i);
    printf("\n");
    for (int i = 0; i < nElements; i++)
        printf("%d  ", set[i]);
    printf("\n");

    int x = getchar();

    return 0;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int MakeSet(int* pSet, int iIndex)
{
    pSet[iIndex] = EMPTY; // negative number represents height.
    return iIndex;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int FindParent(int* pSet, int iIndex)
{
    if (pSet[iIndex] < 0)
        return iIndex;

    return FindParent(pSet, pSet[iIndex]);
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int Union(int* pSet, int iParentIndex1, int iParentIndex2)
{
    if (pSet[iParentIndex1] < pSet[iParentIndex2])
    {
        pSet[iParentIndex2] = iParentIndex1;
    }
    else if (pSet[iParentIndex2] < pSet[iParentIndex1])
    {
        pSet[iParentIndex1] = iParentIndex2;
    }
    else
    {
        pSet[iParentIndex2] = iParentIndex1;
        pSet[iParentIndex1] -= 1; // Incrementing height.
    }
}