#include <cstdio>
#include <cstring>

#include "vaslet_version.h"

int main(int argc, char* argv[]) {
  if (argc == 2 && std::strcmp(argv[1], "--help") == 0) {
    std::puts(
        "Usage: vasletd --help | --version\n"
        "Configuration service is not implemented yet.");
    return 0;
  }
  if (argc == 2 && std::strcmp(argv[1], "--version") == 0) {
    std::printf("vasletd %s\n", VASLET_VERSION);
    return 0;
  }
  std::fputs(
      "vasletd: configuration service is not implemented yet.\n"
      "Use --help for available options.\n",
      stderr);
  return 1;
}
