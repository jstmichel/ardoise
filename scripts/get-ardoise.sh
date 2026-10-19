#!/bin/sh
# Builds and installs an Ardoise release as a CMake package, on Linux or macOS:
#
#   curl -fsSL https://github.com/jstmichel/ardoise/releases/latest/download/get-ardoise.sh | sh
#
# Usage: get-ardoise.sh [--version <tag>] [--prefix <dir>] [--qt <dir>] [--dry-run] [--help]
#
#   --version <tag>  install this release (for example v0.1.0) instead of the latest
#   --prefix <dir>   install under this folder (default: ~/.local)
#   --qt <dir>       Qt installation to build against, when CMake does not find it
#   --dry-run        download and check, then print the build commands without running them
#
# Requires curl, tar, CMake 3.21+, a C++20 compiler and Qt 6.2+ with Widgets, SVG and
# LinguistTools. Exit codes: 0 installed or already installed, 1 failure.

set -u

releases=${ARDOISE_RELEASES_URL:-https://github.com/jstmichel/ardoise/releases}
tag=""
prefix="${HOME}/.local"
qt=""
dry_run=0

usage() {
    sed -n '2,15s/^# \{0,1\}//p' "$0" 2>/dev/null \
        || echo "Usage: get-ardoise.sh [--version <tag>] [--prefix <dir>] [--qt <dir>] [--dry-run]"
}

say() {
    printf '%s\n' "$*"
}

fail() {
    printf 'error: %s\n' "$*" >&2
    exit 1
}

while [ $# -gt 0 ]; do
    case $1 in
        --version)
            [ $# -ge 2 ] || fail "--version needs a tag, for example v0.1.0"
            tag=$2
            shift 2
            ;;
        --prefix)
            [ $# -ge 2 ] || fail "--prefix needs a folder"
            prefix=$2
            shift 2
            ;;
        --qt)
            [ $# -ge 2 ] || fail "--qt needs the Qt installation folder"
            qt=$2
            shift 2
            ;;
        --dry-run) dry_run=1; shift ;;
        -h|--help) usage; exit 0 ;;
        *) fail "unknown option: $1 (see --help)" ;;
    esac
done

# run <command…>: runs it, or only prints it with --dry-run.
run() {
    if [ "$dry_run" -eq 1 ]; then
        say "  would run: $*"
        return 0
    fi
    "$@"
}

# sha256 <file>: prints the file's SHA-256 digest.
sha256() {
    if command -v sha256sum >/dev/null 2>&1; then
        sha256sum "$1" | cut -d ' ' -f 1
    else
        shasum -a 256 "$1" | cut -d ' ' -f 1
    fi
}

for tool in curl tar cmake mktemp; do
    command -v "$tool" >/dev/null 2>&1 || fail "$tool is needed"
done
command -v sha256sum >/dev/null 2>&1 || command -v shasum >/dev/null 2>&1 \
    || fail "sha256sum or shasum is needed"

work=$(mktemp -d) || fail "cannot create a temporary folder"
trap 'rm -rf "$work"' EXIT
trap 'exit 1' INT TERM

if [ -n "$tag" ]; then
    base="$releases/download/$tag"
else
    base="$releases/latest/download"
fi

say "Looking for ${tag:-the latest release}…"
curl -fsSL -o "$work/SHA256SUMS" "$base/SHA256SUMS" || fail "cannot download $base/SHA256SUMS"
archive=$(sed -n 's/^[0-9a-f]\{64\}  \(ardoise-[0-9][0-9.]*\.tar\.gz\)$/\1/p' "$work/SHA256SUMS" | head -n 1)
[ -n "$archive" ] || fail "the release lists no source archive"
version=${archive#ardoise-}
version=${version%.tar.gz}

installed=""
for config in "$prefix"/lib*/cmake/Ardoise/ArdoiseConfigVersion.cmake; do
    [ -f "$config" ] || continue
    installed=$(sed -n 's/^set(PACKAGE_VERSION "\(.*\)")$/\1/p' "$config" | head -n 1)
done
if [ "$installed" = "$version" ]; then
    say "Ardoise $version is already installed in $prefix."
    exit 0
fi

say "Downloading $archive…"
curl -fsSL -o "$work/$archive" "$base/$archive" || fail "cannot download $base/$archive"
expected=$(sed -n "s/^\([0-9a-f]\{64\}\)  $archive\$/\1/p" "$work/SHA256SUMS")
[ "$(sha256 "$work/$archive")" = "$expected" ] \
    || fail "$archive does not match the release's SHA256SUMS; nothing was installed"
say "Checksum verified."

tar -xzf "$work/$archive" -C "$work" || fail "cannot extract $archive"
source="$work/ardoise-$version"
[ -f "$source/CMakeLists.txt" ] || fail "$archive has no ardoise-$version/CMakeLists.txt"

set -- -S "$source" -B "$work/build" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF \
    -DARDOISE_BUILD_EXAMPLES=OFF -DARDOISE_INSTALL=ON "-DCMAKE_INSTALL_PREFIX=$prefix"
if command -v ninja >/dev/null 2>&1; then
    set -- "$@" -G Ninja
fi
if [ -n "$qt" ]; then
    set -- "$@" "-DCMAKE_PREFIX_PATH=$qt"
fi

say "Building Ardoise $version…"
run cmake "$@" || fail "CMake could not configure Ardoise; check that Qt 6.2+ is installed, or pass --qt"
run cmake --build "$work/build" --parallel || fail "Ardoise did not build"
if [ "$dry_run" -eq 0 ] && ! mkdir -p "$prefix" 2>/dev/null; then
    fail "cannot write to $prefix; choose another folder with --prefix"
fi
run cmake --install "$work/build" || fail "Ardoise was not installed in $prefix"

if [ "$dry_run" -eq 1 ]; then
    say "Dry run: nothing was installed."
    exit 0
fi

say ""
say "Ardoise $version is installed in $prefix. In a CMake project:"
say ""
say "  find_package(Ardoise ${version%.*} CONFIG REQUIRED)"
say "  target_link_libraries(my-app PRIVATE Ardoise::Widgets)"
case ":${PATH}:" in
    *":$prefix/bin:"*) ;;
    *)
        say ""
        say "$prefix/bin is not in PATH, so configure your project with"
        say "-DCMAKE_PREFIX_PATH=$prefix for CMake to find Ardoise."
        ;;
esac
