# Socket Programming Project

This project implements various socket communication methods in C, including TCP, UDP, and Unix Domain Sockets (UDS). The main program can dynamically configure input and output communication channels based on command-line arguments and execute specified programs or facilitate a chat.

## Features

- **TCP Server and Client**: Handles TCP connections for both server and client modes.
- **UDP Server and Client**: Manages UDP communications for server and client setups.
- **Unix Domain Socket (UDS)**: Supports both datagram and stream sockets for inter-process communication on the same host.
- **Command Execution**: Executes programs with specified arguments and handles their input/output through the configured sockets.
- **Timeout Handling**: Optional timeout for operations, with a graceful exit upon timeout.

## Files Included

- `main.c`: Contains the main logic and functions for handling different socket types and executing commands.
- `Makefile`: A simple makefile to compile the program.

## Usage

Compile the program using the provided Makefile:
```sh
make
```

Run the program with the desired socket type, server/client mode, and other options:
```sh
./socket_program -e "<command>" -o "<output>" -i "<input>" -b "<both>" -t <timeout>
```

### Options

- `-e <string>`: Command to execute.
- `-o <string>`: Output configuration. Supported formats:
  - `TCPS<port>`: TCP server on the specified port.
  - `TCPC<ip>,<port>`: TCP client connecting to the specified IP and port.
  - `UDPS<port>`: UDP server on the specified port.
  - `UDPC<ip>,<port>`: UDP client connecting to the specified IP and port.
  - `UDSCD<path>`: Unix domain socket client in datagram mode with the specified path.
  - `UDSCS<path>`: Unix domain socket client in stream mode with the specified path.
- `-i <string>`: Input configuration (same formats as `-o`).
- `-b <string>`: Both input and output configuration (same formats as `-o`).
- `-t <timeout>`: Timeout in seconds for the operations.


### Example

Run a TCP server that executes a command:
```sh
./socket_program -e "ls -l" -o TCPS8080
```
Run a TCP client that connects to a server and executes a command:
```sh
./socket_program -e "cat" -o TCPC127.0.0.1,8080
```
playing ttt using TCP:
```sh
./mync -e "./ttt 123456789" -i TCPS4269
./mync -e "./ttt 123456789" -o TCPS4269
./mync -e "./ttt 123456789" -b TCPS4269
```
using UDP:
```sh
./mync -e "./ttt 123456789" -i UDPS4269  
./mync -e "./ttt 123456789" -o UDPC4269
./mync -e "./ttt 123456789" -b UDPS4269
```
Client:
```sh
nc -u localhost 4269
```

using UDS: opening a chat portal
```sh
./mync -i UDSSSmyserver.sock 
./mync -e "./ttt 123456789" -i UDSSSmyserver.sock
```
Client:
```sh
nc -U myserver.sock
```

### Function Descriptions

#### `void run_prog(char* E)`
- Splits the command string into tokens and executes the program using `execvp`.

#### `int TCP_CLIENT_PORT(char *ip, int port)`
- Creates a TCP client socket and connects to the specified IP and port.

#### `int TCP_SERVER_PORT(int port)`
- Creates a TCP server socket and listens for incoming connections on the specified port.

#### `int UDP_SERVER_PORT(int port)`
- Creates a UDP server socket and binds it to the specified port.

#### `int UDP_CLIENT_PORT(char *ip, int port)`
- Creates a UDP client socket and connects to the specified IP and port.

#### `int UDS_SERVER_DATAGRAM(char *path)`
- Creates a Unix Domain Socket in datagram mode and binds it to the specified path.

#### `int UDS_CLIENT_DATAGRAM(char *path)`
- Creates a Unix Domain Socket client in datagram mode and connects to the specified path.

#### `int UDS_SERVER_STREAM(char *path)`
- Creates a Unix Domain Socket in stream mode and binds it to the specified path.

#### `int UDS_CLIENT_STREAM(char *path)`
- Creates a Unix Domain Socket client in stream mode and connects to the specified path.

#### `void alarm_handler(int sig)`
- Handles the timeout signal by printing a message and exiting the program.

