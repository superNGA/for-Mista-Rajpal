#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdint.h>


typedef struct CharInfo_t
{
    int32_t m_iFrequency;
    int32_t m_iCode;
    char    m_char;
}CharInfo_t;


#define T CharInfo_t 

// Template for Comparision function.
typedef bool (* T_Comparision)(T*, T*);

void Heapify    (T* pData, int nElements, T_Comparision pComparisionFn);
void HeapifyFrom(T* pData, int iParentIndex, int nElements,  T_Comparision pComparisionFn);
int  Pop        (T* pData, int iRootIndex,   int iLastIndex, T_Comparision pComparisionFn, T* pDataOut);
