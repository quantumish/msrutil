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
    printf("Thermal Status %d\n",            msr & 1);
    printf("Thermal Status Log %d\n",       (msr & (uint64_t)pow(2, 1)) > 0);
    printf("PROCHOT Event %d\n",            (msr & (uint64_t)pow(2, 2)) > 0);
    printf("PROCHOT Log %d\n",              (msr & (uint64_t)pow(2, 3)) > 0);
    printf("Critical Temperature %d\n",     (msr & (uint64_t)pow(2, 4)) > 0);
    printf("Critical Temperature Log %d\n", (msr & (uint64_t)pow(2, 5)) > 0);
    printf("Thermal Threshold #1 %d\n",     (msr & (uint64_t)pow(2, 6)) > 0);
    printf("Thermal Threshold #1 Log %d\n", (msr & (uint64_t)pow(2, 7)) > 0);
    printf("Thermal Threshold #2 %d\n",     (msr & (uint64_t)pow(2, 8)) > 0);
    printf("Thermal Threshold #2 Log %d\n", (msr & (uint64_t)pow(2, 9)) > 0);
    printf("Power Limit Notif %d\n",        (msr & (uint64_t)pow(2, 10)) > 0);
    printf("Power Limit Notif Log %d\n",    (msr & (uint64_t)pow(2, 11)) > 0);
    printf("Digital Reading %llu\n",        (msr & 0b1111111000000000000000) >> 15);
    printf("Resolution %llu\n",             (msr & 0b111100000000000000000000000000) >> 26);
    printf("Reading Valid %d\n",            (msr & (uint64_t)pow(2, 31)) > 0);
}

void read_thermal_target(uint64_t msr)
{
    printf("PROCHOT Target %llu\n",         (msr & 0b11111111000000000000000) >> 15);
    printf("TCC Activation %llu\n",         (msr & 0b111100000000000000000000000) >> 23);
}

uint64_t get_msr()
{
    FILE* fptr = fopen(PATH, "r");
    char line[1024];
    char target[3] = "EAX";
    while (fgets(line, 1024, fptr) != NULL) {};
    printf("%s", line);
    for (int i = 0; line[i] != '\0'; i++) {      
        if (strncmp(&line[i], target, 3) == 0) {
            return (uint64_t) strtoll(&line[i]+4, NULL, 10);
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
    write(fd, "READ 412", 9);
    uint64_t msr = get_msr();
    read_thermal(msr);
    /* write(fd, "READ 418", 9); */
    /* msr = get_msr(); */
    /* read_thermal_target(msr); */
}
