#include <stdio.h>
#include "UtilDeque/Deque.h"



///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    Deque_t q;
    UtilDeque_InitDeque(&q, sizeof(int));
    int i = 10;
    UtilDeque_PushBack(&q, &i); i = 1;
    UtilDeque_PushBack(&q, &i); i = 5;
    UtilDeque_PushBack(&q, &i); i = 23;
    UtilDeque_PushBack(&q, &i); i = 10;

    int* pQ = (int*)q.m_pMemory;

    while (q.m_nElements > 0)
    {
        int shit;
        UtilDeque_PopFront(&q, &shit);
        printf("%d, ", shit);
    }

    if (getchar() != 0)
        printf("Terminating\n");
    return 0;
}