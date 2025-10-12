#include <stdio.h>
#include <assert.h>


///////////////////////////////////////////////////////////////////////////
#define T int


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    // To make sure valid data type.
    (T)5 % 2;
    
    T         testCase[] = { 1, 2, 2, 4, 6, 7, 5, 3, 4, 6, 8, 9, 0, 7, 6, 2, 3, 4, 1, 5, 4, 7, 6, 6, 5, 6, 8, 9, 4, 3, 2};
    const int nElements  = sizeof(testCase) / sizeof(T);

    int iCounter[10] = {0};
    for (int i = 0; i < 10; i++)
        iCounter[i] = 0;

    for (int i = 0; i < nElements; i++)
        iCounter[testCase[i]] += 1;

    int nCount = 0;
    for( int i = 0; i < nElements; i++)
    {
        if (iCounter[nCount] == 0)
            nCount += 1;

        testCase[i] = nCount;
        iCounter[nCount] -= 1;
    }

    int x = getchar();

    return 0;
}