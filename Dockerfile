FROM ubuntu:18.04

LABEL \
  maintainer="jethrokuan"


RUN apt-get update -qq

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
  git \
  mercurial \
  curl \
  wget \
  python \
  build-essential\
  ca-certificates

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
  cmake \
  clang

ENV \
  CC="/usr/bin/clang" \
  CXX="/usr/bin/clang++"
