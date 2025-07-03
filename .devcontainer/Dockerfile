# Fetch ubuntu image
FROM ubuntu:22.04

# Install prerequisites
RUN \
    apt update && \
    apt install -y cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential && \
    apt install -y binutils-arm-none-eabi openocd make libusb-1.0-0-dev git python3
    
# create a directory for the project
RUN \
    mkdir -p /work/ && \
    cd /work && \
    git clone https://github.com/libopencm3/libopencm3 && \
    cd /work/libopencm3 && \
    make


# Clone source code for the project
RUN \
    git clone https://github.com/RaphNLab/RE46C109_Programmer && 

    
# Command that will be invoked when the container starts
ENTRYPOINT ["/bin/bash"]