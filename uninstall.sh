#!/usr/bin/env bash

set -e

AARECK_ROOT="${0%/*}"

read -p "installation path [/usr/local]: " path
path=${path:-/usr/local}

rm -f "$path/bin/aareck"

echo -e "\naareck has been uninstalled from $path/bin/aareck"
