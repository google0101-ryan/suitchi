#pragma once

#include <util/defs.h>

namespace Memory
{

void Initialize();

void map(uint8_t* start, uint8_t* end, bool r = true, bool w = true, bool x = true);

uint32_t Read32(uint64_t addr);

uint64_t AllocStackMemory(uint64_t size);
uint64_t AllocGPMemory(uint64_t size);

}