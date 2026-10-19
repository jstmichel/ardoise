#!/bin/sh
# Runs scripts/get-ardoise.sh against a fake release served from a temporary folder, with a
# stub cmake that only records its arguments: nothing is built or installed for real.
# Usage: GetArdoise.sh <path to get-ardoise.sh>
set -u

script=$1
root=$(mktemp -d) || exit 1
trap 'rm -rf "$root"' EXIT
failures=0

check() {
    if [ "$1" -eq 0 ]; then
        printf 'PASS: %s\n' "$2"
    else
        printf 'FAIL: %s\n' "$2"
        failures=$((failures + 1))
    fi
}

sha256() {
    if command -v sha256sum >/dev/null 2>&1; then
        sha256sum "$1" | cut -d ' ' -f 1
    else
        shasum -a 256 "$1" | cut -d ' ' -f 1
    fi
}

# Fake release v0.1.0 with a source archive, also served as the latest release.
mkdir -p "$root/source/ardoise-0.1.0" "$root/releases/download/v0.1.0" "$root/bin"
echo 'project(Ardoise VERSION 0.1.0)' >"$root/source/ardoise-0.1.0/CMakeLists.txt"
release="$root/releases/download/v0.1.0"
tar -czf "$release/ardoise-0.1.0.tar.gz" -C "$root/source" ardoise-0.1.0
printf '%s  ardoise-0.1.0.tar.gz\n' "$(sha256 "$release/ardoise-0.1.0.tar.gz")" >"$release/SHA256SUMS"
mkdir -p "$root/releases/latest"
cp -R "$release" "$root/releases/latest/download"

cat >"$root/bin/cmake" <<'EOF'
#!/bin/sh
printf '%s\n' "$*" >>"$CMAKE_LOG"
EOF
chmod +x "$root/bin/cmake"

# get [arguments…]: runs the script with the stub, the fake release and a fresh log.
get() {
    rm -f "$root/cmake.log"
    PATH="$root/bin:$PATH" CMAKE_LOG="$root/cmake.log" ARDOISE_RELEASES_URL="file://$root/releases" \
        sh "$script" --prefix "$root/prefix" "$@" >"$root/output" 2>&1
}

get
check $? "the latest release installs"
grep -q -- "-S .*/ardoise-0.1.0 " "$root/cmake.log" && grep -q -- "-DCMAKE_INSTALL_PREFIX=$root/prefix" "$root/cmake.log"
check $? "CMake configures the extracted source for the prefix"
grep -q -- "-DBUILD_TESTING=OFF" "$root/cmake.log" && grep -q -- "-DARDOISE_BUILD_EXAMPLES=OFF" "$root/cmake.log"
check $? "tests and examples are not built"
grep -q -- "^--install " "$root/cmake.log"
check $? "CMake installs the build"

get --version v0.1.0 --qt /opt/qt
check $? "a given release installs"
grep -q -- "-DCMAKE_PREFIX_PATH=/opt/qt" "$root/cmake.log"
check $? "--qt reaches CMake"

get --dry-run
check $? "a dry run succeeds"
[ ! -e "$root/cmake.log" ] && grep -q "would run: cmake --install" "$root/output"
check $? "a dry run only prints the commands"

get --version v9.9.9
[ $? -eq 1 ] && [ ! -e "$root/cmake.log" ]
check $? "a missing release fails before building"

echo 'tampered' >>"$release/ardoise-0.1.0.tar.gz"
get --version v0.1.0
[ $? -eq 1 ] && [ ! -e "$root/cmake.log" ] && grep -q "does not match" "$root/output"
check $? "a wrong checksum fails before building"

mkdir -p "$root/prefix/lib/cmake/Ardoise"
echo 'set(PACKAGE_VERSION "0.1.0")' >"$root/prefix/lib/cmake/Ardoise/ArdoiseConfigVersion.cmake"
get
[ $? -eq 0 ] && [ ! -e "$root/cmake.log" ] && grep -q "already installed" "$root/output"
check $? "an installed version is not built again"

[ "$failures" -eq 0 ]
