#include "vaslet/vaslet.h"

int main() {
  const char* version = vaslet::Version();
  return version == nullptr || version[0] == '\0';
}
