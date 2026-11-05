// Copyright (c) 2026 ChnjFan
//
// Licensed under the MIT License. See LICENSE in the project root for
// license information.

// Entry point for the configuration service skeleton. Parses a configuration
// path and handles help and version options; the service is not implemented.

#include <cstdio>
#include <cstdlib>
#include <string>

#include "common/command_line.h"
#include "defaults.h"
#include "vaslet_version.h"

void parse_options(int argc, char* argv[], std::string& config_path) {
    vaslet::common::CommandLine arguments("vasletd");
    arguments.AddFlag("help", 'h', "Show this help message");
    arguments.AddFlag("version", 'v', "Show the program version");
    arguments.AddOption("config", 'c', "Path to the configuration file");
    if (!arguments.Parse(argc, argv)) {
        std::fprintf(stderr, "vasletd: invalid argument: %s\n",
                     arguments.error_argument());
        arguments.PrintUsage(stderr);
        std::exit(1);
    }
    if (arguments.Has("help")) {
        arguments.PrintUsage();
        std::puts("Configuration service is not implemented yet.");
        std::exit(0);
    }
    if (arguments.Has("version")) {
        std::printf("vasletd %s\n", VASLET_VERSION);
        std::exit(0);
    }

    if (arguments.Has("config")) {
        config_path = arguments.Get("config");
    } else {
        config_path = VASLETD_DEFAULT_CONFIG_PATH;
    }
}

int main(int argc, char* argv[]) {
    std::string config_path;
    parse_options(argc, argv, config_path);

    return 1;
}
