FROM ubuntu
RUN apt-get update
RUN apt-get install -y git
RUN apt-get install -y make
RUN apt-get install -y curl
RUN apt-get install -y clang-11.0
RUN clang++ --version
RUN git clone https://github.com/JoeydeWaal1/stock_price.git
WORKDIR stock_price/UseCase
RUN make clean
RUN make
