# get the model name of the CPU
model_name=$(cat /proc/cpuinfo | grep "model name" | head -n 1 | awk -F ': ' '{print $2}')
    # check the microarchitecture code name of CPU
    # check if the CPU is Intel
    # if [[ $model_name == *"Intel"* ]]; then
    # check if the CPU is Intel Cascade Lake
    if [[ $model_name == *"Xeon"* ]]; then
        microarch="csl"
    # check if the CPU is Skylake
    elif [[ $model_name == *"Core"* && $model_name == *"-6"* ]]; then
        microarch="skl"
    # check if the CPU is Kaby Lake
    elif [[ $model_name == *"Core"* && $model_name == *"-7"* ]]; then
        microarch="kbl"
    # check if the CPU is Coffee Lake
    elif [[ $model_name == *"Core"* && $model_name == *"-8"* ]]; then
        microarch="cfl"
    # check if the CPU is Coffe Lake Refresh
    elif [[ $model_name == *"Core"* && $model_name == *"-9"* ]]; then
        microarch="cfl"
    # check if the CPU is Comet Lake
    elif [[ $model_name == *"Core"* && $model_name == *"-10"* ]]; then
        microarch="cml"
    # check if the CPU if Tiger Lake
    elif [[ $model_name == *"Core"* && $model_name == *"-11"* ]]; then
        microarch="tgl"
    else
        echo -e  "${RED}[-]${NC}" "Unsupported CPU model: $model_name, please contact the author for support to update the PMU enabling."
        exit 1
    fi

    if [[ $microarch == "csl" || $microarch == "skl" ]]; then
        # run the script named after $model_name + '.sh' to enable the PMU
        echo -e "${GREEN}[+]${NC}" "The CPU is $GREEN$microarch$NC, enabling the PMU..."
        # sudo modprobe msr
        cd enpmu_x86
        sudo ./$microarch.sh
        cd -
    else
        echo -e "${RED}[-]${NC}" "The CPU is not supported, please contact the author for support to update the PMU enabling."
        exit 1
    fi