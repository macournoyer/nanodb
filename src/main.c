#include <stdio.h>
#include "nd.h"

int main (int argc, char const *argv[]) {
  UNUSED(argc);
  UNUSED(argv);
  
  nd_init_store("test.ndb");
  
  nd_put("x", 1, "ohaie", 5);
  // puts(nd_get("x", 1));
  
  nd_server_start(5000);
  nd_server_stop();
  
  nd_destroy_store();
  
  return 0;
}