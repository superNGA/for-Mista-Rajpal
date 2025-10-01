#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdint>


// Process Imformation.
struct Process_t
{
    Process_t() : m_iStartTime(0), m_iExectime(0) {}

    Process_t(int32_t iStartTime, int32_t iPriority, int32_t iExecTime) : 
        m_iStartTime(iStartTime), m_iExectime(iExecTime) {}

    int32_t m_iStartTime = 0;
    int32_t m_iExectime  = 0;
};


bool PickMVPProc(Process_t& procOut, std::vector<Process_t>& vecProcess, int64_t iCurrenTick);
void RunProcess(Process_t& activeProcess, bool& bCPUIdle);


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int main(void)
{
    std::vector<Process_t> vecProcess;
    vecProcess.push_back(Process_t(0, 10, 10));
    vecProcess.push_back(Process_t(2, 0,  30));
    vecProcess.push_back(Process_t(5, 2,  24));
    vecProcess.push_back(Process_t(1, 5,  1));
    vecProcess.push_back(Process_t(9, 0,  9));
    

    Process_t activeProc;
    int64_t   iTimer   = 0;
    bool      bCPUIdle = true;
    while(vecProcess.empty() == false)
    {
        if(bCPUIdle == true)
        {
            PickMVPProc(activeProc, vecProcess, iTimer);
            printf("Selected proces with StartTime : %d CurrentTick : %lld\n",  activeProc.m_iStartTime, iTimer);
        }

        RunProcess(activeProc, bCPUIdle);

        iTimer++;
    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
bool PickMVPProc(Process_t& procOut, std::vector<Process_t>& vecProcess, int64_t iCurrentTick)
{
    if(vecProcess.empty() == true)
        return false;


    // Picking process with most priority
    int32_t iShortestBurst = INT_MAX;
    std::vector<Process_t>::iterator entryToRemove = vecProcess.end();
    for(auto it = vecProcess.begin(); it != vecProcess.end(); it++)
    {
        if(it->m_iExectime < iShortestBurst && it->m_iStartTime <= iCurrentTick)
        {
            procOut        = *it;
            iShortestBurst = it->m_iExectime;
            entryToRemove  = it;
        }
    }

    // Remove from the list, whatever we have found.
    if(entryToRemove != vecProcess.end())
        vecProcess.erase(entryToRemove);

    return iShortestBurst != INT_MAX;
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void RunProcess(Process_t& activeProcess, bool& bCPUIdle)
{
    activeProcess.m_iExectime -= 1;

    if(activeProcess.m_iExectime <= 0)
    {
        printf("proc [ StartTime : %d, ExecTime : %d ] ended\n", activeProcess.m_iStartTime, activeProcess.m_iExectime);
        bCPUIdle = true;
    }
    else 
    { 
        bCPUIdle = false;
    }
}
