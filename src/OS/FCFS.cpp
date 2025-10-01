#include <iostream>
#include <deque>
#include <cstdint>


// Process Imformation.
struct Process_t
{
    Process_t() : m_iStartTime(0), m_iExectime(0) {}

    Process_t(int32_t iStartTime, int32_t iExecTime) : 
        m_iStartTime(iStartTime), m_iExectime(iExecTime) {}

    int32_t m_iStartTime = 0;
    int32_t m_iExectime  = 0;
};
typedef std::deque<Process_t> ProcQ;


void CheckForNewProc(ProcQ& qReadyProc, ProcQ& qAllProc, int iCurrentTick);
void RunCPUTick(ProcQ& qReadyProc, int iCurrentTick);


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    std::deque<Process_t> qAllProc;
    std::deque<Process_t> qReadyProc;
    qAllProc.push_back(Process_t(0, 10));
    qAllProc.push_back(Process_t(2, 13));
    qAllProc.push_back(Process_t(5, 12));
    qAllProc.push_back(Process_t(1, 90));
    qAllProc.push_back(Process_t(0, 02));
    qAllProc.push_back(Process_t(9, 14));
    

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
void CheckForNewProc(ProcQ& qReadyProc, ProcQ& qAllProc, int iCurrentTick)
{
    for(auto it = qAllProc.begin(); it != qAllProc.end();)
    {
        if(it->m_iStartTime <= iCurrentTick)
        {
            qReadyProc.push_back(*it);
            printf("Found new proc -> StartTime : %d, ExecTime : %d\n", it->m_iStartTime, it->m_iExectime);

            it = qAllProc.erase(it);
        }
        else 
        {
            it++;
        }
    }
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void RunCPUTick(ProcQ& qReadyProc, int iCurrentTick)
{
    static Process_t* pActiveProc = nullptr;

    if(pActiveProc == nullptr)
    {
        pActiveProc = &qReadyProc[0];
    }

    if(pActiveProc != nullptr)
    {
        pActiveProc->m_iExectime -= 1;

        if(pActiveProc->m_iExectime <= 0)
        {
            printf("Done proc -> start time : %d, exec time : %d\n", pActiveProc->m_iStartTime, pActiveProc->m_iExectime);
            qReadyProc.pop_front();
            pActiveProc = nullptr;
        }
    }
}


