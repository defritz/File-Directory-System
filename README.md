# File-Directory-System
Data Structures Project from Sophomore Year

Allows for the creation and modification of files and directories, as well as displaying the paths to created files.

Commands:
    ls / ls <path> - list contents of cwd or specified directory
    pwd - displays path from root directory to current working directory (cwd)
    mkdir <path> - creates a directory at the specified path
    touch <path> - creates a file in the directory at the end of the provided path
    cd <path> - changes the cwd to the directory specified by the provided path
    rm <path> - removes the specifed path or directory at the end of the provided path (error if directory is not empty)
    rm -f <path> - forces the removal of specified directory, deleting all of its contents
    find <local name> - lists the paths to all directories/files containing the matching local name
 Notes:
    A path is represented by a sequence of names seperated by '/' symbols
    Paths that begin with '/' are absolute paths, with the absolue path '/' serving as the root directory
    Paths without a leading '/' are relative paths, taken in context to the current working directory (cwd)
    For any directory, all of its immediate children must be unique
    The name '.' represents the cwd
    The name '..' represents the parent directory of the cwd
