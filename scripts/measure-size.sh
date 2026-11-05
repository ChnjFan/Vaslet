#!/bin/sh
# Report installed artifact bytes and direct runtime dependencies.
# This does not enforce a budget or measure memory usage.
set -eu

stage=${1:?Usage: measure-size.sh INSTALL_PREFIX [LIBDIR]}
libdir=${2:-lib}
if [ -f "$stage/$libdir/libvaslet.so" ]; then
  library="$stage/$libdir/libvaslet.so"
elif [ -f "$stage/$libdir/libvaslet.dylib" ]; then
  library="$stage/$libdir/libvaslet.dylib"
else
  echo "Missing shared library in $stage/$libdir" >&2
  exit 1
fi

total=0
for artifact in "$stage/bin/vasletd" "$library" "$stage/bin/vasletctl"; do
  if [ ! -f "$artifact" ]; then
    echo "Missing artifact: $artifact" >&2
    exit 1
  fi
  bytes=$(wc -c < "$artifact" | tr -d '[:space:]')
  total=$((total + bytes))
  printf '%s bytes  %s\n' "$bytes" "$artifact"
done
printf 'Total: %s bytes (three binaries; dependency sizes excluded)\n' "$total"

for artifact in "$stage/bin/vasletd" "$library" "$stage/bin/vasletctl"; do
  printf '\nDependencies: %s\n' "$artifact"
  if command -v readelf >/dev/null 2>&1; then
    readelf -d "$artifact" | sed -n '/NEEDED/p'
  elif command -v otool >/dev/null 2>&1; then
    otool -L "$artifact"
  else
    echo 'No readelf or otool available; use the target SDK inspection tool.'
  fi
done
