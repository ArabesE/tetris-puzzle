#!/usr/bin/env bash
set -u
BIN="${1:-./bin/tetromino_solver}"

if [ ! -x "$BIN" ]; then
  echo "Binary not found: $BIN" >&2
  exit 1
fi

fail=0

echo "== Running tests =="
for f in tests/cases/*.in; do
  [ -e "$f" ] || continue
  printf "
-- %s --
" "$f"
  echo "Input:"
  sed 's/^/  /' "$f"
  echo "Output:"
  "$BIN" < "$f"
  status=$?
  echo "Exit code: $status"

  expected=0
  case "$(basename "$f")" in
    unsolvable_*) expected=3 ;;
    malformed_*)  expected=1 ;;
    stress_*)     expected=0 ;;
    solvable_*)   expected=0 ;;
  esac

  if [ "$status" -eq "$expected" ]; then
    echo "RESULT: PASS"
  else
    echo "RESULT: FAIL (expected $expected)"
    fail=$((fail+1))
  fi

done

echo
if [ "$fail" -eq 0 ]; then
  echo "All tests PASS"
  exit 0
else
  echo "$fail test(s) FAILED"
  exit 1
fi