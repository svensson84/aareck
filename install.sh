#!/usr/bin/env bash

set -e

AARECK_ROOT="${0%/*}"
PREFIX="$1"

if [[ -z "$PREFIX" ]]; then
  printf '%s\n' \
    "usage: $0 <prefix>" \
    "  e.g. $0 /usr/local" >&2
  exit 1
fi

install -m 755 "$AARECK_ROOT/bin/aareck" "$PREFIX/bin"

echo "aareck has been installed to $PREFIX/bin/aareck"
