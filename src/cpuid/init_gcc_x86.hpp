// Copyright (c) 2013 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cpuid.h>

#include "cpuinfo.hpp"
#include "extract_x86_flags.hpp"

namespace cpuid
{
    void get_cpuid(unsigned int index, int output[4])
    {
        int a, b, c, d;

        __asm__ __volatile__(
//#if defined(__x86_64__)
//                "pushq %%rbx        \n\t"  // Save %rbx
//#else
//                "pushl %%ebx        \n\t"  // Save %ebx
//#endif
                "cpuid              \n\t"
//                "movl %%ebx, %[ebx] \n\t"  // Copy ebx into output variable
//#if defined(__x86_64__)
//                "popq %%rbx         \n\t"  // Restore %rbx
//#else
//                "popl %%ebx         \n\t"  // Restore %ebx
//#endif
                : "=a"(a), "=b"(b),
                  "=c"(c), "=d"(d)
                : "a"(index), "c"(0)
                : "%ebx","%ecx","%edx");  // Clobbered registers

        output[0] = a;
        output[1] = b;
        output[2] = c;
        output[3] = d;
    }


    /// @todo Document
    void init_cpuinfo(cpuinfo::impl& info)
    {
        // Note: We need to capture these 4 registers, otherwise we get
        // a segmentation fault on 32-bit Linux
        int output[4] = {0,0,0,0};

        // The register information per input can be extracted from here:
        // http://en.wikipedia.org/wiki/CPUID

        // Set registers for basic flag extraction
        get_cpuid(1, output);
        extract_x86_flags(info, output[2], output[3]);

        // Set registers for extended flags extraction
        get_cpuid(7, output);
        extract_x86_extended_flags(info, output[1]);
    }
}
