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
    //struct kern_ctl_reg userctl = {};
    //    ctl_register(userctl, *ctlref);
    /* uint32_t lo, hi; */
    /* // Example off of stackoverflow to test assembly */
    /* // NOTE: Figure out why kernel (and my computer) crashes when I run */
    /* // non-volatile assembly that isn't just 'nop'! */
    /* asm volatile("rdmsr" : "=a"(lo), "=d"(hi) : "c"(0x1b1)); */
    /* os_log(OS_LOG_DEFAULT, "Contents of MSR: %d %d", lo, hi); */
    /* } */
    asm volatile ("cmpq %rax, %rbx");
    return KERN_SUCCESS;
}

kern_return_t msrutil_stop(kmod_info_t *ki, void *d)
{
    os_log(OS_LOG_DEFAULT, "msrutil successfully unloaded.");
    return KERN_SUCCESS;
}
