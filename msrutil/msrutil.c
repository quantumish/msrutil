//
//  msrutil.c
//  msrutil
//
//  Created by David Freifeld on 5/2/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

// FORGET-ME-NOT: Build solely ~/projects/msrutil/build/Products/Debug/msrutil.kext and


#include <sys/systm.h>
#include <sys/kern_control.h>
#include <os/log.h>
#include <mach/mach_types.h>

#define MSR_GET 0
#define MSR_TO_READ_SET 0
#define MAX_STRING_LEN 20
#define DEFAULT_STRING "default"

//Included in Xcode template, not sure why.
kern_return_t msrutil_start(kmod_info_t * ki, void *d);
kern_return_t msrutil_stop(kmod_info_t *ki, void *d);

char g_string_buf[10];
static boolean_t g_filter_registered = FALSE;
static kern_ctl_ref g_ctl_ref;

static int msrutil_get(kern_ctl_ref ctl_ref, u_int32_t unit, void *unitinfo, int opt,
                         void *data, size_t *len)
{
    int ret = 0;
    switch (opt) {
        case (MSR_GET):
            *len = min(MAX_STRING_LEN, *len);
            strncpy(data, g_string_buf, *len);
            break;
        default:
            ret = ENOTSUP;
            break;
    }
    return ret;
}

static int msrutil_set(kern_ctl_ref ctl_ref, u_int32_t unit, void* unitinfo, int opt,
                         void* data, size_t len)
{
    int ret = 0;
    switch (opt) {
        case MSR_TO_READ_SET:
            strncpy(g_string_buf, (char*)data, min(MAX_STRING_LEN, len));
            printf("MSR_TO_READ_SET: new string set to: \"%s\"\n", g_string_buf);
            break;
        default:
            ret = ENOTSUP;
            break;
    }
    return ret;
}

static int msrutil_connect(kern_ctl_ref ctl_ref, struct sockaddr_ctl *sac, void** unitinfo)
{
    os_log(OS_LOG_DEFAULT, "Process with PID %d connected.", proc_selfpid());
    return 0;
}
static int msrutil_disconnect(kern_ctl_ref ctl_ref, uint32_t unit, void* unitinfo)
{
    os_log(OS_LOG_DEFAULT, "Process with PID %d disconnected.", proc_selfpid());
    return 0;
}

kern_return_t msrutil_start(kmod_info_t * ki, void *d)
{
    os_log(OS_LOG_DEFAULT, "msrutil successfully loaded.");
    
    static struct kern_ctl_reg userctl = {
        "quantumish.msrutil",
        0,
        0,
        CTL_FLAG_PRIVILEGED,
        0,
        0,
        msrutil_connect,
        msrutil_disconnect,
        NULL,
        msrutil_get,
        msrutil_set
    };
    

    int ret = ctl_register(&userctl, &g_ctl_ref);
    
    uint32_t lo, hi;
    // Example off of stackoverflow to test assembly
    // NOTE: Figure out why kernel (and my computer) crashes when I run non-volatile assembly that isn't just 'nop'!
    asm volatile("rdmsr":"=a"(lo),"=d"(hi):"c"(0x1b1));
    
    os_log(OS_LOG_DEFAULT, "Contents of MSR? lo: %d hi: %d", lo, hi);
    //    register int *eax asm ("eax");
    //    register int *ecx asm ("ecx");
    //    register int *edx asm ("edx");
    //    printf("%i%i%i", *eax, *ecx, *edx);
    strncpy(g_string_buf, DEFAULT_STRING, strlen(DEFAULT_STRING));
    
    if (ret == KERN_SUCCESS)
    {
        g_filter_registered = TRUE;
        return KERN_SUCCESS;
    }
    return KERN_FAILURE;
}

kern_return_t msrutil_stop(kmod_info_t *ki, void *d)
{
    if (g_filter_registered)
        ctl_deregister(g_ctl_ref);
    
    os_log(OS_LOG_DEFAULT, "msrutil successfully unloaded.");
    return KERN_SUCCESS;
}
