FROM debian:latest

# Install dependencies
RUN apt update -y && apt install -y build-essential git cmake fish nasm
RUN mkdir /ft_nm
WORKDIR /ft_nm
COPY ./inc ./inc
COPY ./src ./src
COPY ./libft ./libft
COPY ./sample ./sample
COPY ./Makefile ./Makefile
COPY ./CMakeLists.txt ./CMakeLists.txt
COPY ./test.sh ./test.sh

# Build
RUN make


# Run
CMD ["fish"]