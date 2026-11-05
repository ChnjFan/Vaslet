// Copyright (c) 2026 ChnjFan
//
// Licensed under the MIT License. See LICENSE in the project root for
// license information.

#include <gtest/gtest.h>

#include "vaslet/vaslet.h"

namespace vaslet::tests::api {
namespace {

TEST(ClientApiTest, ReturnsNonemptyVersion) {
    const char* version = vaslet::Version();
    ASSERT_NE(version, nullptr);
    EXPECT_STRNE(version, "");
}

}  // namespace
}  // namespace vaslet::tests::api
