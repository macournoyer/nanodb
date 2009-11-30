#define EV_STANDALONE 1
#include <ev.c>

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>

int fd;

struct ev_loop *loop;
ev_io accept_watcher;

static void new_connection(int cfd, struct sockaddr_in remote_addr) {
  puts("got new connection");

  close(cfd);
  shutdown(cfd, SHUT_RDWR);
}

static void accept_cb(EV_P_ struct ev_io *watcher, int revents) {
  struct sockaddr_in remote_addr;
  socklen_t sin_size = sizeof(remote_addr);
  int flags;
  
  int cfd = accept(fd, (struct sockaddr *)&remote_addr, &sin_size);
  
  if (cfd == -1) {
    assert(0); // TODO handle...
    return;
  }
  
  flags = fcntl(cfd, F_GETFL, 0);
  if ((fcntl(cfd, F_SETFL, flags | O_NONBLOCK)) < 0) {
    assert(0); // TODO handle...
    return;
  }
  
  new_connection(cfd, remote_addr);
}

void nd_server_bind(int port) {
  int ret;
  int sock_flags = 1;
  struct sockaddr_in local_addr;
  
  memset(&local_addr, 0, sizeof(local_addr));
  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(port);
  local_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
  
  fd = socket(AF_INET, SOCK_STREAM, 0);
  assert(fd > 0);

  /* set socket as beeing nonblocking */
  ret = fcntl(fd, F_SETFL, O_NONBLOCK);
  assert(ret != -1);
  
  ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &sock_flags, sizeof(sock_flags));
  assert(ret != -1);
  
  bind(fd, (struct sockaddr *)&local_addr, sizeof(local_addr));
  assert(ret != -1);

  listen(fd, 10); // listen backlog
  assert(ret != -1);
  
  // initialize watchers
  ev_io_init(&accept_watcher, accept_cb, fd, EV_READ);
  ev_io_start(loop, &accept_watcher);
}

void nd_server_start(int port) {
  loop = ev_default_loop(0);
  
  nd_server_bind(port);
  
  ev_loop(loop, EVLOOP_ONESHOT);
}

void nd_server_stop() {
  ev_io_stop(loop, &accept_watcher);
  shutdown(fd, SHUT_RDWR);
  close(fd);
}
