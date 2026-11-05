// Copyright (c) 2026 ChnjFan
//
// Licensed under the MIT License. See LICENSE in the project root for
// license information.

// Minimal example of including and linking the public Vaslet client API.

#include <cstdio>

#include "vaslet/vaslet.h"

int main() {
    std::printf("Vaslet client library %s\n", vaslet::Version());
    return 0;
}
