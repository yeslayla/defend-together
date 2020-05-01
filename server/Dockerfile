FROM fedora:32

# Build and setup app
RUN dnf install g++ enet-devel gtest gmock gmock-devel gtest-devel -y

ADD / /dt

RUN cd /dt; /dt/build.sh

CMD ["/dt/builds/server.out"]

# Setup status endpoint
RUN dnf install nginx -y

COPY nginx.conf /etc/nginx/conf.d/nginx.conf

RUN rm /etc/nginx/conf.d/default.conf

RUN systemctl start nginx.service


EXPOSE 80/tcp
EXPOSE 7777/udp