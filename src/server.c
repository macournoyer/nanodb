#define EV_STANDALONE 1
#include <ev.c>

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>

#define READ_SIZE  1024

int fd;
struct ev_loop *loop;
ev_io accept_watcher;
ev_signal signal_watcher;


/** Connections **/

struct nd_connection {
  int fd;
  ev_io read_watcher;
  ev_io write_watcher;
};

void nd_connection_close(struct nd_connection *c) {
  ev_io_stop(loop, &c->read_watcher);
  ev_io_stop(loop, &c->write_watcher);
  close(c->fd);
  shutdown(c->fd, SHUT_RDWR);
  free(c);
}

static void readable_cb(EV_P_ struct ev_io *watcher, int revents) {
  char buf[READ_SIZE];
  struct nd_connection *c = (struct nd_connection*) watcher->data;
  
  size_t received = recv(c->fd, buf, READ_SIZE, 0);
  
  if (received == -1) {
    /* error, TODO close connection */
    return;
  }
  
  if (received == 0) {
    /* received 0 byte, read again next loop */
    return;
  }
  
  printf("%s", buf);
  
  // Close after one request for now
  nd_connection_close(c);
}

void nd_new_connection(int cfd) {
  puts("Got new connection");
  struct nd_connection *c = (struct nd_connection*)malloc(sizeof(struct nd_connection));
  c->fd = cfd;
  c->read_watcher.data = c->write_watcher.data = c;
  
  ev_io_init(&c->read_watcher, readable_cb, c->fd, EV_READ);
  // ev_io_init(&c->write_watcher, writable_cb, c->fd, EV_WRITE);
  
  /* start event watchers */
  ev_io_start(loop, &c->read_watcher);
}


/** Server **/

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
  
  nd_new_connection(cfd);
}

static void sigint_cb(struct ev_loop *loop, ev_signal *w, int revents) {
  puts("Got INT signal, stopping ...");
  ev_unloop (loop, EVUNLOOP_ALL);
}


void nd_bind(int port) {
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

void nd_install_signals() {
  ev_signal_init(&signal_watcher, sigint_cb, SIGINT);
  ev_signal_start(loop, &signal_watcher);
}

void nd_server_start(int port) {
  loop = ev_default_loop(0);
  
  nd_install_signals();
  nd_bind(port);
  
  ev_loop(loop, 0);
}

void nd_server_stop() {
  ev_io_stop(loop, &accept_watcher);
  shutdown(fd, SHUT_RDWR);
  close(fd);
}
