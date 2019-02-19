#ifndef _INST_SPEC_H_
#define _INST_SPEC_H_

#include "dyn_regs.h"

#include <vector>

struct BPATCH_DLL_EXPORT InstSpec {
    std::vector<Dyninst::MachRegister> saveRegs;
    bool inlineCalls;   
    bool trampGuard;
    bool redZone;
    InstSpec():
        inlineCalls(false),
        trampGuard(true),
        redZone(true)
    {}
};



#endif
