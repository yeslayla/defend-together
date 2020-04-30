FROM fedora:32

RUN /bin/bash -c "dnf install g++ enet-devel gtest gmock gmock-devel gtest-devel -y"

ADD / /dt

RUN cd /dt; /dt/build.sh

CMD ["./builds/server.out"]