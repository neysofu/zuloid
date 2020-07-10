FROM frolvlad/alpine-glibc

RUN apk --no-cache add cmake clang clang-dev make gcc g++ libc-dev linux-headers
 
COPY . .
 
RUN rm -rf build/ && \
    mkdir build/ && \
    cd build/ && \
    cmake .. && \
    make && \
    ./zorro_test

ENTRYPOINT [ "build/ZORRO" ]
