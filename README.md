# IRC
An IRC Server in C++

### Installation
``` bash
git clone <repository_url>
cd ft_irc
make
```
## Usage example
#### Run this cmd in the terminal
```
./ircserv 4444 hello
```
#### So at this point, the server is running. It just needs to be connected to. You can use any IRC client or simply use nc
```
nc localhost 4444
```
#### and then pass the following commands
```
pass hello
nick lsaba
user lsaba 0 * lsaba
```

