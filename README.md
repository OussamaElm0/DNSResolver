# DNSResolver

DNSResolver is a simple client-server application designed to resolve IP addresses to their corresponding domain names and vice versa using DNS lookups. The server listens for incoming connections and processes requests from clients, returning the primary domain name, IP address, and any associated aliases.

## Features

- Simple client-server architecture
- Resolves IP addresses to domain names and aliases
- Resolves domain names to IP addresses
- Provides informative responses to clients

## Getting Started

### Prerequisites

- GCC compiler
- A Unix-like operating system (Linux, macOS, etc.)

### Installation

1. Clone the repository:

```sh
git clone https://github.com/OussamaElm0/DNSResolver.git
cd DNSResolver
```
2. Compile the server and client programs:
```sh
./compiler.sh
```
or 
```sh
bash compiler.sh
```
Alternatively, you can compile the programs manually:

```sh
gcc -o server server.c
gcc -o client client.c
```

### Usage
1. Start the server:
```sh
./server
```
The server will start listening on port 1312.

2. In a separate terminal, start the client:
```sh
./client
```

3. Follow the on-screen instructions to resolve IP addresses and domain names.

### Example
![Demo](https://i.imgur.com/YY5fqhZ.gif)

### Contributing
Contributions are welcome! Please open an issue or submit a pull request for any improvements or bug fixes.

### Contact

For any questions or feedback, please contact:

- [Linkedin](https://www.linkedin.com/in/oussama-el-mabrouki-81594b283/)
- [GitHub](https://github.com/OussamaElm0)
