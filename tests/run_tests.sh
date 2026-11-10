#!/bin/sh

set -u

test_root=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
passed=0
failed=0

for test_script in "$test_root"/*/test_*.sh; do
	if [ ! -f "$test_script" ]; then
		continue
	fi

	printf '\n=== %s ===\n' "${test_script#"$test_root"/}"
	if "$test_script"; then
		passed=$((passed + 1))
	else
		status=$?
		echo "FAIL: ${test_script#"$test_root"/} (exit $status)" >&2
		failed=$((failed + 1))
	fi
done

printf '\nTest summary: %s passed, %s failed.\n' "$passed" "$failed"
[ "$failed" -eq 0 ]
