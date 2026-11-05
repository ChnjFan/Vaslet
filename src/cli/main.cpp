#include <cstdio>
#include <cstring>

#include "vaslet/vaslet.h"

int main(int argc, char* argv[]) {
  if (argc == 2 && std::strcmp(argv[1], "--help") == 0) {
    std::puts(
        "Usage: vasletctl --help | --version\n"
        "Configuration commands are not implemented yet.");
    return 0;
  }
  if (argc == 2 && std::strcmp(argv[1], "--version") == 0) {
    std::printf("vasletctl %s\n", vaslet::Version());
    return 0;
  }
  std::fputs(
      "vasletctl: configuration commands are not implemented yet.\n"
      "Use --help for available options.\n",
      stderr);
  return 1;
}
