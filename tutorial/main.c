#include "common.h"

#ifdef mv_LOGGING_on
    #include "logging.h"
#else
    #define _TR_
#endif

void print_proj_version()
{
  _TR_;
  printf
  (
    "Project version %d.%d.%d.%d \n", 
     VERSION_MAJOR,
     VERSION_MINOR,
     VERSION_PATCH,
     VERSION_TWEAK
  );
}

int main(int argc, char** argv)
{
  _TR_;
  print_proj_version();

  #ifdef mv_LOGGING_on
    print_log(3, "zer", "one", "two");
  #endif

  return 0;
}