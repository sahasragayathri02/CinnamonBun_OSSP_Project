#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
cd "$ROOT"
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT HUP INT TERM
PASS=0; FAIL=0
check_output() {
  name=$1; expected=$2; shift 2
  if "$@" >"$TMP/out" 2>"$TMP/err"; then rc=0; else rc=$?; fi
  actual=$(cat "$TMP/out")
  if [ "$rc" -eq 0 ] && [ "$actual" = "$expected" ]; then
    printf 'PASS: %s\n' "$name"; PASS=$((PASS+1))
  else
    printf 'FAIL: %s (exit=%s, expected <%s>, got <%s>)\n' "$name" "$rc" "$expected" "$actual"
    cat "$TMP/err" >&2; FAIL=$((FAIL+1))
  fi
}
check_failure() {
  name=$1; shift
  if "$@" >"$TMP/out" 2>"$TMP/err"; then
    printf 'FAIL: %s (expected non-zero exit)\n' "$name"; FAIL=$((FAIL+1))
  else
    rc=$?; printf 'PASS: %s (non-zero exit %s as expected)\n' "$name" "$rc"; PASS=$((PASS+1))
  fi
}
check_output "2-stage pipeline" "2" ./pipeline "printf 'a\nb\n'" "wc -l"
check_output "3-stage pipeline" "2" ./pipeline "printf 'b\na\nb\n'" "sort" "uniq" "wc -l"
check_output "4-stage pipeline" "3" ./pipeline "printf 'pear\napple\nbanana\napple\n'" "sort" "uniq" "wc -l"
check_output "5-stage pipeline" "3" ./pipeline "printf 'pear\napple\nbanana\napple\n'" "cat" "sort" "uniq" "wc -l"
check_output "empty input" "0" ./pipeline "printf ''" "cat" "wc -l"
check_output "large input (20,000 lines)" "20000" ./pipeline "awk 'BEGIN { for (i=1; i<=20000; i++) print i }'" "wc -l"
check_failure "invalid command" ./pipeline "__pipeline_command_that_does_not_exist__"
check_failure "command failure in pipeline" ./pipeline "printf 'x\n'" "grep z" "wc -l"
check_output "logged concurrent pipeline" "2" ./pipeline --log "printf 'hello\nhello\n'" "grep hello" "wc -l"
printf '\nTest summary: %s passed, %s failed\n' "$PASS" "$FAIL"
[ "$FAIL" -eq 0 ]
