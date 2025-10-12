#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "UtilHeap/Heap.h"

typedef struct Task_t
{
    int m_iDuration;
    int m_iDeadline;
    int m_iWeight;
}Task_t;


bool ComparisionFn(void* pTask1, void* pTask2)
{
    return ((Task_t*)pTask1)->m_iWeight > ((Task_t*)pTask2)->m_iWeight;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    Task_t tasks[] = {
        {2, 4,  10},
        {1, 1,  5},
        {2, 2,  8},
        {1, 3,  7},
        {3, 6,  6},
        {4, 8,  9},
        {2, 5,  4},
        {1, 2,  3},
        {3, 7,  12},
        {2, 9,  11},
        {1, 3,  2},
        {2, 6,  5},
        {3, 10, 8},
        {1, 4,  6},
        {2, 8,  7},
        {3, 12, 10},
        {1, 5,  4},
        {2, 7,  9},
        {3, 11, 3},
        {1, 6,  5}
    };
    const size_t nTasks  = sizeof(tasks) / sizeof(Task_t);
    Task_t*      taskOut = (Task_t*)(malloc(sizeof(tasks)));
    if (taskOut == NULL)
        return 0;
    memset(taskOut, 0, sizeof(taskOut));


    // Heapify to sort.
    Heapify(tasks, sizeof(Task_t), nTasks, ComparisionFn);
    
    int iCurrentTime = 0; int iTaskOutIndex = 0; Task_t taskTemp = { 0, 0, 0 };
    for (int iTaskIndex = 0; iTaskIndex < nTasks; iTaskIndex++)
    {
        // Get the height weight task. ( this is greedy approach btw )
        Pop(tasks, sizeof(Task_t), 0, nTasks - 1 - iTaskIndex, ComparisionFn, &taskTemp);

        if (taskTemp.m_iDuration + iCurrentTime <= taskTemp.m_iDeadline)
        {
            // store this one
            taskOut[iTaskOutIndex] = taskTemp; iTaskOutIndex++;

            // We will be doing this task till "its duration" ammount of time.
            iCurrentTime += taskTemp.m_iDuration;
        }
    }

    for (int iTaskIndex = 0; iTaskIndex < iTaskOutIndex; iTaskIndex++)
        printf("Duration : %d, DeadLine : %d, Weight : %d\n", taskOut[iTaskIndex].m_iDuration, taskOut[iTaskIndex].m_iDeadline, taskOut[iTaskIndex].m_iWeight);

    if (getchar() != 0)
        printf("Terminating\n");

    return 0;
}