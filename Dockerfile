FROM debian:10-slim

RUN apt-get update && \
    apt-get -y install cmake make clang gcc

# https://stackoverflow.com/questions/62780558/
WORKDIR /zuloid

COPY . .
 
RUN rm -rf build/ && \
    mkdir build/ && \
    cd build/ && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make && \
    strip zuloid && \
    ./test_zuloid

ENTRYPOINT [ "build/zuloid" ]
