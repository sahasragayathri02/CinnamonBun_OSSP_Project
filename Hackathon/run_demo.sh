#!/bin/sh
set -eu
cd "$(dirname "$0")"
make
echo "Demo 1: four-stage fruit pipeline (expected output: 3)"
./pipeline --log "printf 'pear\napple\nbanana\napple\n'" "sort" "uniq" "wc -l"
echo
echo "Demo 2: three-stage grep pipeline (expected output: 2)"
./pipeline --log "printf 'hello\nworld\nhello\n'" "grep hello" "wc -l"
echo
echo "Demo 3: five-stage pipeline (expected output: 3)"
./pipeline "printf 'pear\napple\nbanana\napple\n'" "cat" "sort" "uniq" "wc -l"
