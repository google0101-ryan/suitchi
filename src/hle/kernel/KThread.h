#pragma once 

#include <hle/kernel/KObject.h>
#include <aarch64/arm.h>

class KThread : public KObject
{
private:
    ArmCoreContext context;
public:
    KThread(uint64_t entry);

    ArmCoreContext& GetContext() {return context;}
};