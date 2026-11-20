#!/bin/bash

if [ "$#" -ne 2 ]; then
	echo "Usage: ./bot.sh <port> <password>" >&2
	exit 1
fi

port=$1
password=$2

if [[ ! "$port" =~ ^[0-9]+$ ]] || [ "$port" -lt 0 ] || [ "$port" -gt 65535 ]; then
	echo "Invalid port number: $port" >&2
	exit 1
fi

if [ -z "$password" ]; then
	echo "Password cannot be empty." >&2
	exit 1
fi

if ! command -v nc >/dev/null 2>&1; then
	echo "Error: nc is required." >&2
	exit 1
fi

channel="#time"
tmp_dir=$(mktemp -d)
outgoing=$tmp_dir/to-server
incoming=$tmp_dir/from-server
disconnected=$tmp_dir/disconnected
mkfifo "$outgoing" "$incoming"

cleanup() {
	if [ -n "${reader_pid:-}" ]; then
		kill "$reader_pid" 2>/dev/null || true
	fi
	if [ -n "${nc_pid:-}" ]; then
		kill "$nc_pid" 2>/dev/null || true
		wait "$nc_pid" 2>/dev/null || true
	fi
	rm -rf "$tmp_dir"
	exit 0
}
trap cleanup INT TERM

connect_bot() {
	rm -f "$disconnected"

	nc 127.0.0.1 "$port" <"$outgoing" >"$incoming" &
	nc_pid=$!
	exec 3>"$outgoing"
	exec 4<"$incoming"

	printf 'PASS %s\r\nNICK IRCBot\r\nUSER ircbot 0 * :IRC Bot\r\n' "$password" >&3
	sleep 1
	printf 'JOIN %s\r\n' "$channel" >&3
	sleep 1
	now=$(date '+%H:%M:%S')
	printf 'TOPIC %s :%s\r\nMODE %s +t\r\n' "$channel" "$now" "$channel" >&3

	(
		while IFS= read -r line <&4; do
			line=${line%$'\r'}
			if [[ "$line" == PING* ]]; then
				token=${line#PING }
				printf 'PONG %s\r\n' "$token" >&3
			fi
		done
		touch "$disconnected"
	) &
	reader_pid=$!

	while [ ! -e "$disconnected" ]; do
		sleep 30
		if [ -e "$disconnected" ]; then
			break
		fi

		now=$(date '+%H:%M:%S')
		printf 'PRIVMSG %s :%s\r\n' "$channel" "$now" >&3
	done

	exec 3>&-
	exec 4<&-
	kill "$reader_pid" 2>/dev/null || true
	wait "$reader_pid" 2>/dev/null || true
	kill "$nc_pid" 2>/dev/null || true
	wait "$nc_pid" 2>/dev/null || true
	reader_pid=
	nc_pid=
}

while true; do
	connect_bot
	echo "Disconnected; retrying..." >&2
	sleep 1
done
