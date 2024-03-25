FROM ubuntu:23.10 as teda-build

WORKDIR /

RUN apt-get update && \
    apt-get install -y build-essential cmake cmake-curses-gui vim git ninja-build

RUN apt-get install -y "libqt6*"

RUN apt-get install qt6-base-dev

RUN mkdir /build-jkqtplotter && \
    cd /build-jkqtplotter && \
    git clone https://github.com/jkriege2/JKQtPlotter.git

RUN mkdir /build-jkqtplotter/build && \
    cd /build-jkqtplotter/build && \
    cmake ../JKQtPlotter -GNinja -DJKQtPlotter_ENABLED_CXX20=ON -DJKQtPlotter_BUILD_LIB_JKQTFAST=OFF
    
RUN cd /build-jkqtplotter/build && \
    ninja && \
    ninja install

RUN apt-get install -y wget dpkg-dev binutils libx11-dev libxpm-dev libxft-dev libxext-dev python3 libssl-dev

RUN mkdir -p /root-system/build && \
    cd /root-system && \
    wget https://root.cern/download/root_v6.30.04.source.tar.gz && \
    tar -zvxf root_v6.30.04.source.tar.gz

RUN cd /root-system/build && \
    cmake ../root-6.30.04 -GNinja -DCMAKE_CXX_STANDARD=20 -Dmathmore=ON -Dminuit2=ON -Droot7=OFF -Dtmva=OFF -Dwebgui=OFF -Dx11=OFF -Dxrootd=OFF -Dgdml=OFF && \
    ninja && \
    ninja install

RUN mkdir -p /teda/build && \
    cd /teda && \
    git clone https://github.com/dungducphan/TauExperimentDataAnalyzer.git

RUN cd /teda/TauExperimentDataAnalyzer && \
    git checkout dungducphan/feature_DockerBuildAndDeploy && \
    git fetch && \
    git pull && \
    apt-get install -y libtiff-dev && \
    cd /teda/build && \
    cmake ../TauExperimentDataAnalyzer -GNinja

RUN cd /teda/build && \
    ninja





