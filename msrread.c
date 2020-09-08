#include <stdio.h>
#include <stdint.h>

void IA32_THERM_STATUS_READ(uint64_t eax)
{
    printf("(%llu & %llu) %llu deg C\n", (eax >> 16), 0x111111, (eax >> 16) & 0x111111);
}

int main()
{
    IA32_THERM_STATUS_READ(2284191744);
    return 0;
}
