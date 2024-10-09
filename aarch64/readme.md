



## 使能
使能EL0对PMU的访问，需要在EL1下进行操作以下几个寄存器：

### PMUSERENR_EL0

    使能EL0对PMU的访问

**ER, bit [3]**

Event counter Read. Traps EL0 access to event counters to EL1, or to EL2 when it is implemented and enabled for the current Security state and HCR_EL2.TGE is 1.

In AArch64 state, trapped accesses are reported using EC syndrome value 0x18. 

In AArch32 state, trapped accesses are reported using EC syndrome value 0x03.

**0b0:** EL0 using AArch64: EL0 reads of the PMXEVCNTR_EL0 and PMEVCNTR<n>_EL0, and EL0 read/write accesses to the PMSELR_EL0, are trapped if PMUSERENR_EL0.EN is also 0.
EL0 using AArch32: EL0 reads of the PMXEVCNTR and PMEVCNTR<n>, and EL0 read/write accesses to the PMSELR, are trapped if PMUSERENR_EL0.EN is also 0.

**0b1:** Overrides PMUSERENR_EL0.EN and enables:
- RO access to PMXEVCNTR_EL0 and PMEVCNTR<n>_EL0 at EL0.
- RW access to PMSELR_EL0 at EL0.
- RW access to PMSELR at EL0.

The reset behavior of this field is:
- On a Warm reset, this field resets to an architecturally UNKNOWN value.

---

**CR bit[2]**

Cycle counter Read. Traps EL0 access to cycle counter reads to EL1, or to EL2 when it is implemented and enabled for the current Security state and HCR_EL2.TGE is 1.

In AArch64 state, trapped accesses are reported using EC syndrome value 0x18.

In AArch32 state, trapped MRC accesses are reported using EC syndrome value 0x03, trapped MRRC accesses are reported using EC syndrome value 0x04.

**0b0** EL0 using AArch64: EL0 read accesses to the PMCCNTR_EL0 are trapped if PMUSERENR_EL0.EN is also 0. 
EL0 using AArch32: EL0 read accesses to the PMCCNTR are trapped if PMUSERENR_EL0.EN is also 0.

**0b1** Overrides PMUSERENR_EL0.EN and enables access to:
- PMCCNTR_EL0 at EL0.
- PMCCNTR at EL0. The reset behavior of this field is:
- On a Warm reset, this field resets to an architecturally UNKNOWN value.

---

**SW bit[1]**

Traps Software Increment writes to EL1, or to EL2 when it is implemented and enabled for the current Security state and HCR_EL2.TGE is 1.

In AArch64 state, trapped accesses are reported using EC syndrome value 0x18. 

In AArch32 state, trapped accesses are reported using EC syndrome value 0x03.

**0b0** EL0 using AArch64: EL0 writes to the PMSWINC_EL0 are trapped if PMUSERENR_EL0.EN is also 0.
EL0 using AArch32: EL0 writes to the PMSWINC are trapped if PMUSERENR_EL0.EN is also 0.

**0b1** Overrides PMUSERENR_EL0.EN and enables access to:
- PMSWINC_EL0 at EL0.
- PMSWINC at EL0. The reset behavior of this field is:
- On a Warm reset, this field resets to an architecturally UNKNOWN value.

---


**EN bit[0]**

Traps EL0 accesses to the Performance Monitor registers to EL1, or to EL2 when it is implemented and enabled for the current Security state and HCR_EL2.TGE is 1, from both Execution states as follows:

- In AArch64 state, MRS or MSR accesses to the following registers are reported using EC syndrome value 0x18:

    + PMCR_EL0, PMOVSCLR_EL0, PMSELR_EL0, PMCEID0_EL0, PMCEID1_EL0, PMCCNTR_EL0, PMXEVTYPER_EL0, PMXEVCNTR_EL0, PMCNTENSET_EL0, PMCNTENCLR_EL0, PMOVSSET_EL0, PMEVCNTR<n>_EL0, PMEVTYPER<n>_EL0, PMCCFILTR_EL0, PMSWINC_EL0.

    - If FEAT_PMUv3p4 is implemented, PMMIR_EL1.

