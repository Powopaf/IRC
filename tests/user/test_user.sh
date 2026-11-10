#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
tmp=$(mktemp -d)
pid=
cleanup() { [ -z "$pid" ] || { kill "$pid" 2>/dev/null || true; wait "$pid" 2>/dev/null || true; }; rm -rf "$tmp"; }
trap cleanup EXIT
command -v nc >/dev/null && command -v python3 >/dev/null
make -C "$root"
port=$(python3 -c 'import socket;s=socket.socket();s.bind(("127.0.0.1",0));print(s.getsockname()[1]);s.close()')
"$root/ircserv" "$port" testpass >"$tmp/server" 2>&1 & pid=$!
for i in $(seq 1 50); do grep -q 'listening on port' "$tmp/server" && break; sleep .1; done
printf 'USER incomplete\r\nPASS testpass\r\nNICK UserTest\r\nUSER usertest 0 * :User Test\r\n' | timeout 3 nc 127.0.0.1 "$port" >"$tmp/out" || true
grep -Fq ' 461 * USER :Not enough parameters' "$tmp/out"
grep -Fq ' 001 UserTest :Welcome' "$tmp/out"
echo 'PASS: USER validates parameters and registers a user'
