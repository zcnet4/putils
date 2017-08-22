# [Directory](ADirectory.hpp)

An abstraction to Unix and Windows directories, which lets you iterate through a directory and get information about each file.

### Members

##### File
```
struct File
{
    std::string name;
    std::string fullPath;
    bool isDirectory;
};
```
Abstraction to a file, specifying name, full path and whether this is a directory.

##### getNextFile
```
std::unique_ptr<File> getNextFile();
```
Returns a pointer to a `File`, or `nullptr` if the end of the directory has been reached.

###### getFiles
```
std::vector<File> getFiles();
```
Returns a vector of all the files in the directory.

##### for_each
```
void for_each(const std::function<void(const File &)> &func);
```
Applies a function to all files in the directory.
