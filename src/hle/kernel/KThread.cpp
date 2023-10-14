#include "KThread.h"

#include <hle/memory/memory.h>

struct ConfigEntry
{
    uint32_t key, flags;
    uint64_t value[2];
};

KThread::KThread(uint64_t entry)
: KObject(KObjectType::ObjectType_KThread)
{
    memset(context.regs, 0, sizeof(context.regs));
    context.pc = entry;
    context.regs[31].x = Memory::AllocStackMemory(0x200000);
    ConfigEntry* cfg_entry = (ConfigEntry*)Memory::AllocGPMemory(sizeof(ConfigEntry));
    cfg_entry->key = 0;
    cfg_entry->flags = 0;
    cfg_entry->value[0] = cfg_entry->value[1] = 0;
    context.regs[0].x = (uint64_t)cfg_entry;
    context.regs[1].x = UINT64_MAX;
}