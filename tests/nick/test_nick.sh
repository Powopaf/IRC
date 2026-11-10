#!/bin/sh

set -eu

repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
tmpdir=$(mktemp -d)
server_pid=
first_client_pid=
second_client_pid=

cleanup() {
	exec 3>&- 4>&- 2>/dev/null || true
	for pid in "$second_client_pid" "$first_client_pid" "$server_pid"; do
		if [ -n "$pid" ]; then
			kill "$pid" 2>/dev/null || true
			wait "$pid" 2>/dev/null || true
		fi
	done
	rm -rf "$tmpdir"
}
trap cleanup EXIT
trap 'exit 1' HUP INT TERM

for command in nc python3; do
	if ! command -v "$command" >/dev/null 2>&1; then
		echo "Required command not found: $command" >&2
		exit 1
	fi
done

make -C "$repo_root"

port=$(python3 -c '
import socket
s = socket.socket()
s.bind(("127.0.0.1", 0))
print(s.getsockname()[1])
s.close()
')
"$repo_root/ircserv" "$port" nick_test_password >"$tmpdir/server.log" 2>&1 &
server_pid=$!

wait_for_log() {
	pattern=$1
	attempt=0
	while [ "$attempt" -lt 50 ]; do
		if grep -Fq "$pattern" "$tmpdir/server.log"; then
			return 0
		fi
		if ! kill -0 "$server_pid" 2>/dev/null; then
			break
		fi
		attempt=$((attempt + 1))
		sleep 0.1
	done
	echo "Timed out waiting for server log: $pattern" >&2
	cat "$tmpdir/server.log" >&2
	return 1
}

wait_for_log "ft_irc server listening on port $port"

mkfifo "$tmpdir/first.in"
nc 127.0.0.1 "$port" <"$tmpdir/first.in" >"$tmpdir/first.out" &
first_client_pid=$!
exec 3>"$tmpdir/first.in"
wait_for_log "NEW user CONNECTED"
printf 'NICK taken\r\n' >&3
wait_for_log "NICK taken"

mkfifo "$tmpdir/second.in"
nc 127.0.0.1 "$port" <"$tmpdir/second.in" >"$tmpdir/second.out" &
second_client_pid=$!
exec 4>"$tmpdir/second.in"
printf 'NICK\r\nNICK bad!\r\nNICK newnick\r\nNICK taken\r\n' >&4

attempt=0
while [ "$attempt" -lt 50 ]; do
	if grep -Fq ' 431 * :No nickname given' "$tmpdir/second.out" \
		&& grep -Fq ' 432 * bad! :Erroneous nickname' "$tmpdir/second.out" \
		&& grep -Fq ' 433 newnick taken :Nickname is already in use' "$tmpdir/second.out"; then
		echo "PASS: NICK missing, invalid, valid, and already-in-use cases."
		exit 0
	fi
	attempt=$((attempt + 1))
	sleep 0.1
done

echo "NICK test failed. Received responses:" >&2
cat "$tmpdir/second.out" >&2
echo "Server log:" >&2
cat "$tmpdir/server.log" >&2
exit 1
