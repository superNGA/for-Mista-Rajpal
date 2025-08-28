#include <stdio.h>
#include <math.h>
#include <stdlib.h>


typedef struct Node_t
{
    float m_flData;
    struct Node_t* m_pNext;
}Node_t;


int main(void)
{
    float testCase[] = { -12.0f, -4.0f, -34.0f, 5.0f, 0.0f, 23.0f, 13.0f, 17.0f, 24.0f, 32.0f };
    int   nElements  = sizeof(testCase) / sizeof(float);
    float dataFraction[sizeof(testCase) / sizeof(float)]; for (int i = 0; i < nElements; i++) dataFraction[i] = 0.0f;

    Node_t* output[sizeof(testCase) / sizeof(float)];
    for (int i = 0; i < nElements; i++)
        output[i] = NULL;

    // Finding Min & Max from data.
    float flMin = INFINITY, flMax = -INFINITY;
    for (int i = 0; i < nElements; i++)
    {
        if (testCase[i] > flMax)
            flMax = testCase[i];

        if (testCase[i] < flMin)
            flMin = testCase[i];
    }

    // Normalizing all data.
    for (int i = 0; i < nElements; i++)
    {
        dataFraction[i] = (testCase[i] - flMin) / (flMax - flMin);
        printf("%.2f, ", dataFraction[i]);
    }


    // Now all data lies from 0 to n.
    for (int i = 0; i < nElements; i++)
    {
        int k = dataFraction[i] * (nElements - 1);

        Node_t* pParent    = output[k];
        Node_t* pNewNode   = (Node_t*)malloc(sizeof(Node_t));
        if (pNewNode == NULL)
        {
            printf("Bad allocation\n");
            break;
        }

        pNewNode->m_flData = testCase[i];
        pNewNode->m_pNext  = NULL;

        if (pParent == NULL)
        {
            output[k] = pNewNode;
        }
        else if (pParent->m_flData > pNewNode->m_flData)
        {
            pNewNode->m_pNext = output[k];
            output[k] = pNewNode;
        }
        else
        {
            while(pParent->m_pNext != NULL)
            {
                if (pParent->m_flData < pNewNode->m_flData && (pParent->m_pNext == NULL || pParent->m_pNext->m_flData > pNewNode->m_flData))
                    break;

                pParent = pParent->m_pNext;
            }

            pNewNode->m_pNext = pParent->m_pNext;
            pParent->m_pNext = pNewNode;
        }

        printf("Added new node : %.2f\n", pNewNode->m_flData);
    }

    Node_t* pLastNode = NULL;
    Node_t* pHead = NULL;
    for (int i = 0; i < nElements; i++)
    {
        Node_t* pCurNode = output[i];

        if (pHead == NULL && pCurNode != NULL)
            pHead = pCurNode;

        if (pCurNode == NULL)
            continue;
        
        if (pLastNode != NULL)
            pLastNode->m_pNext = pCurNode;

        while (pCurNode->m_pNext != NULL)
        {
            pCurNode = pCurNode->m_pNext;
        }

        pLastNode = pCurNode;
    }


    for(int i = 0; i < nElements; i++)
    {
        printf("%.2f -> ", pHead->m_flData);
        pHead = pHead->m_pNext;
    }

    int x = getchar();
    return 0;
}