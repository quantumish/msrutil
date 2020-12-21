#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/kern_control.h>
#include <sys/kern_event.h>
#include <sys/sys_domain.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main()
{
    int fd = socket(PF_SYSTEM, SOCK_RAW, SYSPROTO_EVENT);
    struct kev_request req;
    if (ioctl(fd, SIOCGKEVFILT, &req)) {
        perror("SIOCSKEVFILT");
        exit(-1);
    }
    printf("The current filter is vendor code %d, class %d, subclass %d\n",
           req.vendor_code, req.kev_class, req.kev_subclass);
}
