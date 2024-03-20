FROM ubuntu:22.04 as teda-build

WORKDIR /

RUN apt-get update && \
    apt-get install -y build-essential cmake cmake-curses-gui vim git ninja-build

RUN apt-get install -y "libqt6*"

RUN apt-get install qt6-base-dev



