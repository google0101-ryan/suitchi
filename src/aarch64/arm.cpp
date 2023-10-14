#include "arm.h"

#include <hle/memory/memory.h>

ArmCore core;

template<class T>
T SignExtend(T x, const int bits)
{
    T m = 1;
    m <<= bits - 1;
    return (x ^ m) - m;
}

bool IsUnconditionalBranch(uint32_t instr)
{
    return ((instr >> 26) & 0x3F) == 0x5;
}

void ArmCore::SetFlags64(uint64_t result, uint64_t op1, uint64_t op2, bool is_sub)
{
    if (is_sub)
        op2 = ~op2;
    bool s1 = (op1 >> 63) & 1;
    bool s2 = (op2 >> 63) & 1;
    bool sr = (result >> 63) & 1; 
    context.n = (result >> 63) & 1;
    context.z = (result == 0);
    context.v = (!(s1 ^ s2)) & (sr ^ s1);
    context.c = (s1 | s2) & !sr;
}

void ArmCore::DecodeDataProcImm(uint32_t instr)
{
    uint8_t op0 = (instr >> 23) & 0x7;

    switch (op0)
    {
    case 2:
        AddSubImm(instr);
        break;
    default:
        printf("Unknown data processing imm op0 0x%02x\n", op0);
        exit(1);
    }

    context.pc += 4;
}

void ArmCore::AddSubImm(uint32_t instr)
{
    bool sf = (instr >> 31) & 1;
    bool op = (instr >> 30) & 1;
    bool s = (instr >> 29) & 1;
    bool sh = (instr >> 22) & 1;
    uint64_t imm = (instr >> 10) & 0xFFF;
    uint8_t rn = (instr >> 5) & 0x1F;
    uint8_t rd = instr & 0x1F;

    if (sh)
        imm <<= 12;

    uint64_t reg1 = context.regs[rn].x;
    uint64_t result;
    if (op)
        result = reg1 - imm;
    else
        result = reg1 + imm;
    if (s)
        SetFlags64(result, reg1, imm, op);
    if (rd == 31 && s)
        printf("cmp x%d, #%ld\n", rn, imm);
    else
    {
        if (sf)
            context.regs[rd].x = result;
        else
            context.regs[rd].x = result & 0xFFFFFFFF;
        if (rd == 31)
            printf("%s sp, x%d, #%ld\n", op ? "sub" : "add", rn, imm);
        else
            printf("%s x%d, x%d, #%ld\n", op ? "sub" : "add", rd, rn, imm);
    }
}

void ArmCore::DecodeBranches(uint32_t instr)
{
    uint8_t op0 = (instr >> 31) & 0x7;
    switch (op0)
    {
    case 0:
        UnconditionalBranchImm(instr);
        break;
    default:
        printf("Unknown branch op0 0x%02x\n", op0);
        exit(1);
    }
}

void ArmCore::UnconditionalBranchImm(uint32_t instr)
{
    int32_t offs = SignExtend<int32_t>((instr & 0x3FFFFFF) << 2, 28);
    if ((instr >> 31) & 1)
        context.regs[30].x = context.pc;
    context.pc += offs;

    printf("b%s 0x%08lx\n", ((instr >> 31) & 1) ? "l" : "", context.pc - base);
}

void ArmCore::Run()
{
    std::unordered_map<uint8_t, std::function<void(uint32_t)>> decoders =
    {};

    decoders[0x8] = decoders[0x9] = std::bind(&ArmCore::DecodeDataProcImm, this, std::placeholders::_1);
    decoders[0xA] = decoders[0xB] = std::bind(&ArmCore::DecodeBranches, this, std::placeholders::_1);

    while (1)
    {
        uint32_t instr = Memory::Read32(context.pc);

        uint8_t index = (instr >> 25) & 0xF;

        if (!decoders[index])
        {
            printf("Unhandled op0 0x%02x\n", index);
            exit(1);
        }

        decoders[index](instr);
    }
}

void ArmCore::Dump()
{
    for (int i = 0; i < 32; i++)
        printf("x%d\t->\t0x%08lx\n", i, context.regs[i].x);
    printf("pc\t->\t0x%08lx\n", context.pc);
    printf("[%s%s%s%s]\n", context.n ? "n" : ".", 
                            context.z ? "z" : ".", 
                            context.v ? "v" : ".", 
                            context.c ? "c" : ".");
}
