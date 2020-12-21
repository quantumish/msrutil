#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/kern_control.h>
#include <sys/sys_domain.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    struct sockaddr_ctl       addr;
    int                       ret = 1;
    int result;
    int fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    if (fd != -1) {
        bzero(&addr, sizeof(addr)); // sets the sc_unit field to 0
        addr.sc_len = sizeof(addr);
        addr.sc_family = AF_SYSTEM;
        addr.ss_sysaddr = AF_SYS_CONTROL;
#ifdef STATIC_ID
        addr.sc_id = kEPCommID;  // should be unique - use a registered Creator ID here
        addr.sc_unit = kEPCommUnit;  // should be unique.
#else
        {
            struct ctl_info info;
            memset(&info, 0, sizeof(info));
            strncpy(info.ctl_name, "org.mklinux.nke.foo", sizeof(info.ctl_name));
            if (ioctl(fd, CTLIOCGINFO, &info)) {
                perror("Could not get ID for kernel control.\n");
                exit(-1);
            }
            addr.sc_id = info.ctl_id;
            addr.sc_unit = 0;
        }
#endif
        result = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
        if (result) {
            fprintf(stderr, "connect failed %d\n", result);
        }
    } else { /* no fd */
        fprintf(stderr, "failed to open socket\n");
    }

    /* if (!result) { */
    /*     result = setsockopt( fd, SYSPROTO_CONTROL, 2, NULL, 0); */
    /*     if (result){ */
    /*         fprintf(stderr, "setsockopt failed on kEPCommand1 call - result was %d\n", result); */
    /*     } */
    /* } */
    unsigned char msg[] = "abc";
    send(fd, msg, 3, 10);
    //result = getsockopt( fd, SYSPROTO_CONTROL, 2, NULL, 0);
}
