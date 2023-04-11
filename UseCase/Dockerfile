FROM alpine:3.14
# RUN apk update
RUN apk upgrade
RUN apk add make
RUN apk add curl-dev
RUN apk add curl
RUN apk add clang
RUN apk add clang-dev
RUN apk add linux-headers
RUN apk add g++
RUN apk add libc-dev
COPY main.cpp ./main.cpp
COPY Makefile ./Makefile
COPY start.sh start.sh
RUN make


CMD ./main ${APIKEY} ${TB_ACCESSTOKEN}
 # ["./main",${APIKEY},${TB_ACCESSTOKEN}]
