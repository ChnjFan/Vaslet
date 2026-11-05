// Copyright (c) 2026 ChnjFan
//
// Licensed under the MIT License. See LICENSE in the project root for
// license information.

// Public Vaslet client API. Currently provides version queries only.

#ifndef VASLET_VASLET_H_
#define VASLET_VASLET_H_

#if defined(__GNUC__) || defined(__clang__)
#define VASLET_API __attribute__((visibility("default")))
#else
#define VASLET_API
#endif

namespace vaslet {

// Returns the library version. The string is static and must not be freed.
VASLET_API const char* Version() noexcept;

}  // namespace vaslet

#endif  // VASLET_VASLET_H_
