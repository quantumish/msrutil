#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/kern_control.h>
#include <sys/sys_domain.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define PATH "/Users/davidfreifeld/test.log"

void read_thermal(uint64_t msr)
{
    printf("Thermal Status %llu", msr & 1);
    printf("Thermal Status Log %llu", msr & (uint64_t)pow(2, 1));
    printf("PROCHOT Event %llu", msr & (uint64_t)pow(2, 2));
    printf("PROCHOT Log %llu", msr & (uint64_t)pow(2, 3));
    printf("Critical Temperature %llu", msr & (uint64_t)pow(2, 4));
    printf("Critical Temperature Log %llu", msr & (uint64_t)pow(2, 5));
    printf("Thermal Threshold #1 %llu", msr & (uint64_t)pow(2, 6));
    printf("Thermal Threshold #1 Log %llu", msr & (uint64_t)pow(2, 7));
    printf("Thermal Threshold #2 %llu", msr & (uint64_t)pow(2, 8));
    printf("Thermal Threshold #2 Log %llu", msr & (uint64_t)pow(2, 9));
    printf("Power Limit Notif %llu", msr & (uint64_t)pow(2, 10));
    printf("Power Limit Notif Log %llu", msr & (uint64_t)pow(2, 11));
}

uint64_t get_msr()
{
    FILE* fptr = fopen(PATH, "r");
    char line[1024];
    char* target = "EAX";
    while (fgets(line, 1024, fptr) != NULL) {};
    for (int i = 0; line[i] != '\0'; i++) {
        if (strcmp(&line[i], target) == 0) {
            return (uint64_t) strtoll(&line[i], NULL, 10);
        }
    }
    printf("That's all folks!\n");
}

int main() {
    struct sockaddr_ctl addr;
    int ret = 1;
    int result;
    int fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    if (fd != -1) {
        bzero(&addr, sizeof(addr)); // sets the sc_unit field to 0
        addr.sc_len = sizeof(addr);
        addr.sc_family = AF_SYSTEM;
        addr.ss_sysaddr = AF_SYS_CONTROL;
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
        result = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
        if (result) {
            fprintf(stderr, "connect failed %d\n", result);
        }
    } else { /* no fd */
        fprintf(stderr, "failed to open socket\n");
    }
    unsigned char msg[] = "READ 412";
    write(fd, msg, 9);
}
