FROM         ubuntu:18.04
MAINTAINER   Jethro kuan <jethrokuan5@gmail.com>
CMD          bash

# Required system packages
RUN apt-get update && apt-get -y install \
  apt-utils \
  build-essential \
  curl \
  doxygen \
  git \
  tar \
  wget \
  xz-utils \
  cmake \
  clang

ENV \
  CC="/usr/bin/clang" \
  CXX="/usr/bin/clang++"
