FROM debian:stable-20210816-slim
WORKDIR /WORKDIR
RUN apt-get update && \
    apt-get --yes install make cmake g++ libssl-dev
COPY . .
RUN cmake ./ && make && cp libkleinsHTTP.so libkleinsHTTP.a /usr/lib/ && cp libkleinsHTTP.h /usr/include
