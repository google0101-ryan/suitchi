#include <loader/nro.h>
#include <hle/memory/memory.h>
#include <hle/kernel/Scheduler.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <nro name>\n", argv[0]);
        return 0;
    }

    NROLoader loader;
    Suitchi::result<Executable> exe = loader.LoadNRO(argv[1]);

    if (!exe.has_value())
    {
        printf("ERROR: Couldn't load file \'%s\'\n", argv[1]);
        return -1;
    }

    Memory::map(exe->code.mem.data(), exe->code.mem.data()+exe->code.mem.size_bytes());
    Memory::map(exe->ro.mem.data(), exe->ro.mem.data()+exe->ro.mem.size_bytes());
    Memory::map(exe->data.mem.data(), exe->data.mem.data()+exe->data.mem.size_bytes());

    Memory::Initialize();

    g_Scheduler->CreateThread((uint64_t)exe->code.mem.data());
    g_Scheduler->EnterScheduler();

    std::atexit([]() {core.Dump();});

    core.base = (uint64_t)exe->code.mem.data();
    core.Run();

    return 0;
}