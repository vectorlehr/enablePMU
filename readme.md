



## 使能
使能EL0对PMU的访问，需要在EL1下进行操作以下几个寄存器：
- PMUSERENR_EL0： 使能EL0对PMU的访问
- PMCR_EL0： 使能PMU
- PMCNTENSET_EL0： 使能PMU的计数器