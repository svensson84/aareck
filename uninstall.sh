#!/usr/bin/env bash

set -e

PREFIX="$1"

if [[ -z "$PREFIX" ]]; then
  printf '%s\n' \
    "usage: $0 <prefix>" \
    "  e.g. $0 /usr/local" >&2
  exit 1
fi

rm -f "$PREFIX/bin/aareck"

echo "aareck has been uninstalled from $PREFIX/bin/aareck"
