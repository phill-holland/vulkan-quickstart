FROM ubuntu:focal
RUN apt-get update && apt-get install build-essential wget -y
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y gdb
RUN apt-get update && apt-get install -y libjpeg-dev
RUN apt-get update && apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev -y
RUN  apt-get update && apt-get install libsdl2-dev libsdl2-mixer-dev -y
WORKDIR /init
RUN wget https://github.com/Kitware/CMake/releases/download/v3.18.4/cmake-3.18.4-Linux-x86_64.sh
RUN sh cmake-3.18.4-Linux-x86_64.sh --skip-license
RUN ln -s /init/bin/cmake /usr/local/bin/
RUN wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | apt-key add -
RUN wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.2.176-focal.list https://packages.lunarg.com/vulkan/1.2.176/lunarg-vulkan-1.2.176-focal.list
RUN apt-get update && apt-get install vulkan-sdk -y
RUN apt-get install libx11-dev -y