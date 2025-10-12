#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

#include "UtilHeap/Heap.h"


// typedef struct CharInfo_t
// {
    // int32_t m_iFrequency;
    // int32_t m_iCode;
    // char    m_char;
// }CharInfo_t;

typedef struct HuffmanNode_t
{
    CharInfo_t            m_charInfo;
    struct HuffmanNode_t* m_pLeft;
    struct HuffmanNode_t* m_pRight;
}HuffmanNode_t;


char*       g_szBuffer     = NULL;
const char* g_szTargetFile = "Test.txt";
int         g_frequencyRecord[0xFF];


int64_t     GetFileSize(const char* szFile);
bool        DumpFileToBuffer(char* szBuffer, const char* szFile);
CharInfo_t* CreateFrequencyRecords(char* szBuffer, int* pFrequencyOut, int64_t iBufferSize, int64_t iFrequencyRecordSize, int32_t* iSizeOut);
void        CalculateCodes(CharInfo_t* pFrequencyRecords, int nRecords); 
bool        Comparision(CharInfo_t* p1, CharInfo_t* p2);


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    // Get file size.
    int64_t iFileSize = GetFileSize(g_szTargetFile); if(iFileSize < 0) return 1;

    // Allcoate file size.
    int64_t iBufferSize = iFileSize * sizeof(char);
    g_szBuffer = (char*)malloc(iBufferSize); if(g_szBuffer == NULL) return 1; 

    // Write file to buffer.
    if(DumpFileToBuffer(g_szBuffer, g_szTargetFile) == false) return 1;

    // Create freuqncy records.
    int nRecords = 0;
    CharInfo_t* pCharInfo = CreateFrequencyRecords(g_szBuffer, g_frequencyRecord, iBufferSize, sizeof(g_frequencyRecord) / sizeof(int), &nRecords);

    // Create Huffman tree.
    CalculateCodes(pCharInfo, nRecords);

    return 0;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int64_t GetFileSize(const char* szFile)
{
    struct stat st;
    if(stat(szFile, &st) != 0)
    {
        printf("Failed to get file size");
        return -1;
    }

    printf("%ld Bytes -> target file\n", st.st_size);
    return st.st_size;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
bool DumpFileToBuffer(char* szBuffer, const char* szFile)
{
    FILE* pFile = NULL; fopen_s(&pFile, szFile, "r");

    // Failed file opening.
    if(pFile == NULL)
    {
        printf("Failed to open file.\n");
        return false;
    } 

    int iBufferIndex = 0;
    int ch = 0; ch = fgetc(pFile);

    while(ch != EOF)
    {
        szBuffer[iBufferIndex] = (char)ch;
        iBufferIndex++;

        ch = fgetc(pFile);
    }
    szBuffer[iBufferIndex] = '\0';

    fclose(pFile);
    return true;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
bool Comparision(CharInfo_t* p1, CharInfo_t* p2) { return p1->m_iFrequency <= p2->m_iFrequency; }
CharInfo_t* CreateFrequencyRecords(char* szBuffer, int* pFrequencyOut, int64_t iBufferSize, int64_t iFrequencyRecordSize, int32_t* iSizeOut)
{
    memset(pFrequencyOut, 0, iFrequencyRecordSize);

    for(int i = 0; i < iBufferSize; i++)
    {
        pFrequencyOut[szBuffer[i]] += 1;
    }

    int nNonZeroRecords = 0;
    for(int i = 0; i < iFrequencyRecordSize; i++)
    {
        if(pFrequencyOut[i] > 0)
        {
            nNonZeroRecords++;
        }
    }

    if(nNonZeroRecords <= 0)
        return NULL;

    CharInfo_t* pCharRecords = (CharInfo_t*)malloc(sizeof(CharInfo_t) * nNonZeroRecords);
    if(pCharRecords == NULL) 
        return NULL;


    // Writting non zero data form frequency records to output buffer.
    *iSizeOut             = nNonZeroRecords;
    int iActiveCharRecord = 0;
    for(int i = 0; i < iFrequencyRecordSize; i++)
    {
        if(pFrequencyOut[i] > 0)
        {
            pCharRecords[iActiveCharRecord].m_char       = (char)i;
            pCharRecords[iActiveCharRecord].m_iCode      = 0;
            pCharRecords[iActiveCharRecord].m_iFrequency = pFrequencyOut[i];
            iActiveCharRecord += 1;
        }
    }

    Heapify(pCharRecords, nNonZeroRecords, Comparision);
    
    // Heap sorting the frequency records.
    int iLastIndex = nNonZeroRecords - 1;
    while (iLastIndex >= 0) 
    {
        CharInfo_t temp; 
        int iNewIndex = Pop(pCharRecords, 0, iLastIndex, Comparision, &temp);

        pCharRecords[iLastIndex] = temp;

        iLastIndex = iNewIndex; 
    }

    return pCharRecords;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void CalculateCodes(CharInfo_t* pFrequencyRecords, int nRecords)
{
    int iCode            = 0;
    int nTotalBits       = 0.0f;
    int nTotalCharacters = 0;

    for(int iRecordIndex = 0; iRecordIndex < nRecords; iRecordIndex++)
    {
        pFrequencyRecords[iRecordIndex].m_iCode = iCode;
        iCode |= (1 << (iRecordIndex + 1));

        int nCodeBits     = iRecordIndex + 1;
        nTotalBits       += nCodeBits * pFrequencyRecords[iRecordIndex].m_iFrequency; 
        nTotalCharacters += pFrequencyRecords[iRecordIndex].m_iFrequency;

        printf("'%c' -> %d CODE : %d\n", pFrequencyRecords[iRecordIndex].m_char, pFrequencyRecords[iRecordIndex].m_iFrequency, pFrequencyRecords[iRecordIndex].m_iCode);
    }

    printf("Bits per character : %f\n", (float)(nTotalBits) / (float)(nTotalCharacters));
}
