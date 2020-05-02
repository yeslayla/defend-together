FROM fedora:32

# Build and setup app
RUN dnf install g++ enet-devel gtest gmock gmock-devel gtest-devel nginx -y

ADD / /dt

RUN cd /dt; /dt/build.sh

RUN echo "Container is healthy!" > /usr/share/nginx/html/index.html

CMD  /usr/sbin/nginx ; /dt/builds/server.out

EXPOSE 80
EXPOSE 7777/udp