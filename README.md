*This project has been created as part of the 42 curriculum by pifourni, mirleca*

# IRC

## Description

IRC is an Internet Relay Chat server written in C++98. The server accepts TCP
connections, authenticates users, and implements the channel and messaging
features needed to communicate with a standard IRC client.

The project uses `poll()` to handle multiple simultaneous client connections
and supports user registration, public and private messages, channel creation,
channel membership, invitations, topics, kicks, operator privileges, and
channel modes.

Supported channel modes include:

- `i`: invite-only channel
- `t`: only operators may change the topic
- `k`: channel password
- `o`: grant or remove operator status
- `l`: set or remove a user limit

Supported commands include `PASS`, `NICK`, `USER`, `CAP`, `JOIN`, `MSG`,
`PRIVMSG`, `PING`, `KICK`, `INVITE`, `TOPIC`, and `MODE`.

## Instructions

### Requirements

- A C++ compiler with C++98 support
- `make`
- A Unix-like operating system
- An IRC client such as Irssi or HexChat, or a TCP tool such as `nc`

### Compilation

From the root of the repository, compile with:

```sh
make
```

This creates the `ircserv` executable.

Useful Makefile targets:

```sh
make clean    # Remove object files
make fclean   # Remove object files and the executable
make re       # Rebuild from scratch
```

### Execution

Start the server with a port and a password:

```sh
./ircserv <port> <password>
```

For example:

```sh
./ircserv 6667 testpass
```

The port must be between 0 and 65535, and the password cannot be empty.
Connect from an IRC client to `127.0.0.1` on the selected port using the same
password.

With Irssi, a typical connection can be made with:

```text
/connect 127.0.0.1 6667 testpass
```

Individual tests are available under `tests/`.

## Resources

- [RFC 1459: Internet Relay Chat Protocol](https://www.rfc-editor.org/rfc/rfc1459)
- [RFC 2812: Internet Relay Chat: Client Protocol](https://www.rfc-editor.org/rfc/rfc2812)
- [Modern IRC Documents](https://modern.ircdocs.horse/)
- [C++ Reference](https://en.cppreference.com/w/cpp)
- [Linux poll(2) manual](https://man7.org/linux/man-pages/man2/poll.2.html)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)

AI was used to help draft this README.
