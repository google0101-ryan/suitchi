#include "Scheduler.h"

Scheduler::Scheduler()
{
    thread_index = 0;
}

void Scheduler::CreateThread(uint64_t entry)
{
    KThread* t = new KThread(entry);
    threads.push_back(t);
}

void Scheduler::EnterScheduler()
{
    memcpy(&core.context, &threads[0]->GetContext(), sizeof(ArmCoreContext));
}

void Scheduler::SwapThreads()
{
    memcpy(&threads[thread_index % threads.size()]->GetContext(), &core.context, sizeof(ArmCoreContext));
    thread_index++;
    memcpy(&core.context, &threads[thread_index % threads.size()]->GetContext(), sizeof(ArmCoreContext));
}

Scheduler schedLocal;
Scheduler* g_Scheduler = &schedLocal;