#include "memory.h"
#include <map>
#include <sys/mman.h>

struct AddrSpacePiece
{
    size_t size;
    bool r, w, x;
};

std::map<uint8_t*, AddrSpacePiece> pieces;

uint8_t* stack;

// Map various extra memory (stack, heap, tls, etc) after data
void Memory::Initialize()
{
    uint8_t* alias = (uint8_t*)mmap(0,  0x1000000000, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_NORESERVE, -1, 0);
    if (alias == MAP_FAILED)
    {
        printf("ERROR: Couldn't allocate alias region");
        exit(1);
    }
    map(alias, alias+0x1000000000, true, true, true);
    
    uint8_t* heap = (uint8_t*)mmap(0,  0x180000000, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_NORESERVE, -1, 0);
    if (heap == MAP_FAILED)
    {
        printf("ERROR: Couldn't allocate heap region");
        exit(1);
    }
    map(heap, heap+0x180000000, true, true, true);
    
    stack = (uint8_t*)mmap(0,  0x80000000, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_NORESERVE, -1, 0);
    if (stack == MAP_FAILED)
    {
        printf("ERROR: Couldn't allocate stack region");
        exit(1);
    }
    map(stack, stack+0x80000000, true, true, true);
    
    uint8_t* tlsInfo = (uint8_t*)mmap(0,  0x1000000000, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_NORESERVE, -1, 0);
    if (tlsInfo == MAP_FAILED)
    {
        printf("ERROR: Couldn't allocate tlsInfo region");
        exit(1);
    }
    map(tlsInfo, tlsInfo+0x1000000000, true, true, true);
}

void Memory::map(uint8_t *start, uint8_t *end, bool r, bool w, bool x)
{
    if (pieces.find(start) != pieces.end())
    {
        printf("Warning: Tried to remap addr %p\n", start);
    }

    AddrSpacePiece piece;
    piece.size = (size_t)(end - start);
    piece.r = r;
    piece.w = w;
    piece.x = x;

    pieces[start] = piece;
    printf("Mapping %p (%lu bytes)\n", start, end-start);
}

uint32_t Memory::Read32(uint64_t addr)
{
    if (pieces.lower_bound((uint8_t*)addr) == pieces.end())
    {
        printf("ERROR: Tried to read 4 bytes from unknown addr 0x%08lx\n", addr);
        exit(1);
    }

    // if (!pieces[(uint8_t*)addr].r)
    // {
    //     printf("ERROR: Tried to read 4 bytes from read-protected addr 0x%08lx\n", addr);
    //     exit(1);
    // }

    return *(uint32_t*)addr;
}

uint64_t free_stack_base = 0;

uint64_t Memory::AllocStackMemory(uint64_t size)
{
    uint64_t addr = (uint64_t)(stack + free_stack_base);
    free_stack_base += size;
    return addr;
}

uint64_t Memory::AllocGPMemory(uint64_t size)
{
    uint64_t addr = (uint64_t)mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_NORESERVE, -1, 0);
    map((uint8_t*)addr, (uint8_t*)addr+size, true, true, true);
    return addr;
}
