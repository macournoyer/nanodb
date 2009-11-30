#define UNUSED(expr)  do { (void)(expr); } while (0)

int nd_init_store(const char *path);
int nd_destroy_store();

int nd_put(const char *kbuf, int ksiz, const char *vbuf, int vsiz);
char *nd_get(const char *kbuf, int ksiz);

void nd_server_start(int port);
void nd_server_stop();
