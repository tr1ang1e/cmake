#include <stdio.h>

#include "ProjectConfig.h"

int main(int argc, char* argv[])
{
  printf("%s version %d.%d \n", argv[0], PROJECTNAME_VERSION_MAJOR, PROJECTNAME_VERSION_MINOR);

  return 0;
}