FROM ubuntu
RUN apt-get update
RUN apt-get upgrade
RUN apt-get install -y make
RUN apt-get install -y libcurl4-openssl-dev
RUN apt-get install -y curl
RUN apt-get install -y clang-11.0
COPY main.cpp ./main.cpp
COPY Makefile ./Makefile
RUN make

CMD ./main ${APIKEY} ${TB_ACCESSTOKEN}
