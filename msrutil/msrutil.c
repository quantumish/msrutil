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
#include <sys/mbuf.h>
#include <string.h>
#include <sys/types.h>

#define MAX_INSTRUCTION_SIZE 32

/* A simple setsockopt handler */
errno_t EPHandleSet( kern_ctl_ref ctlref, unsigned int unit, void *userdata, int opt, void *data, size_t len )
{
    int    error = EINVAL;
    os_log(OS_LOG_DEFAULT, "EPHandleSet opt is %d\n", opt);
    return error;
}
 
/* A simple A simple getsockopt handler */
errno_t EPHandleGet(kern_ctl_ref ctlref, unsigned int unit, void *userdata, int opt, void *data, size_t *len)
{
    os_log(OS_LOG_DEFAULT, "EPHandleGet opt is %d *****************\n", opt); 
    /* int    error = EINVAL; */
/* #if DO_LOG */
/*     log(LOG_ERR, "EPHandleGet opt is %d *****************\n", opt); */
/* #endif */
/*     return error; */
    return 0;
}
 
/* A minimalist connect handler */
errno_t
EPHandleConnect(kern_ctl_ref ctlref, struct sockaddr_ctl *sac, void **unitinfo)
{
#if DO_LOG
    log(LOG_ERR, "EPHandleConnect called\n");
#endif
    return (0);
}
 
/* A minimalist disconnect handler */
errno_t
EPHandleDisconnect(kern_ctl_ref ctlref, unsigned int unit, void *unitinfo)
{
#if DO_LOG
    log(LOG_ERR, "EPHandleDisconnect called\n");
#endif
    return 0;
}

errno_t get_msr(int id)
{
    unsigned long long eax = 0;
    unsigned long long ecx = 0;
    unsigned long long edx = 0;
    char s[MAX_INSTRUCTION_SIZE];
    sprintf(s, "movl $%d, %%ecx", &id);
    asm volatile (s);
    asm volatile ("rdmsr");
    asm("movq %%rax,%0" : "=r"(eax));
    asm("movq %%rcx,%0" : "=r"(ecx));
    asm("movq %%rdx,%0" : "=r"(edx));
    os_log(OS_LOG_DEFAULT, "Read MSR %d. EAX: %llu, ECX: %llu, EDX: %llu", id, eax, ecx, edx);
    return 0;
}

/* A minimalist write handler */
errno_t EPHandleWrite(kern_ctl_ref ctlref, unsigned int unit, void *userdata, mbuf_t m, int flags)
{
    os_log(OS_LOG_DEFAULT, "EPHandleWrite called");
    
    // From https://stackoverflow.com/questions/11969961/kernel-communication
    size_t data_length;
    data_length = mbuf_pkthdr_len(m);
    
    char data_received[data_length+1];
    memcpy(data_received, (char*) mbuf_data(m), data_length);
    data_received[data_length] = '\0';
    
    if (memcmp(data_received, "READ", 4) == 0) {
        int msr_id;
        sscanf(data_received+5, "%d", &msr_id);
        get_msr(msr_id);
    }
    return 0;
}


kern_return_t msrutil_start(kmod_info_t * ki, void *d)
{
    os_log(OS_LOG_DEFAULT, "msrutil successfully loaded.");
    unsigned long long eax = 0;
    unsigned long long ecx = 0;
    unsigned long long edx = 0;
    for (int i = 0; i < 50; i++) {
        asm volatile ("movl $412, %ecx");
        asm volatile ("rdmsr");
        asm("movq %%rax,%0" : "=r"(eax));
        asm("movq %%rcx,%0" : "=r"(ecx));
        asm("movq %%rdx,%0" : "=r"(edx));
        os_log(OS_LOG_DEFAULT, "Read MSR IA32_THERM_STATUS. EAX: %llu, ECX: %llu, EDX: %llu", eax, ecx, edx);
    }
    asm volatile ("movl $390, %ecx");
    asm volatile ("rdmsr");
    asm("movq %%rax,%0" : "=r"(eax));
    asm("movq %%rcx,%0" : "=r"(ecx));
    asm("movq %%rdx,%0" : "=r"(edx));
    os_log(OS_LOG_DEFAULT, "Read MSR IA32_PERFEVTSEL0. EAX: %llu, ECX: %llu, EDX: %llu", eax, ecx, edx);
    errno_t error;
    struct kern_ctl_reg ep_ctl; // Initialize control
    kern_ctl_ref kctlref;
    bzero(&ep_ctl, sizeof(ep_ctl));  // sets ctl_unit to 0
    ep_ctl.ctl_id = 0; /* OLD STYLE: ep_ctl.ctl_id = kEPCommID; */
    ep_ctl.ctl_unit = 0;
    strlcpy(ep_ctl.ctl_name, "org.mklinux.nke.foo", 19+1);
    ep_ctl.ctl_flags = CTL_FLAG_PRIVILEGED & CTL_FLAG_REG_ID_UNIT;
    ep_ctl.ctl_send = EPHandleWrite;
    ep_ctl.ctl_getopt = EPHandleGet;
    ep_ctl.ctl_setopt = EPHandleSet;
    ep_ctl.ctl_connect = EPHandleConnect;
    ep_ctl.ctl_disconnect = EPHandleDisconnect;
    error = ctl_register(&ep_ctl, &kctlref);
    return KERN_SUCCESS;
}

kern_return_t msrutil_stop(kmod_info_t *ki, void *d)
{
    os_log(OS_LOG_DEFAULT, "msrutil successfully unloaded.");
    return KERN_SUCCESS;
}
 
