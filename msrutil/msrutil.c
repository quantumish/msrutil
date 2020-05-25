//
//  msrutil.c
//  msrutil
//
//  Created by David Freifeld on 5/2/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#include <sys/systm.h>
#include <os/log.h>
#include <mach/mach_types.h>

//kern_return_t msrutil_start(kmod_info_t * ki, void *d);
//kern_return_t msrutil_stop(kmod_info_t *ki, void *d);

kern_return_t msrutil_start(kmod_info_t * ki, void *d)
{
    os_log(OS_LOG_DEFAULT, "msrutil successfully loaded!");
    uint32_t lo, hi;
    asm volatile("rdmsr":"=a"(lo),"=d"(hi):"c"(0x10));
    printf("AAAAA %d%d", lo, hi);
    os_log(OS_LOG_DEFAULT, "AAAAA %d%d", lo, hi);
//    register int *eax asm ("eax");
//    register int *ecx asm ("ecx");
//    register int *edx asm ("edx");
//    printf("%i%i%i", *eax, *ecx, *edx);
    return KERN_SUCCESS;
}

kern_return_t msrutil_stop(kmod_info_t *ki, void *d)
{
    os_log(OS_LOG_DEFAULT, "msrutil successfully unloaded.");
    return KERN_SUCCESS;
}
