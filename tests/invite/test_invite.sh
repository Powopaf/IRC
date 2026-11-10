#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
tmp=$(mktemp -d)
server_pid= alice_pid= bob_pid=
cleanup() {
	exec 3>&- 4>&- 2>/dev/null || true
	for pid in "$bob_pid" "$alice_pid" "$server_pid"; do
		[ -z "$pid" ] || { kill "$pid" 2>/dev/null || true; wait "$pid" 2>/dev/null || true; }
	done
	rm -rf "$tmp"
}
trap cleanup EXIT
command -v nc >/dev/null && command -v python3 >/dev/null
make -C "$root"
port=$(python3 -c 'import socket;s=socket.socket();s.bind(("127.0.0.1",0));print(s.getsockname()[1]);s.close()')
"$root/ircserv" "$port" testpass >"$tmp/server" 2>&1 & server_pid=$!
wait_for() {
	file=$1; text=$2; i=0
	while [ "$i" -lt 50 ]; do
		grep -Fq "$text" "$file" 2>/dev/null && return 0
		i=$((i + 1)); sleep .1
	done
	echo "Timed out waiting for $text" >&2; cat "$file" "$tmp/server" >&2; return 1
}
wait_for "$tmp/server" 'listening on port'
mkfifo "$tmp/alice.in"
nc 127.0.0.1 "$port" <"$tmp/alice.in" >"$tmp/alice.out" & alice_pid=$!
exec 3>"$tmp/alice.in"
printf 'PASS testpass\r\nNICK Alice\r\nUSER alice 0 * :Alice\r\nJOIN #invite-test\r\n' >&3
wait_for "$tmp/alice.out" ' JOIN :#invite-test'
mkfifo "$tmp/bob.in"
nc 127.0.0.1 "$port" <"$tmp/bob.in" >"$tmp/bob.out" & bob_pid=$!
exec 4>"$tmp/bob.in"
printf 'PASS testpass\r\nNICK Bob\r\nUSER bob 0 * :Bob\r\n' >&4
wait_for "$tmp/bob.out" ' 001 Bob :Welcome'
printf 'INVITE Bob #invite-test\r\n' >&3
wait_for "$tmp/alice.out" ' 341 Alice Bob #invite-test'
wait_for "$tmp/bob.out" ' INVITE Bob #invite-test'
echo 'PASS: INVITE confirms and notifies the invitee'
