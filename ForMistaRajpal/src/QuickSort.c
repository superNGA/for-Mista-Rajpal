#include <stdio.h>

#define T int
#define SWAP(a, b, type) {type _temp_ = a; a = b; b = _temp_; }


///////////////////////////////////////////////////////////////////////////
void QuickSort(T* pInput, int iStart, int iEnd);
int  Partition(T* pInput, int iStart, int iEnd);


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    T         testCase[] = { 12, 45, 29, 101, 14, 7, 2, 45, 63, 93 };
    const int nElements  = sizeof(testCase) / sizeof(T);

    QuickSort(testCase, 0, nElements - 1);

    // Printing
    for (int i = 0; i < nElements; i++)
        printf("%d, ", testCase[i]);

    getchar();

    return 0;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void QuickSort(T* pInput, int iStart, int iEnd)
{
    if (iStart > iEnd)
        return;

    int iPivotIndex = Partition(pInput, iStart, iEnd);

    QuickSort(pInput, iStart, iPivotIndex - 1);
    QuickSort(pInput, iPivotIndex + 1, iEnd);
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int Partition(T* pInput, int iStart, int iEnd)
{
    int i = iStart; int j = iStart;

    while (j < iEnd)
    {
        if (pInput[j] < pInput[iEnd])
        {
            SWAP(pInput[i], pInput[j], T);
            i += 1;
        }
        
        j += 1;
    }

    SWAP(pInput[iEnd], pInput[i], T);
    return i;
}