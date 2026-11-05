// Copyright (c) 2026 ChnjFan
//
// Licensed under the MIT License. See LICENSE in the project root for
// license information.

// Entry point for the configuration CLI skeleton. Only help and version
// options are supported; version queries use the public client library.

#include <cstdio>

#include "common/command_line.h"
#include "vaslet/vaslet.h"

int main(int argc, char* argv[]) {
    vaslet::common::CommandLine arguments("vasletctl");
    arguments.AddFlag("help", 'h', "Show this help message");
    arguments.AddFlag("version", 'v', "Show the program version");
    if (!arguments.Parse(argc, argv)) {
        std::fprintf(stderr, "vasletctl: invalid argument: %s\n",
                     arguments.error_argument());
        arguments.PrintUsage(stderr);
        return 1;
    }
    if (arguments.Has("help")) {
        arguments.PrintUsage();
        std::puts("Configuration commands are not implemented yet.");
        return 0;
    }
    if (arguments.Has("version")) {
        std::printf("vasletctl %s\n", vaslet::Version());
        return 0;
    }
    std::fputs(
        "vasletctl: configuration commands are not implemented yet.\n"
        "Use --help for available options.\n",
        stderr);
    return 1;
}
