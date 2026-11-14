# Project Guidelines

## Purpose

Vaslet is a lightweight JSON configuration center for Linux. It aims to give
applications a simple, reliable way to manage and retrieve configuration without
the operational overhead of a large configuration platform.

The project is intended for public development on GitHub under the MIT License.

## Project Principles

- Keep Linux as the primary development and runtime target. Avoid adding
  cross-platform abstractions without a concrete need.
- Keep JSON at the center of the configuration model. Prefer standard JSON
  semantics over custom syntax or implicit type conversions.
- Favor a small runtime footprint, few dependencies, and straightforward
  installation and operation.
- Prefer clear interfaces and predictable behavior over feature breadth or
  premature abstraction.
- Treat configuration integrity as a core requirement. Validate input before
  accepting changes, report failures clearly, and avoid corrupting existing
  configuration when an operation fails.
- Make behavior explicit: document defaults, validation rules, persistence
  guarantees, and failure behavior as those capabilities are implemented.
- Keep the initial scope focused. Distributed consensus, clustering, complex
  plugin systems, and large management interfaces are not baseline requirements;
  introduce them only when a concrete use case justifies their cost.

## Repository Language

- Write all repository content in English, including documentation, code
  comments, identifiers, configuration examples, test fixtures, scripts, and
  text embedded in assets.
- Write application messages, diagnostics, and error text in English.
- Conversation with maintainers may use their preferred language; this does not
  change the English-only requirement for repository files.

## Implementation and Contributions

- No implementation language, framework, build system, storage backend, or
  communication protocol has been selected yet. Do not treat this document or
  existing ignore patterns as an implicit technology decision.
- When making foundational choices, explain how they support the project's
  lightweight Linux focus and document the chosen approach in English.
- Keep changes focused on the requested work. Avoid speculative infrastructure
  and unrelated refactoring.
- Keep setup and usage documentation aligned with implemented behavior. Clearly
  distinguish planned capabilities from working features.
- Add verification appropriate to the change. For configuration handling, cover
  malformed JSON, invalid input, and relevant read/write failure paths when
  those behaviors are implemented.
- Do not commit credentials, private configuration, generated build artifacts,
  or machine-specific files. Avoid exposing configuration secrets in logs or
  diagnostics.

## C++ Coding Style

- Follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
  when writing, reviewing, or submitting project C++ code, including naming,
  formatting, header organization, comments, and language feature usage.
- Apply this requirement to project-owned code. Preserve the upstream style of
  third-party dependencies under `thirdparty/`.
- Git commit messages must follow the Git Commit Guidelines below.

## Git Commit Guidelines

- Use Conventional Commits for every commit message, written in English.
- Use the header format `<type>[optional scope][!]: <description>`.
- Use one of these types: `feat`, `fix`, `docs`, `style`, `refactor`, `perf`,
  `test`, `build`, `ci`, `chore`, or `revert`.
- Use `feat` for new functionality, `fix` for bug fixes, `build` for build system
  or dependency changes, and `style` for formatting changes that do not affect
  behavior. Choose the other types according to the purpose of the change.
- Keep the type and optional scope lowercase. Use a short, meaningful scope
  such as `config`, `server`, or `deps` when it helps identify the affected area.
- Write a concise description in the imperative mood, without a trailing period.
- Always include a body separated from the header by a blank line. List the
  specific changes as bullet points, and explain the reason for the change and
  any relevant behavior or compatibility impact when needed.
- Mark breaking changes with `!` before the colon and include a
  `BREAKING CHANGE: <explanation>` footer describing the required migration.
- Keep each commit focused on one logical change. Do not include unrelated
  changes in the same commit.

Examples:

```text
feat(config): add JSON configuration validation

- Validate configuration input before accepting changes
- Report malformed JSON without exposing configuration secrets
```

```text
fix(server): preserve configuration when a write fails

- Keep the existing configuration intact when persistence fails
- Report write failures to the caller
```

```text
build(deps): add RapidJSON submodule

- Add RapidJSON under thirdparty/rapidjson
- Register the upstream repository in .gitmodules
```

```text
docs: document commit message conventions

- Require Conventional Commits headers written in English
- Require a blank line followed by a list of specific changes
```
