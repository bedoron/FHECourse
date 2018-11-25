FROM alpine:latest

FROM ubuntu:18.04
MAINTAINER Doron <b.e.doron@gmail.com>

# Install binary dependencies
RUN apt-get -qqy update && apt-get install -qqy \
	g++ \
	git \
	make \
    cmake \
    build-essential \
    libgflags-dev \
	--no-install-recommends

RUN mkdir -p SEAL/
COPY /SEAL/ /SEAL/
WORKDIR /SEAL/

RUN rm -rf cmake; \
    rm -rf CMakeFiles; \
    rm -rf CMakeCache*; \
    ls -al;


