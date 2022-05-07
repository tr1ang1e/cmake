#include "common.h"
#include "counters.h"

int increment_counter(int* counter)
{
    return (*counter)++;
}

int print_proj_version_counter = 0;
void print_proj_version()
{
  _TR_;
  increment_counter(&print_proj_version_counter);

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
  printf("%s \n", _LC_);
  printf("print_proj_version() called %d times \n", print_proj_version_counter);

  #ifdef mv_LOGGING_on
    print_log(3, "zer", "one", "two");
    print_log(3, "thr", "fou", "fiv");
    printf("print_log() called %d times \n", print_log_counter);
  #endif

  print_exportable();

  return 0;
}