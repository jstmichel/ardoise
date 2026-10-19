# Contributing to Ardoise

## Requirements

- CMake 3.21 or newer.
- A C++20-capable compiler: GCC, Clang/Apple Clang, or MSVC.
- Qt 6.2 or newer with Core, Gui, Widgets, SVG, and LinguistTools development files.
- Ninja for the commands below; other CMake generators are supported.

Standalone tests additionally require Qt Test. No SQL, Network, WebSockets,
sqlite3, Python runtime, or sibling application repository is required.

## Window behavior

MainWindow and Dialog preserve native platform title bars, buttons, movement and
resizing while the content uses the Ardoise theme. Consumer code should use
setWindowTitle/setWindowIcon and place its application menu in its content. The
former titleBar() and MainWindow::setApplicationMenu() APIs have been removed;
TitleBar remains a standalone widget for existing explicit consumers. Offscreen
tests verify window flags and dialog close/Escape behavior; verify actual OS
decorations on each target desktop.

## Platform setup

### Linux

Install a C++ compiler, CMake, Ninja, and Qt 6 base, SVG, and tools development packages from your
distribution. The Qt Widgets CMake package must be discoverable by CMake.

### macOS

Install Xcode Command Line Tools and CMake, Ninja, and a desktop Qt 6 development
kit. Build with the compiler and architecture matching that Qt kit. Set
`CMAKE_PREFIX_PATH` to the Qt installation prefix if CMake cannot locate it.

### Windows

Install Visual Studio Build Tools with the C++ workload and a matching Qt 6 desktop
kit. Run Ninja commands from a developer shell for the selected compiler. Install
CMake and Ninja if they are not available in that shell.

Match the Qt kit to the compiler and architecture: use an MSVC Qt kit with MSVC or
a MinGW Qt kit with its matching MinGW compiler. Make the Qt kit's `bin` directory
available on `PATH` when running programs that require its runtime DLLs.

The visual framework targets all three platforms, but only Linux has been
validated with offscreen interaction and rendering tests. Native macOS and Windows validation is pending.

## Configure, build, and test

Run from the repository root:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

If Qt is outside CMake's default search paths, add
`-DCMAKE_PREFIX_PATH="/absolute/path/to/Qt/6.x/compiler-kit"` to configuration.
On Windows, use the corresponding kit path such as
`C:/Qt/6.x/msvc2022_64`, replacing the version and kit with your actual installation.

For a multi-configuration generator:

```sh
cmake -S . -B build-multi -G "Ninja Multi-Config"
cmake --build build-multi --config Debug
ctest --test-dir build-multi -C Debug --output-on-failure
```

The single-configuration example executable is
`build/examples/consumer/ardoise-consumer` (`.exe` on Windows). Multi-configuration
builds place it under `build-multi/examples/consumer/Debug/` for Debug builds.

### Options

| Option | Standalone default | Embedded default | Purpose |
| --- | --- | --- | --- |
| `ARDOISE_BUILD_EXAMPLES` | `ON` | `OFF` | Build the integration consumer and UI gallery. |
| `ARDOISE_INSTALL` | `ON` | `OFF` | Generate installation rules and the CMake package. |
| `BUILD_TESTING` | `ON` | Controlled by the parent | Register tests only in standalone builds. |

For a library-only build:

```sh
cmake -S . -B build-library -G Ninja -DARDOISE_BUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF
cmake --build build-library
```

## Installation and consumer integration

The public target is always `Ardoise::Widgets`. Its requirements propagate to
consumers; applications do not need to repeat the framework's include paths or
explicitly link Qt Widgets for this dependency.

### Source checkout

An application can add the repository using `add_subdirectory()` as shown in
README. To check the independent example against the source checkout:

```sh
cmake -S examples/consumer -B build-source-consumer -G Ninja -DARDOISE_SOURCE_DIR="$PWD"
cmake --build build-source-consumer
```

In PowerShell, replace `"$PWD"` with `"$($PWD.Path)"` in CMake arguments.

### Installed package

After building:

```sh
cmake --install build --prefix "$PWD/build/install"
cmake -S examples/consumer -B build-package-consumer -G Ninja -DCMAKE_PREFIX_PATH="$PWD/build/install"
cmake --build build-package-consumer
```

For multi-configuration builds, pass `--config Debug` to installation and build
commands. Provide both the framework prefix and Qt prefix in `CMAKE_PREFIX_PATH`
if Qt is not otherwise discoverable; quote the semicolon-separated list.

The package installs the static archive, public headers, and CMake configuration.
Its prefix can be moved; Qt must remain discoverable. Rebuild for each target
compiler, architecture, and platform. The package does not deploy Qt runtime
libraries or platform plugins.

## Validation

CTest builds and runs independent consumers that link only `Ardoise::Widgets`:

- `ardoise-source-consumer` consumes the checkout with `add_subdirectory()`.
- `ardoise-installed-consumer` installs into an isolated build directory, moves
  that prefix, and consumes the relocated package with `find_package()`.

These checks exercise public headers, transitive C++20 and Qt Widgets requirements,
static linking of actual widgets/icons/translations, version output, and package relocation. Build the framework before
running CTest. The installation test is omitted when `ARDOISE_INSTALL=OFF`.
All generated test data stays under the build directory.

The remaining suites cover theme rendering, controls and window behavior.
Run `build/examples/gallery/ardoise-gallery` for interactive review. Take the
README screenshots again with `scripts/screenshots.sh [build-directory]` and review
the changed images in `docs/screenshots/` before committing them.
Check keyboard focus, hover/pressed/disabled states, text, high DPI and real
window movement/resizing on available desktops. Native macOS/Windows checks
remain necessary; offscreen results do not certify compositor behavior.
See [design-system.md](docs/design-system.md) for component and platform policy.

## Development and documentation

Shared editing icons (ZoomOut, ZoomIn, ActualSize, Crop, Upscale, Downscale, Paperclip, Sparkles, IdentityEdit, Sharpen, Send, Upload and Image) are
monochrome SVG assets bundled through resources/icons.qrc and IconProvider.
Append public icon enum values to preserve existing values; keep the resource
map synchronized. WidgetsTest renders every icon, including the editing assets.

Follow [AGENTS.md](AGENTS.md). Keep documentation and code comments in English;
`README.fr.md` is the French translation of `README.md` and changes with it.
Add only requested functionality and update the documentation in the same change
when build requirements, APIs, layout, or integration behavior change. A new
component also goes into the gallery, the README catalog and its screenshot.

Bugs and ideas start as GitHub issues, using the templates. An idea is discussed
there before any code.

Use Conventional Commits: `type(optional-scope): imperative summary`.
Generated build files and local installation prefixes must not be committed.

## Continuous integration and releases

`.github/workflows/ci.yml` builds and runs every test on Linux (the distribution's
Qt), macOS and Windows (Qt 6.8) for each push to `main` and each pull request.

To release, set the new version in `project()` of `CMakeLists.txt`, add
`docs/release-notes/<version>.md`, merge, then push a `v<version>` tag.
`.github/workflows/release.yml` checks that the tag matches the project version,
builds and tests the tagged source, and attaches the source archive,
`scripts/get-ardoise.sh` and their `SHA256SUMS` to a draft release, which the
maintainer reviews and publishes. Consumers pin that tag in `FetchContent`;
`get-ardoise.sh` installs the latest published release. The `ardoise-get-ardoise`
test runs the installer against a fake release with a stub `cmake`.
