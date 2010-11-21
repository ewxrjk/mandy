#include <glibmm.h>
#include <stdio.h>
#include "images.h"

int main() {
  fwrite(logodata, 1, sizeof logodata, stdout);
  if(ferror(stdout)
     || fclose(stdout) < 0) {
    perror("stdout");
    return 1;
  }
  return 0;
}
