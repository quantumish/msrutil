//
//  msrutil.c
//  msrutil
//
//  Created by David Freifeld on 5/2/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

// FORGET-ME-NOT: Build solely ~/projects/msrutil/build/Products/Debug/msrutil.kext and

#include "msrutil.h"

#include <sys/systm.h>
#include <sys/kern_control.h>
#include <os/log.h>
#include <mach/mach_types.h>
#include <string.h>
#include <sys/types.h>

// Included in Xcode template, not sure why.
/* kern_return_t msrutil_start(kmod_info_t * ki, void *d); */
/* kern_return_t msrutil_stop(kmod_info_t *ki, void *d); */

kern_return_t msrutil_start(kmod_info_t * ki, void *d)
{
    os_log(OS_LOG_DEFAULT, "msrutil successfully loaded.");
    asm volatile ("movl $412, %ecx");
    asm volatile ("rdmsr");
    unsigned long long eax = 0;
    unsigned long long ecx = 0;
    unsigned long long edx = 0;
    asm("movq %%rax,%0" : "=r"(eax));
    asm("movq %%rcx,%0" : "=r"(ecx));
    asm("movq %%rdx,%0" : "=r"(edx));
    os_log(OS_LOG_DEFAULT, "Read MSR. EAX: %llu, ECX: %llu, EDX: %llu", eax, ecx, edx);
    return KERN_SUCCESS;
}

kern_return_t msrutil_stop(kmod_info_t *ki, void *d)
{
    os_log(OS_LOG_DEFAULT, "msrutil successfully unloaded.");
    return KERN_SUCCESS;
}