- In AArch32 state, MRC or MCR accesses to the following registers are reported using EC
syndrome value 0x03:
    + PMCR, PMOVSR, PMSELR, PMCEID0, PMCEID1, PMCCNTR, PMXEVTYPER, PMXEVCNTR, PMCNTENSET, PMCNTENCLR, PMOVSSET, PMEVCNTR<n>, PMEVTYPER<n>, PMCCFILTR, PMSWINC.
    - If FEAT_PMUv3p4 is implemented, PMMIR.
    - If FEAT_PMUv3p1 is implemented, in AArch32 state, PMCEID2, and PMCEID3.

+ In AArch32 state, MRRC or MCRR accesses to PMCCNTR are reported using EC syndrome
value 0x04.

**0b0** While at EL0, accesses to the specified registers at EL0 are trapped, unless overridden by one of PMUSERENR_EL0.{ER, CR, SW}.


**0b1** While at EL0, software can access all of the specified registers. The reset behavior of this field is:
- On a Warm reset, this field resets to an architecturally UNKNOWN value.

> Tips： 直接将低4位全部设置成1即可，即0x0f



### PMCR_EL0
    详细描述PMU的实现，包括计数器的数量，事件的数量等，以及用于配制和控制PMU

---

**LP bit[7]**

When FEAT_PMUv3p5 is implemented:
Long event counter enable. Determines when unsigned overflow is recorded by an event counter overflow bit.
In the description of this field:
0b0
0b1
- If EL2 is implemented and is using AArch32, PMN is HDCR.HPMN.
- If EL2 is implemented and is using AArch64, PMN is MDCR_EL2.HPMN. 
- If EL2 is not implemented, PMN is PMCR_EL0.N.

**0b0** Event counter overflow on increment that causes unsigned overflow of PMEVCNTR<n>_EL0[31:0]. 

**0b1** Event counter overflow on increment that causes unsigned overflow of PMEVCNTR<n>_EL0[63:0].

If PMN is not 0, this field affects the operation of event counters in the range [0 .. (PMN-1)]. This field does not affect the operation of other event counters and PMCCNTR_EL0.

The operation of this field applies even when EL2 is disabled in the current Security state. The reset behavior of this field is:
- On a Warm reset, this field resets to an architecturally UNKNOWN value.


**Otherwise:**
Reserved, RES0.


> Tips: 一般情况下，将LC和E设置为1即可，即0x41


### PMCNTENSET_EL0
    使能PMCCNTR_EL0和PMEVCNTR<n>_EL0的计数器，读这个寄存器可以知道哪几个计数器是使能的

---

**C bit[31]**

PMCCNTR_EL0 enable bit. Enables the cycle counter register. Possible values are:

**0b0** When read, means the cycle counter is disabled. When written, has no effect.

**0b1** When read, means the cycle counter is enabled. When written, enables the cycle counter.

The reset behavior of this field is:

- On a Warm reset, this field resets to an architecturally UNKNOWN value.

---

**P<n>, bit[n], for n=0 to 30**

Event counter enable bit for PMEVCNTR<n>_EL0.

If N is less than 31, then bits [30:N] are RAZ/WI. When EL2 is implemented and enabled in the current Security state, in EL1 and EL0, N is the value in MDCR_EL2.HPMN. Otherwise, N is the value in PMCR_EL0.N.

**0b0** When read, means that PMEVCNTR<n>_EL0 is disabled. When written, has no effect.

**0b1** When read, means that PMEVCNTR<n>_EL0 event counter is enabled. When written, enables PMEVCNTR<n>_EL0.

The reset behavior of this field is:

- On a Warm reset, this field resets to an architecturally UNKNOWN value.

