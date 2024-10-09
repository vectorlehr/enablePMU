#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <setjmp.h>

__attribute__((always_inline)) inline uint64_t rdpmc(uint32_t counter)
{
    uint32_t eax, edx;
    __asm__ __volatile__("rdpmc" : "=a" (eax), "=d" (edx) : "c" (counter));
    return ((uint64_t)edx << 32) | eax;
}


__attribute__((always_inline)) inline uint64_t rdtsc() {
  uint64_t a, d;
  asm volatile("mfence");

  asm volatile("rdtsc" : "=a"(a), "=d"(d));
  a = (d << 32) | a;
  asm volatile("mfence");
  return a;
}

// ---------------------------------------------------------------------------
__attribute__((always_inline)) inline void flush(void *p) { asm volatile("clflush 0(%0)\n" : : "c"(p) : "rax"); }

// ---------------------------------------------------------------------------
__attribute__((always_inline)) inline void maccess(void *p) { asm volatile("movq (%0), %%rax\n" : : "c"(p) : "rax"); }

void maccess_wr(void *p, int val) { asm volatile("movq %%rax, (%1)\n" : : "a"(val), "c"(p) : ); }

// ---------------------------------------------------------------------------
__attribute__((always_inline)) inline void mfence() { asm volatile("mfence"); }

// ---------------------------------------------------------------------------
__attribute__((always_inline)) inline void lfence() { asm volatile("lfence"); }


__attribute__((aligned(4096))) uint8_t probe_array[4096 * 300];


void benchmark(void){
    for(int i = 0; i < 300; i++){
        flush(&probe_array[i * 4096]);
    }

    uint64_t s[6], e[6];

    for(int i = 0; i < 201; i++){
        flush(&probe_array[i * 4096]);
    }

    mfence();
    
    s[1] = rdpmc(1);
    s[2] = rdpmc(2);
    s[3] = rdpmc(3);
    s[4] = rdpmc(4);
    s[5] = rdpmc(5);
    s[0] = rdpmc(0);
    mfence();

    

    for(int i = 0; i < 200; i++){
        maccess(&probe_array[i * 4096]);
    }

    mfence();
    e[0] = rdpmc(0);
    e[1] = rdpmc(1);
    e[2] = rdpmc(2);
    e[3] = rdpmc(3);
    e[4] = rdpmc(4);
    e[5] = rdpmc(5);
    mfence();

/*
# cpu1,  Core::X86::Msr::PERF_CTL0, All Data Cache Accesses 0x430729
sudo wrmsr -p 1 0xc0010200 0x430729

# cpu1, Core::X86::Msr::PERF_CTL1, retired clflush instrcutions 0x430026
sudo wrmsr -p 1 0xc0010202 0x430026

# cpu1, Core::X86::Msr::PERF_CTL2, Load Store unit dispatch 0x430029
sudo wrmsr -p 1 0xc0010204 0x430029

# cpu1, Core::X86::Msr::PERF_CTL3, Number of STLF hits 0x430035
sudo wrmsr -p 1 0xc0010206 0x430035

# cpu1, Core::X86::Msr::PERF_CTL4, prefetch instructions dispatched 0x43004b
sudo wrmsr -p 1 0xc0010208 0x43004b

# cpu1, Core::X86::Msr::PERF_CTL5, hardware prefetch data cache fils 0x43005a
sudo wrmsr -p 1 0xc001020a 0x43005a
*/
    printf("All Data Cache Accesses: %lu\n", e[0] - s[0]);
    printf("retired clflush instrcutions: %lu\n", e[1] - s[1]);
    printf("Load Store unit dispatch: %lu\n", e[2] - s[2]);
    printf("Number of STLF hits: %lu\n", e[3] - s[3]);
    printf("prefetch instructions dispatched: %lu\n", e[4] - s[4]);
    printf("hardware prefetch data cache fils: %lu\n", e[5] - s[5]);


}

int main(){
    for(int i = 0; i < 300; ++i)
        probe_array[i * 4096] = 1;

    benchmark();

    return 0;
}