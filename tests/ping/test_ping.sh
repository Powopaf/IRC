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
printf 'PASS testpass\r\nNICK PingTest\r\nUSER ping 0 * :Ping Test\r\nPING :token\r\n' | timeout 3 nc 127.0.0.1 "$port" >"$tmp/out" || true
grep -Fq ' 001 PingTest :Welcome' "$tmp/out"
grep -Fq 'PONG :ft_irc' "$tmp/out"
echo 'PASS: PING receives a PONG response'
