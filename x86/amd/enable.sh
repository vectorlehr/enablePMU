#!/bin/bash

# enable AMD PMU

sudo bash -c "echo 2 > /sys/bus/event_source/devices/cpu/rdpmc || exit"

sudo modprobe msr

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