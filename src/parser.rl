#include <assert.h>
#include "nd.h"

%%{
  machine nanodb;
  
  action get   { parser->method = ND_GET; }
  action put   { parser->method = ND_PUT; }
  
  action skey  { parser->key = (char*)p; }
  action ekey  { parser->key_len = pe - p; }
  
  action done  { parser->done_cb(parser->data); }
  
  nl     = "\n" | ";";
  ws     = " ";
  
  method = "GET" %get
         | "PUT" %put;
  
  char   = [a-zA-Z0-9_];
  key    = char+ >skey %ekey;
  
  request = method ws key (nl @done);
  
  main := request;
}%%

%% write data;

void nd_parser_init(struct nd_parser *parser, done_cb done_cb, void *data) {
  int cs = 0;
  %% write init;
  parser->cs = cs;
  parser->method = ND_NONE;
  parser->done_cb = done_cb;
  parser->data = data;
  parser->key = 0;
  parser->key_len = 0;
}

void nd_parse(struct nd_parser *parser, char *buf, int size) {
  const char *p, *pe, *eof = 0;
  int cs = parser->cs;

  p = buf;
  pe = buf + size;

  %% write exec;

  parser->cs = cs;

  assert(p <= pe && "buffer overflow after parsing execute");
}