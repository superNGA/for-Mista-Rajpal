//#include <stdio.h>
//#include <stdbool.h>
//#include <malloc.h>
//#include <string.h>
//
//
//const char g_szText[]    = "123are you a nigare bro";
//const char g_szPattern[] = "are";
//
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//static int* CreatePrefixList(const char* szPattern, size_t nCharacters)
//{
//    if (nCharacters == 0 || szPattern == NULL)
//        return NULL;
//
//    size_t iSize = nCharacters * sizeof(int);
//    int* pPrefixList = (int*)(malloc(iSize));
//    if (pPrefixList == NULL)
//        return NULL;
//
//    memset(pPrefixList, 0, iSize);
//
//    // 0th character can't have repeating suffix.
//    pPrefixList[0] = 0;
//
//    for (size_t i = 0; i < nCharacters; i++)
//    {
//        // Comparing this string
//        for (int j = 0; j <= i; j++)
//            printf("%c", szPattern[j]);
//        printf("\n");
//
//        // Iterating till this pattern.
//        int iPrefixMatchLength = -1;
//        for (int j = 0; j < (i + 1) / 2; j++)
//        {
//            printf("Trying to match prefix of length : %d\n", j + 1);
//
//            bool bMatchFound = true;
//            for (int k = 0; k <= j; k++)
//            {
//                if (szPattern[k] != szPattern[i - j + k])
//                {
//                    bMatchFound = false;
//                }
//
//                printf("Checking %c against %c\n", szPattern[k], szPattern[i - j + k]);
//            }
//
//            if (bMatchFound == true && j > iPrefixMatchLength)
//                iPrefixMatchLength = j;
//        }
//
//        if(iPrefixMatchLength != -1)
//            pPrefixList[i] = iPrefixMatchLength + 1;
//    }
//
//
//    return pPrefixList;
//}
//
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//int main(void)
//{
//    size_t iPatternSize = sizeof(g_szPattern) - 1LLU;
//    size_t iTextSize    = sizeof(g_szText)    - 1LLU;
//
//    int* pPrefixList = CreatePrefixList(g_szPattern, iPatternSize);
//
//    for (int i = 0; i < iPatternSize; i++)
//        printf("%d, ", pPrefixList[i]);
//    printf("\n");
//
//    // Iterate n do something IDK.
//    for (size_t i = 0; i < iTextSize - iPatternSize + 1; i++)
//    {
//        if (g_szText[i] == g_szPattern[0])
//        {
//            bool bMatchFound = true;
//            int  j           = 0;
//
//            for (; j < iPatternSize; j++)
//            {
//                if (g_szPattern[j] != g_szText[i + j])
//                {
//                    bMatchFound = false;
//                }
//
//            }
//
//            if (bMatchFound == true)
//            {
//                printf("Match found @ index %llu\n", i);
//            }
//
//            printf("Skipping iteator to index : %llu", i + pPrefixList[j - 1]);
//            i += pPrefixList[j];
//        }
//    }
//
//
//    if (getchar() != 0)
//        printf("Terminating\n");
//    return 0;
//}

#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>


const char g_szText[] = "ABABABC";
const char g_szPattern[] = "ABABC";


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static int* CreatePrefixList(const char* szPattern, size_t nCharacters)
{
    if (nCharacters == 0 || szPattern == NULL)
        return NULL;

    int* pPrefixList = (int*)malloc(nCharacters * sizeof(int));
    if (!pPrefixList)
        return NULL;

    memset(pPrefixList, 0, nCharacters * sizeof(int));

    int len = 0;      // length of the previous longest prefix suffix
    pPrefixList[0] = 0; // first character has no prefix/suffix

    size_t i = 1;
    while (i < nCharacters)
    {
        if (szPattern[i] == szPattern[len])
        {
            len++;
            pPrefixList[i] = len;
            i++;
        }
        else
        {
            if (len != 0)
            {
                len = pPrefixList[len - 1];
                // do not increment i here
            }
            else
            {
                pPrefixList[i] = 0;
                i++;
            }
        }
    }

    return pPrefixList;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    size_t iPatternSize = strlen(g_szPattern);
    size_t iTextSize = strlen(g_szText);

    int* pPrefixList = CreatePrefixList(g_szPattern, iPatternSize);

    printf("Prefix Table (LPS): ");
    for (int i = 0; i < iPatternSize; i++)
        printf("%d ", pPrefixList[i]);
    printf("\n");

    // KMP Matching
    size_t i = 0; // index for text
    size_t j = 0; // index for pattern

    while (i < iTextSize)
    {
        if (g_szPattern[j] == g_szText[i])
        {
            i++;
            j++;
        }

        if (j == iPatternSize)
        {
            printf("Match found @ index %zu\n", i - j);
            j = pPrefixList[j - 1]; // continue searching
        }
        else if (i < iTextSize && g_szPattern[j] != g_szText[i])
        {
            if (j != 0)
                j = pPrefixList[j - 1];
            else
                i++;
        }
    }

    free(pPrefixList);
    if (getchar() != 0)
        printf("Terminating\n");
    return 0;
}
