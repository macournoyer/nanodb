#include <tcutil.h>
#include <tcbdb.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

TCBDB *store;

void nd_init_store() {
  store = tcbdbnew();
}