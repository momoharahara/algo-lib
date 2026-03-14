#!/bin/sh

set -eu

cd `dirname $0`
cd ..

if [ $# -ne 1 ]; then
  echo "usage: tools/copy_submit.sh <problem-letter>" >&2
  exit 1
fi

letter=`printf '%s' "$1" | tr '[:upper:]' '[:lower:]'`
src_file="src/$letter.cpp"
out_file="bin/$letter.submit.cpp"

if [ ! -f "$src_file" ]; then
  echo "source file not found: $src_file" >&2
  exit 1
fi

if ! command -v pbcopy >/dev/null 2>&1; then
  echo "pbcopy not found" >&2
  exit 1
fi

python3 tools/expand.py "$src_file" -o "$out_file"
pbcopy < "$out_file"

echo "expanded $src_file -> $out_file and copied to clipboard"
