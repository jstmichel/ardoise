#!/bin/sh
# Takes the README screenshots again from the gallery, offscreen and at twice the scale.
# Usage: scripts/screenshots.sh [build-directory]   (default: build)
set -eu

root=$(cd "$(dirname "$0")/.." && pwd)
build=${1:-"$root/build"}
gallery="$build/examples/gallery/ardoise-gallery"
output="$root/docs/screenshots"

if [ ! -x "$gallery" ]; then
    echo "error: $gallery not found; build the project first" >&2
    exit 1
fi

mkdir -p "$output"
QT_QPA_PLATFORM=offscreen QT_SCALE_FACTOR=2 LANG=C.UTF-8 LANGUAGE=en \
    ARDOISE_GALLERY_CAPTURE="$output" "$gallery"
ls "$output"
