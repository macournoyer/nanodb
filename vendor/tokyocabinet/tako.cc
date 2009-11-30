#include <iostream>
#include <tchdb.h>
#include <tcutil.h>
#include <string.h>
struct THING
{
  int id;
  char buffer[128];
  double d;

};

int main(int argc, char** argv)
{

  TCHDB* _hdb = tchdbnew();
  if ( _hdb == NULL)
  {
    std::cerr << "create error " << '\n';
    return 1;
  }

  /* open the database */
  if (!tchdbopen(_hdb, "test_cabinet.tch", HDBOCREAT|HDBOWRITER|HDBOTRUNC) )
  {
    int ecode = tchdbecode(_hdb);
    std::cerr << "open error: " <<  tchdberrmsg(ecode) << '\n';
    return 1;

  }

  THING thing;
  memset(&thing, 0, sizeof(THING));

  //1253614487.229357 2 :590000 records
  for (int i = 0; i < 1000000; ++i)
  {

    thing.id = i;
    // // now put into the cabinet
    if ( !tchdbputasync(_hdb, &i, sizeof(int), &thing, sizeof
(THING) ) )
    {
      int ecode = tchdbecode(_hdb);
      std::cerr << "put error: " << tchdberrmsg(ecode) << '\n';
      return 1;
    }
    if ( (i % 10000) == 0)
    {
      std::cerr << i << " records\n" ;
    }

  }

  if ( !tchdbsync(_hdb) )
  {
    int ecode = tchdbecode(_hdb);
    std::cerr << "sync error: " <<  tchdberrmsg(ecode) << '\n';
    return 1;

  }

  if (!tchdbclose(_hdb) )
  {
    int ecode = tchdbecode(_hdb);
    std::cerr << "close error: " <<  tchdberrmsg(ecode) << '\n';
    return 1;

  }

  tchdbdel(_hdb);
  return 0;

}
