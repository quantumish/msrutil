//
//  msrutil.c
//  msrutil
//
//  Created by David Freifeld on 5/2/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

// FORGET-ME-NOT: Build solely ~/projects/msrutil/build/Products/Debug/msrutil.kext and

#define BUFSIZE 2048
#define PORT 5001

#include "msrutil.h"

#include <sys/systm.h>
#include <sys/kern_control.h>
#include <os/log.h>
#include <mach/mach_types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>

// Included in Xcode template, not sure why.
/* kern_return_t msrutil_start(kmod_info_t * ki, void *d); */
/* kern_return_t msrutil_stop(kmod_info_t *ki, void *d); */


kern_return_t msrutil_start(kmod_info_t * ki, void *d)
{
    os_log(OS_LOG_DEFAULT, "msrutil successfully loaded.");

    int s;
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        os_log(OS_LOG_DEFAULT, "Could not start socket.");
        return KERN_FAILURE;
    }

    // Establish sockaddr_in struct to pass into bind function
    struct sockaddr_in addr;
    memset((char *)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; // Specify address family.
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY just 0.0.0.0, machine IP address
    addr.sin_port = htons(PORT); // Specify port.
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        os_log(OS_LOG_DEFAULT, "Could not bind socket.");
        return KERN_FAILURE;
    }

    // IP address of client
    struct sockaddr_in remaddr;
    socklen_t addrlen = sizeof(remaddr);
    int recvlen;
    unsigned char buf[BUFSIZE];

    while (1==1) {
        char direction[BUFSIZE];
        char args[BUFSIZE];
        char response[BUFSIZE];
        recvlen = recvfrom(s, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
        sscanf(buf, "%s %s", direction, args);
        if (strcmp(direction, "MSR") == 0) {
          uint32_t lo, hi;
          // Example off of stackoverflow to test assembly
          // NOTE: Figure out why kernel (and my computer) crashes when I run
          // non-volatile assembly that isn't just 'nop'!
          asm volatile("rdmsr" : "=a"(lo), "=d"(hi) : "c"(0x1b1));
          sprintf(response, "Contents of MSR: %d %d", lo, hi);
          sendto(s, response, strlen(response)+1, 0, (struct sockaddr *)&remaddr, addrlen);
        }
    }
    return KERN_SUCCESS;
}

kern_return_t msrutil_stop(kmod_info_t *ki, void *d)
{
    os_log(OS_LOG_DEFAULT, "msrutil successfully unloaded.");
    return KERN_SUCCESS;
}
