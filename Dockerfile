FROM ubuntu:18.04
RUN apt-get -y update && apt-get -y install \
    build-essential \
    git \
    cmake \
    libntl-dev

RUN mkdir -p /opt/build/src 
WORKDIR /opt/build

RUN git clone https://github.com/shaih/HElib.git && mkdir HElib/_build
WORKDIR /opt/build/HElib/
RUN git pull
WORKDIR /opt/build/HElib/_build
RUN cmake .. && make
RUN cp lib/* /usr/local/lib/
WORKDIR /opt/build/HElib
RUN cp src/*.h /usr/local/include/
RUN rm -rf /opt/build/HElib/_build
WORKDIR /opt/build/src
COPY src .
WORKDIR /opt/build/
RUN cmake ./src && make

CMD "/opt/build/fhecourse"