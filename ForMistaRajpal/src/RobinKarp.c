#include <stdio.h>
#include <stdbool.h>


const char g_szText[]    = "these are a niggare, and you are a supareNigare";
const char g_szPattern[] = "are";


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static size_t HashString(const char* pBegin, char* pEnd)
{
    char* pChar = pBegin;
    size_t iHashOut = 0;

    while (pChar != pEnd && *pChar != '\0')
    {
        iHashOut += HashChar(*pChar);
        pChar++;
    }

    return iHashOut;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static inline int HashChar(const char c)
{
    return (int)c;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
static bool FullStringMatch(const char* szPattern, const char* szStart, int iPatternSize)
{
    for (int i = 0; i < iPatternSize; i++)
    {
        if (szStart[i] != szPattern[i])
            return false;
    }

    return true;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    size_t nPattenSize  = sizeof(g_szPattern) - 1LLU;
    size_t nTextSize    = sizeof(g_szText)    - 1LLU;
    
    size_t iPatternHash = HashString(g_szPattern, NULL);
    size_t iRollingHash = HashString(g_szText,    &g_szText[0] + nPattenSize);

    for (size_t i = 0; i <= nTextSize - nPattenSize; i++)
    {
        if (iPatternHash == iRollingHash)
        {
            if (FullStringMatch(g_szPattern, &g_szText[i], nPattenSize) == true)
                printf("Hash match found @ %llu -> %s\n", i, &g_szText[i]);
        }

        // Roll hash forward by one character for the next iteration.
        iRollingHash -= HashChar(g_szText[i]);
        iRollingHash += HashChar(g_szText[i + nPattenSize]);
    }


    if (getchar() != 0)
        printf("Terminating\n");

    return 0;
}