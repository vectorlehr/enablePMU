#include <stdint.h>

#if defined (USE_ARMV8_INSTRUCTIONS)
#define BR_MIS_PRED             0x10
#define BR_PRED                 0x12
#define INST_SPEC               0x1B
#define  BR_MIS_PRED_RETIRED    0x22
#elif defined (USE_X86_64_INSTRUCTIONS)
#define BR_MIS_PRED             0x0
#define BR_PRED                 0x0
#define INST_SPEC               0x0
#define  BR_MIS_PRED_RETIRED    0x0
#else
#error Instruction set not supported
#endif


void set_pmu_event(int reg_no,int event)
{
#if defined(USE_ARMV8_INSTRUCTIONS)
    asm volatile("isb");
    switch(reg_no){
    case 0:
        asm volatile("msr pmevtyper0_el0, %0" : : "r" (event));
        asm volatile("mrs x0, pmcntenset_el0");
        asm volatile("orr x0,x0,#1");
        asm volatile("msr pmcntenset_el0, x0");
        break;
    case 1:
        asm volatile("msr pmevtyper1_el0, %0" : : "r" (event));
        asm volatile("mrs x0, pmcntenset_el0");
        asm volatile("orr x0,x0,#2");
        asm volatile("msr pmcntenset_el0, x0");
        break;
    case 2:
        asm volatile("msr pmevtyper2_el0, %0" : : "r" (event));
        asm volatile("mrs x0, pmcntenset_el0");
        asm volatile("orr x0,x0,#4");
        asm volatile("msr pmcntenset_el0, x0");
        break;
    case 3:
        asm volatile("msr pmevtyper3_el0, %0" : : "r" (event));
        asm volatile("mrs x0, pmcntenset_el0");
        asm volatile("orr x0,x0,#8");
        asm volatile("msr pmcntenset_el0, x0");
        break;
    case 4:
        asm volatile("msr pmevtyper4_el0, %0" : : "r" (event));
        asm volatile("mrs x0, pmcntenset_el0");
        asm volatile("orr x0,x0,#16");
        asm volatile("msr pmcntenset_el0, x0");
        break;
    case 5:
        asm volatile("msr pmevtyper5_el0, %0" : : "r" (event));
        asm volatile("mrs x0, pmcntenset_el0");
        asm volatile("orr x0,x0,#16");
        asm volatile("msr pmcntenset_el0, x0");
        break;
    }
#endif
}

uint32_t read_count(int reg_no)
{
    uint32_t r = 0;

#if defined(USE_ARMV8_INSTRUCTIONS)
	asm volatile("mrs %0, pmevcntr0_el0" : "=r" (r)); 
    switch(reg_no){
    case 0:
        asm volatile("mrs %0, pmevcntr0_el0" : "=r" (r)); 
        break;
    case 1:
        asm volatile("mrs %0, pmevcntr1_el0" : "=r" (r)); 
        break;
    case 2:
        asm volatile("mrs %0, pmevcntr2_el0" : "=r" (r)); 
        break;
    case 3:
        asm volatile("mrs %0, pmevcntr3_el0" : "=r" (r)); 
        break;
    case 4:
        asm volatile("mrs %0, pmevcntr4_el0" : "=r" (r)); 
        break;
    case 5:
        asm volatile("mrs %0, pmevcntr5_el0" : "=r" (r)); 
        break;
    }
#endif
    return r;
}

void clear_pmu_event()
{
#if defined(USE_ARMV8_INSTRUCTIONS)
    asm volatile("mrs x0, pmcntenset_el0");
    asm volatile("and x0,x0,#0xffffffc0");
    asm volatile("msr pmcntenset_el0, x0");
#endif
}