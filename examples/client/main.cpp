#include <cstdio>

#include "vaslet/vaslet.h"

int main() {
  std::printf("Vaslet client library %s\n", vaslet::Version());
  return 0;
}
