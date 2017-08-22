# [NamedPipe](UnixNamedPipe.hpp)

Abstraction to named pipes.

### Members

##### Constructor
```
UnixNamedPipe(const std::string &fileName);
```

##### open
```
void open(const std::string &fileName, std::ifstream::openmode mode = std::ifstream::in | std::ifstream::out);
```
Opens a new named pipe in the given mode.

##### close
```
void close(bool erase = true)
```
Closes the named pipe and optionally erases it.
