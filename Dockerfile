FROM debian:10-slim

RUN apt-get update && \
    apt-get -y install cmake make clang gcc

# https://stackoverflow.com/questions/62780558/
WORKDIR /zorro

COPY . .
 
RUN rm -rf build/ && \
    mkdir build/ && \
    cd build/ && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$((2 * $(nproc))) && \
    strip zorro && \
    ./test_zorro

ENTRYPOINT [ "build/zorro" ]
