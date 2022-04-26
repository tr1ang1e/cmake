#include <stdio.h>
#include "proj_version.h"

#define _TR_ printf(" ::  %s : %s : %d \n", __FILE__, __func__,  __LINE__)

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

  return 0;
}