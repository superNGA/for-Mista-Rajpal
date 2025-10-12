#include <stdio.h>
#include <math.h>

#define T int
#define MASK (T)(0xF)
#define SWAP(a, b, type) {type _temp_ = a; a = b; b = _temp_;}
#define INT_ONLY_CHECK() {(T)5 % 2;}


///////////////////////////////////////////////////////////////////////////
void CountSortInPlace(T* pData, T* pDataOut, T mask, int nElements, int* pCounterArray, int nMaxRange);


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    INT_ONLY_CHECK();

    // Test case.
    T         testCase[] = { 123, 543, 122, 541, 341, 245, 676, 876, 923, 234};
    const int nElements  = sizeof(testCase) / sizeof(T);

    T helper[sizeof(testCase) / sizeof(T)];

    int pCounter[MASK + 1];
    int nMaskBits   = (int)(log2((float)(MASK + 1)));
    int nIterations = (sizeof(T) * 8) / nMaskBits;

    int mask = MASK;
    T* pDataInput = testCase; T* pHelper = helper;
    for (int i = 0; i < nIterations; i++)
    {
        CountSortInPlace(pDataInput, pHelper, mask, nElements, pCounter, MASK + 1);
        
        // Updating input & helper arrays.
        SWAP(pDataInput, pHelper, T*);

        // Updating mask.
        mask = mask << (nMaskBits);
    }

    for (int i = 0; i < nElements; i++)
        printf("%d, ", pDataInput[i]);

    int x = getchar();

    return 0;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void CountSortInPlace(T* pData, T* pDataOut, T mask, int nElements, int* pCounterArray, int nMaxRange)
{
    /*{
        printf("MASK : [ %d ]", mask);
        const int nBits = sizeof(T) * 8;
        for (int i = 0; i < nBits; i++)
        {
            printf("%d", (mask & (1 << (nBits - 1 - i))) == 0 ? 0 : 1);

            if ((i + 1) % 4 == 0)
                printf(" ");
        }
        printf("\n");
    }*/

    // zero out counter.
    for (int i = 0; i < nMaxRange; i++)
    {
        pCounterArray[i] = 0;
    }

    // Calculating how many bits I need to shift the data to get to get rid of any following zeros.
    int nMaskZeroBits = 0;
    int iMaskTemp     = mask;
    while ((iMaskTemp & 1) == 0)
    {
        iMaskTemp = iMaskTemp >> 1;
        nMaskZeroBits++;
    }

    //printf("Data needs to be shifted %d bits.\n", nMaskZeroBits);

    // Count.
    for (int i = 0; i < nElements; i++)
    {
        pCounterArray[(pData[i] & mask) >> nMaskZeroBits] += 1;
    }

    // Calculating indicies for storage in output array.
    for (int i = 1; i < nMaxRange; i++)
    {
        pCounterArray[i] += pCounterArray[i - 1];
        //printf("%d has %d dups\n", i, pCounterArray[i]);
    }

    // Storing elements in output array. ( iteration order here is very important )
    for (int i = nElements - 1; i >= 0; i--)
    {
        int iCounterIndex = (pData[i] & mask) >> nMaskZeroBits;
        int iStorageIndex = pCounterArray[iCounterIndex];
        
        printf("Storing [ %d  i.e.  %d] @ [ %d ]. updated storage index -> %d\n", pData[i], pData[i] & mask, iStorageIndex, iStorageIndex - 1);

        pDataOut[iStorageIndex - 1]   = pData[i];
        pCounterArray[iCounterIndex] -= 1;
    }

    printf("Final Array : ");
    for (int i = 0; i < nElements; i++)
    {
        printf("%d [ %d ], ", pDataOut[i], (pDataOut[i] & mask) >> nMaskZeroBits);
    }
    printf("\n");
}