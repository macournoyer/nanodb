#define UNUSED(expr)  do { (void)(expr); } while (0)

int nd_init_store(const char *path);
int nd_destroy_store();

int nd_put(const char *kbuf, int ksiz, const char *vbuf, int vsiz);
char *nd_get(const char *kbuf, int ksiz);

void nd_server_start(int port);
void nd_server_stop();

enum { ND_NONE, ND_GET, ND_PUT };

typedef void (*done_cb)(void *data);

struct nd_parser {
  int cs;
  int method;
  char *key;
  int key_len;
  done_cb done_cb;
  void *data;
};

void nd_parser_init(struct nd_parser *parser, done_cb done_cb, void *data);
void nd_parse(struct nd_parser *parser, char *buf, int size);
