#include <iostream>
#include <deque>
#include <cstdint>
#include <string>

#define MIN(a, b) ((a) <= (b) ? (a) : (b))


// Process information.
struct Process_t
{
    // Constructor.
    Process_t(int32_t iStartTime, int32_t iExecTime, std::string szID) : m_iStartTime(iStartTime), m_iExecTime(iExecTime), m_szID(szID) {}

    int32_t m_iStartTime    = 0;
    int32_t m_iExecTime     = 0;
    int32_t m_iCPUAllocTime = -1;
    std::string m_szID = "";
};
typedef std::deque<Process_t> ProcQ;


void CheckForNewProc(ProcQ& qReadyProc, ProcQ& qProcess, const int64_t iCurrentTick);
void RunCPUTick(ProcQ& qReadyProc, const int64_t iCurrentTick);


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    ProcQ qAllProc; ProcQ qReadyProc;
    qAllProc.push_back(Process_t(0,  10, "P1"));
    qAllProc.push_back(Process_t(1,  20, "P2"));
    qAllProc.push_back(Process_t(3,  30, "P3"));
    qAllProc.push_back(Process_t(2,  13, "P4"));
    qAllProc.push_back(Process_t(4,  15, "P5"));
    qAllProc.push_back(Process_t(10,  2, "P6"));

    int64_t iTimer = 0;
    while(qAllProc.empty() == false || qReadyProc.empty() == false)
    {
        CheckForNewProc(qReadyProc, qAllProc, iTimer);
        RunCPUTick(qReadyProc, iTimer);

        iTimer++;
    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void CheckForNewProc(ProcQ& qReadyProc, ProcQ& qProcess, const int64_t iCurrentTick)
{
    for(auto it = qProcess.begin(); it != qProcess.end();)
    {
        if(it->m_iStartTime <= iCurrentTick)
        {
            qReadyProc.push_back(*it);
            it = qProcess.erase(it);
        }
        else 
        {
            it++;     
        }
    }
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void RunCPUTick(ProcQ& qReadyProc, const int64_t iCurrentTick)
{
    if(qReadyProc.empty() == true)
        return;

    static Process_t* pActiveProc = nullptr;

    // Choosing new Process
    if(pActiveProc == nullptr)
    {
        pActiveProc = &qReadyProc[0];
        pActiveProc->m_iCPUAllocTime = iCurrentTick; 
        printf("Selected process [ %s ]\n", pActiveProc->m_szID.c_str());
    }
    
    if(pActiveProc != nullptr)
    {
        constexpr int64_t TIME_QUANTUM = 5;
        if(iCurrentTick - pActiveProc->m_iCPUAllocTime >= MIN(TIME_QUANTUM, pActiveProc->m_iExecTime))
        {
            pActiveProc->m_iExecTime    -= MIN(TIME_QUANTUM, pActiveProc->m_iExecTime);
            pActiveProc->m_iCPUAllocTime = -1;

            std::cout << "Removed process [ " << pActiveProc->m_szID << " ], exec time pending : " << pActiveProc->m_iExecTime << std::endl;

            if(pActiveProc->m_iExecTime > 0)
            {
                qReadyProc.push_back(*pActiveProc);
            }
            qReadyProc.pop_front();

            pActiveProc = nullptr;
        }
    }
}
