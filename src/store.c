#include <tcutil.h>
#include <tcbdb.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

TCBDB *store;

int nd_init_store(const char *path) {
  store = tcbdbnew();
  return tcbdbopen(store, path, BDBOWRITER | BDBOREADER | BDBOCREAT);
}

int nd_destroy_store() {
  int r = tcbdbclose(store);
  tcbdbdel(store);
  return r;
}

int nd_put(const char *kbuf, int ksiz, const char *vbuf, int vsiz) {
  return tcbdbput(store, kbuf, ksiz, vbuf, vsiz);
}

char *nd_get(const char *kbuf, int ksiz) {
  int s = 0;
  char *v = tcbdbget(store, kbuf, ksiz, &s);
  return v;
}