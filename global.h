#ifndef GLOBAL_CPP
#define GLOBAL_CPP

#include <QString>
#include "data.h"
#include "install.h"
#include "version.h"

struct _global
{
    const QString VERSION = VER_PRODUCTVERSION_STR;
    const options *set;
    const install *insSet;
    _global(options *s, install *ins) : set(s), insSet(ins) {}

};
extern const _global *global;

#endif // GLOBAL_CPP
