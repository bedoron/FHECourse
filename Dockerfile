FROM ubuntu:18.04
MAINTAINER Doron <b.e.doron@gmail.com>

# Install binary dependencies
RUN apt-get -qqy update && apt-get install -qqy \
	clang \
	git \
	make \
    cmake \
    build-essential \
    libgflags-dev \
    libgsl-dev \
    ca-certificates \
	--no-install-recommends

RUN mkdir -p /opt/
WORKDIR /opt/
RUN git clone https://github.com/Microsoft/SEAL.git
WORKDIR /opt/SEAL/src
RUN cmake -DCMAKE_BUILD_TYPE=Debug . && make install

RUN mkdir -p /app/src
COPY CMakeLists.txt /app
COPY ./src /app/src
WORKDIR /app
RUN find . -iwholename '*cmake*' -not -name CMakeLists.txt -delete && cmake -DCMAKE_BUILD_TYPE=Debug . && make

CMD /app/bin/fhecourse