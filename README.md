# Win32 Directory Plus
### A multipurpose directory operations CLI tool for windows.

___

## Features 
This CLI tool written in C that uses ihe Win32 API to perform enumeration and movements of files in directories. 
It provides the following features:

- **Directory Organisation**

    Sorts all files in a directory into sub directories of naming format "extension\_Files".
    The files, as expected are sorted on the basis of their extensions. 
    *Set Limitations:*
    (Filenames starting with '.' are ignored such as ".gitignore"
    Existing subdirectories are not traaversed for files.)


- **Token Searching**

    Recursively traverses through all sub directories and files in the provided directory
    to find and display the ones with a specific symbolic or substring token present in their filename. 
    *Set Limitations:*
    (Ignores directories and files with names starting with '.' such as ".git" or ",gitignore")



- **Directory Syncing**

    Provides one way directory syncing for a provided source and destination directory. 
    The syncing is as follows:
        -If file present in source and not destination \-> Copy to destination
        -If sub directory present in source and not destination \-> Copy to destination
        -If file in source has a more recent edi date than destination's version \-> Replace destination's version with a copy of source's
    *Set Limitations*
    (If a file or directory is present in destination and not in source, it will not be traversed or checked.
     Ignores directories and files with names starting with '.' such as ".git" or ",gitignore"
     As checking and comparison happens based on filenames and dir names, changing the name in destination will lead to them being untracked.)


- **Directory Tree**

    Recursively traverses through the files and sub directories in the provided directory and displays them in a tree like format.
    *Set Limitations*
    (Displays sub directories with names starting with '.' but does not traverse in them, such directory's example: ".git")


- **Trash Collection**
    
    Recursively traverses through the files and sub directories in the provided directory to display and move the files older than a specified 
    number of months in an existing or newly created "trashbin" directory in the provided directory.
    *Set Limitations:*
    (Ignores directories and files with names starting with '.' such as ".git" or ",gitignore")


---

## Installation

# Using without modification

Ensure you have the following installed on your system:

- [Git](https://git-scm.com/downloads) (for cloning the repository)
- Windows Operating System (The exe was tested on Windows 10)

### Cloning the Repository

1. Open your terminal or command prompt.

2. Navigate to the directory where you want to clone the project.

3. Run the following command to clone the repository:

    ```bash
    git clone https://github.com/Neel2k5/Win32DirectoryPlus.git
    ```

4. Navigate into the project's executable directory:

    ```bash
    cd repository\executable
    ```
5. This directory contains the executable file 'wdp.exe' along with the user guide 'ug\_wdp.txt'.
   These two files must be kept in the same directory for effective usage.
   You may set your enviornment variable path to this directory or copy these two files in some other directory and use the respective path.

6. Open Command Prompt and run the following commands to get started:
    ```bash
    wdp help
    ```
    
    ```bash
    wdp
    ```

## License

This project is licensed under the [Apache License 2.0](LICENSE.md).
