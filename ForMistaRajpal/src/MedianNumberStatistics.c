#include <stdio.h>

#define T int
#define T_BAD 0xFFffFFff
#define SWAP(a, b, type) {type _temp_ = a; a = b; b = _temp_; }


///////////////////////////////////////////////////////////////////////////
T   Select(T* pInput, int iStart, int iEnd, int iTarget);
int Partition(T* pInput, int iStart, int iEnd);


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    T         testCase[] = { 12, 45, 29, 101, 14, 7, 2, 45, 63, 93 };
    const int nElements  = sizeof(testCase) / sizeof(T);

    T median = Select(testCase, 0, 9, 3);

    // printing
    printf("\nMEDIAN : %d\n", median);

    int x = getchar();

    return 0;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
T Select(T* pInput, int iStart, int iEnd, int iTarget)
{
    if (iStart > iEnd)
        return T_BAD;

    int iPartitionIndex = Partition(pInput, iStart, iEnd);
    int iTargetAbs      = iTarget - 1 + iStart;

    if (iTargetAbs == iPartitionIndex)
    {
        return pInput[iTargetAbs];
    }
    else if (iTargetAbs < iPartitionIndex)
    {
        return Select(pInput, iStart, iPartitionIndex - 1, iTarget);
    }
    else if (iTargetAbs > iPartitionIndex)
    {
        return Select(pInput, iPartitionIndex + 1, iEnd, (iTargetAbs - (iPartitionIndex + 1) + 1));
    }

    return T_BAD;
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