#pragma once

#include <util/defs.h>

struct ArmCoreContext
{
    uint64_t pc;

    // r31 is special in that it represents either sp or 0 depending on context
    struct
    {
        uint64_t x;
        uint32_t w;
    } regs[32];

    bool n, z, c, v;
};

class ArmCore
{
private:
    void SetFlags64(uint64_t result, uint64_t op1, uint64_t op2, bool is_sub);

    void DecodeDataProcImm(uint32_t instr);
        void AddSubImm(uint32_t instr);
    void DecodeBranches(uint32_t instr);

    void UnconditionalBranchImm(uint32_t instr);
public:
    ArmCoreContext context;

    uint64_t base;

    void Run();
    void Dump();
};

extern ArmCore core;