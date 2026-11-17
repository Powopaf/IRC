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
printf 'PASS testpass\r\nNICK JoinTest\r\nUSER join 0 * :Join Test\r\nJOIN invalid\r\nJOIN #join-test\r\nJOIN #join-one,#join-two\r\n' | timeout 3 nc 127.0.0.1 "$port" >"$tmp/out" || true
grep -Fq ' 476 JoinTest invalid :Bad Channel Mask' "$tmp/out"
grep -Fq ' JOIN :#join-test' "$tmp/out"
grep -Fq ' 331 JoinTest #join-one :No topic is set' "$tmp/out"
grep -Fq ' 331 JoinTest #join-two :No topic is set' "$tmp/out"
grep -Fq ' 353 JoinTest = #join-one :@JoinTest' "$tmp/out"
grep -Fq ' 366 JoinTest #join-two :End of /NAMES list.' "$tmp/out"
echo 'PASS: JOIN rejects an invalid channel and returns per-channel topic and NAMES replies'
