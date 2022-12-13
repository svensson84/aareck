#!/usr/bin/env bash

set -e

AARECK_ROOT="${0%/*}"

read -p "installation path [/usr/local]: " path
path=${path:-/usr/local}

install -m 755 "$AARECK_ROOT/bin/aareck" "$path/bin"
echo -e "\naareck has been installed to $path/bin/aareck"
