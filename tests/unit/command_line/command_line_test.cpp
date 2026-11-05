// Licensed under the MIT License. See LICENSE in the project root for
// license information.

#include "common/command_line.h"

#include <gtest/gtest.h>

#include <string_view>
#include <vector>

namespace vaslet::tests::command_line {
namespace {

class CommandLineTest : public ::testing::Test {
   protected:
    void SetUp() override {
        parser_.AddFlag("help", 'h', "Show help");
        parser_.AddFlag("version", 'v', "Show version");
        parser_.AddFlag("quiet", '\0', "Suppress output");
        parser_.AddOption("config", 'c', "Configuration path");
        parser_.AddOption("output", '\0', "Output path");
    }

    vaslet::common::CommandLine parser_{"test"};
};

TEST_F(CommandLineTest, ParsesFlagsAndResetsSelections) {
    struct Case {
        std::vector<const char*> arguments;
        bool valid;
        bool help;
        bool version;
        bool quiet;
        const char* error;
    };
    const Case cases[] = {
        {{"test", "--help"}, true, true, false, false, nullptr},
        {{"test"}, true, false, false, false, nullptr},
        {{"test", "-v"}, true, false, true, false, nullptr},
        {{"test", "-hv"}, true, true, true, false, nullptr},
        {{"test", "--quiet", "-h", "--version"},
         true,
         true,
         true,
         true,
         nullptr},
        {{"test", "--help", "-h"}, true, true, false, false, nullptr},
        {{"test", "--version", "--"}, true, false, true, false, nullptr},
        {{"test", "--"}, true, false, false, false, nullptr},
        {{"test", "--help", "--unknown"},
         false,
         false,
         false,
         false,
         "--unknown"},
        {{"test", "-hx"}, false, false, false, false, "-hx"},
        {{"test", "-q"}, false, false, false, false, "-q"},
        {{"test", "--he"}, false, false, false, false, "--he"},
        {{"test", "--help=yes"}, false, false, false, false, "--help=yes"},
        {{"test", "-"}, false, false, false, false, "-"},
        {{"test", ""}, false, false, false, false, ""},
        {{"test", "file"}, false, false, false, false, "file"},
        {{"test", "--", "--help"}, false, false, false, false, "--help"},
        {{"test", "--quiet"}, true, false, false, true, nullptr},
    };
    for (const auto& test : cases) {
        SCOPED_TRACE(::testing::PrintToString(test.arguments));
        ASSERT_EQ(parser_.Parse(static_cast<int>(test.arguments.size()),
                                test.arguments.data()),
                  test.valid);
        if (test.valid) {
            EXPECT_EQ(parser_.Has("help"), test.help);
            EXPECT_EQ(parser_.Has("version"), test.version);
            EXPECT_EQ(parser_.Has("quiet"), test.quiet);
            EXPECT_FALSE(parser_.Has("unregistered"));
            EXPECT_EQ(parser_.error_argument(), nullptr);
        } else {
            EXPECT_STREQ(parser_.error_argument(), test.error);
        }
    }
}

TEST_F(CommandLineTest, ParsesStringValuesAndResetsState) {
    struct ValueCase {
        std::vector<const char*> arguments;
        bool valid;
        bool has_config;
        std::string_view config;
        std::string_view output;
        bool help;
        const char* error;
    };
    const ValueCase value_cases[] = {
        {{"test", "-c", "path"}, true, true, "path", "", false, nullptr},
        {{"test", "--config", "a path/file"},
         true,
         true,
         "a path/file",
         "",
         false,
         nullptr},
        {{"test", "--config=a=b"}, true, true, "a=b", "", false, nullptr},
        {{"test", "-cpath"}, true, true, "path", "", false, nullptr},
        {{"test", "-hcpath"}, true, true, "path", "", true, nullptr},
        {{"test", "-hc", "path"}, true, true, "path", "", true, nullptr},
        {{"test", "-ch"}, true, true, "h", "", false, nullptr},
        {{"test", "--config=-file"}, true, true, "-file", "", false, nullptr},
        {{"test", "-c-file"}, true, true, "-file", "", false, nullptr},
        {{"test", "--config="}, true, true, "", "", false, nullptr},
        {{"test", "-c", ""}, true, true, "", "", false, nullptr},
        {{"test", "-c", "first", "--config=last"},
         true,
         true,
         "last",
         "",
         false,
         nullptr},
        {{"test", "--output", "result", "-c", "path"},
         true,
         true,
         "path",
         "result",
         false,
         nullptr},
        {{"test", "--output=result"},
         true,
         false,
         "",
         "result",
         false,
         nullptr},
        {{"test"}, true, false, "", "", false, nullptr},
        {{"test", "--config"}, false, false, "", "", false, "--config"},
        {{"test", "-c"}, false, false, "", "", false, "-c"},
        {{"test", "-hc"}, false, false, "", "", false, "-hc"},
        {{"test", "--config", "--help"},
         false,
         false,
         "",
         "",
         false,
         "--config"},
        {{"test", "-c", "--"}, false, false, "", "", false, "-c"},
        {{"test", "-c", "-file"}, false, false, "", "", false, "-c"},
        {{"test", "--unknown=path"},
         false,
         false,
         "",
         "",
         false,
         "--unknown=path"},
        {{"test", "--quiet="}, false, false, "", "", false, "--quiet="},
        {{"test", "-c", "path", "extra"}, false, false, "", "", false, "extra"},
        {{"test", "--config=path", "--"},
         true,
         true,
         "path",
         "",
         false,
         nullptr},
        {{"test", "--help"}, true, false, "", "", true, nullptr},
    };
    for (const auto& test : value_cases) {
        SCOPED_TRACE(::testing::PrintToString(test.arguments));
        ASSERT_EQ(parser_.Parse(static_cast<int>(test.arguments.size()),
                                test.arguments.data()),
                  test.valid);
        if (test.valid) {
            EXPECT_EQ(parser_.Has("config"), test.has_config);
            EXPECT_EQ(parser_.Get("config"), test.config);
            EXPECT_EQ(parser_.Get("output"), test.output);
            EXPECT_EQ(parser_.Has("help"), test.help);
            EXPECT_TRUE(parser_.Get("help").empty());
            EXPECT_TRUE(parser_.Get("unregistered").empty());
            EXPECT_EQ(parser_.error_argument(), nullptr);
        } else {
            EXPECT_STREQ(parser_.error_argument(), test.error);
        }
    }
}

}  // namespace
}  // namespace vaslet::tests::command_line
