#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdint.h>


// Template for Comparision function.
typedef bool (* T_Comparision)(void*, void*);

void Heapify    (void* pData, size_t iElementSize, int nElements, T_Comparision pComparisionFn);
void HeapifyFrom(void* pData, size_t iElementSize, int iParentIndex,  int nElements, T_Comparision pComparisionFn, void* pTempVar);
int  Pop        (void* pData, size_t iElementSize,   int iRootIndex, int iLastIndex, T_Comparision pComparisionFn, void* pDataOut);


/*

Finite automata string matching
Longest sub-sequence.

BFS & DFS

matrix multiplication & Optimal BST copy

*/