FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    curl \
    pkg-config \
    libssl-dev \
    libcurl4-openssl-dev \
    zlib1g-dev \
    libpq-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /opt
RUN git clone --branch master https://github.com/jtv/libpqxx.git
WORKDIR /opt/libpqxx
RUN cmake -S . -B build -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS=ON \
    && cmake --build build --parallel \
    && cmake --install build

WORKDIR /app

COPY . .

RUN cmake -S . -B build && cmake --build build --parallel


ENV LD_LIBRARY_PATH=/usr/local/lib
CMD ["./build/DisparadorArca"]


CMD ["./build/DisparadorArca"]
