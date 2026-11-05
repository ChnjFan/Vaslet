// Licensed under the MIT License. See LICENSE in the project root for
// license information.

// Internal registration and parsing of command-line flags and string options.
// Positional arguments are not supported.

#ifndef VASLET_COMMON_COMMAND_LINE_H_
#define VASLET_COMMON_COMMAND_LINE_H_

#include <cstdio>
#include <string_view>
#include <vector>

namespace vaslet::common {

// Collects option registrations and values without terminating the process.
class CommandLine {
   public:
    // All registered strings, including program_name, must outlive this parser.
    explicit CommandLine(const char* program_name)
        : program_name_(program_name) {}

    // Registers a flag and its help text. Names must be unique and nonempty;
    // short_name must be a single letter, or '\0' to omit the short option.
    void AddFlag(const char* name, char short_name, const char* description);

    // Registers an option requiring a string value, with the same naming and
    // lifetime requirements as AddFlag. Empty values are allowed.
    void AddOption(const char* name, char short_name, const char* description);

    // Resets previous selections and values. Accepts --name value,
    // --name=value, -n value, -nvalue, and grouped short flags. Values
    // beginning with '-' must be attached to the option. A trailing "--" is
    // accepted. The last value of a repeated option wins. argv strings must
    // outlive subsequent Get calls. On failure, inspect error_argument() rather
    // than partial selections. Does not print or exit.
    bool Parse(int argc, const char* const argv[]);

    // Returns whether a registered option was present in the last successful
    // parse.
    bool Has(std::string_view name) const;

    // Returns a borrowed value, or an empty view for absent options and flags.
    // Use Has to distinguish an explicit empty value from an absent option.
    std::string_view Get(std::string_view name) const;

    // Returns the offending argv string after failure, or nullptr after
    // success.
    const char* error_argument() const { return error_argument_; }

    // Prints usage and descriptions in registration order.
    void PrintUsage(FILE* output = stdout) const;

   private:
    struct Option {
        const char* name;
        char short_name;
        const char* description;
        bool takes_value = false;
        bool present = false;
        const char* value = nullptr;
    };

    bool SetOption(Option& option, const char* value, int& index, int argc,
                   const char* const argv[]);

    const char* program_name_;
    std::vector<Option> options_;
    const char* error_argument_ = nullptr;
};

}  // namespace vaslet::common

#endif  // VASLET_COMMON_COMMAND_LINE_H_
