// Licensed under the MIT License. See LICENSE in the project root for
// license information.

#include "command_line.h"

namespace vaslet::common {

void CommandLine::AddFlag(const char* name, char short_name,
                          const char* description) {
    options_.push_back({name, short_name, description});
}

void CommandLine::AddOption(const char* name, char short_name,
                            const char* description) {
    options_.push_back({name, short_name, description, true});
}

bool CommandLine::SetOption(Option& option, const char* value, int& index,
                            int argc, const char* const argv[]) {
    if (!option.takes_value && value != nullptr) {
        error_argument_ = argv[index];
        return false;
    }
    if (option.takes_value && value == nullptr) {
        if (index + 1 == argc || argv[index + 1][0] == '-') {
            error_argument_ = argv[index];
            return false;
        }
        value = argv[++index];
    }
    option.present = true;
    option.value = value;
    return true;
}

bool CommandLine::Parse(int argc, const char* const argv[]) {
    error_argument_ = nullptr;
    for (auto& option : options_) {
        option.present = false;
        option.value = nullptr;
    }

    for (int i = 1; i < argc; ++i) {
        const std::string_view argument(argv[i]);
        if (argument == "--") {
            if (i + 1 == argc) {
                return true;
            }
            error_argument_ = argv[i + 1];
            return false;
        }
        if (argument.size() < 2 || argument[0] != '-') {
            error_argument_ = argv[i];
            return false;
        }
        if (argument[1] == '-') {
            const auto separator = argument.find('=');
            const auto name = argument.substr(
                2, separator == std::string_view::npos ? separator
                                                       : separator - 2);
            const char* value = separator == std::string_view::npos
                                    ? nullptr
                                    : argv[i] + separator + 1;
            Option* match = nullptr;
            for (auto& option : options_) {
                if (name == option.name) {
                    match = &option;
                    break;
                }
            }
            if (match == nullptr) {
                error_argument_ = argv[i];
                return false;
            }
            if (!SetOption(*match, value, i, argc, argv)) {
                return false;
            }
        } else {
            for (std::size_t offset = 1; offset < argument.size(); ++offset) {
                Option* match = nullptr;
                for (auto& option : options_) {
                    if (option.short_name != '\0' &&
                        argument[offset] == option.short_name) {
                        match = &option;
                        break;
                    }
                }
                if (match == nullptr) {
                    error_argument_ = argv[i];
                    return false;
                }
                const char* value =
                    match->takes_value && offset + 1 < argument.size()
                        ? argv[i] + offset + 1
                        : nullptr;
                if (!SetOption(*match, value, i, argc, argv)) {
                    return false;
                }
                // The rest of this short-option group belongs to its value.
                if (match->takes_value) {
                    break;
                }
            }
        }
    }
    return true;
}

bool CommandLine::Has(std::string_view name) const {
    for (const auto& option : options_) {
        if (name == option.name) {
            return option.present;
        }
    }
    return false;
}

std::string_view CommandLine::Get(std::string_view name) const {
    for (const auto& option : options_) {
        if (name == option.name && option.value != nullptr) {
            return option.value;
        }
    }
    return {};
}

void CommandLine::PrintUsage(FILE* output) const {
    std::fprintf(output, "Usage: %s [options]\nOptions:\n", program_name_);
    for (const auto& option : options_) {
        const char* value_hint = option.takes_value ? " <value>" : "";
        if (option.short_name != '\0') {
            std::fprintf(output, "  -%c, --%s%s  %s\n", option.short_name,
                         option.name, value_hint, option.description);
        } else {
            std::fprintf(output, "      --%s%s  %s\n", option.name, value_hint,
                         option.description);
        }
    }
}

}  // namespace vaslet::common
