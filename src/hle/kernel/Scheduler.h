#pragma once

#include <util/defs.h>
#include <hle/kernel/KThread.h>

class Scheduler
{
private:
    std::vector<KThread*> threads;
    int thread_index;
public:
    Scheduler();

    void CreateThread(uint64_t entry);

    void EnterScheduler();
    void SwapThreads();
};

extern Scheduler* g_Scheduler;