#pragma once

#include <util/defs.h>

enum KObjectType
{
    ObjectType_KThread,
};

class KObject
{
private:
    Handle objectHandle;
    static Handle curObjectHandle;
public:
    KObject(KObjectType type) : type(type) {objectHandle = curObjectHandle++;}

    KObjectType type;
};