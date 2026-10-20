#include "poolmon.h"
R__LOAD_LIBRARY(libpoolmon.so)

void poolmon(const char * filename)
{
  if ( filename != NULL)
    {
      pfileopen(filename);
    }
}
