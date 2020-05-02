FROM fedora:32

# Build and setup app
RUN dnf install g++ enet-devel gtest gmock gmock-devel gtest-devel -y

ADD / /dt

RUN cd /dt; /dt/build.sh

CMD ["/dt/builds/server.out"]

EXPOSE 7777/udp