# Customised Virtual File System (CVFS)

## 1. Project Name

**Customised Virtual File System (CVFS)**

## 2. Technology

**C Programming**

## 3. Project Overview

CVFS is a custom implementation of a **Virtual File System (VFS)** that simulates the basic functionality of a Linux-like file system.

The project provides a **custom command-line shell** through which users can perform various file-related operations.

It is designed to provide practical understanding of **file system concepts, system programming, memory management, and operating system internals**.

## 4. Main Features

- Custom command-line shell
- File creation
- File reading
- File writing
- File deletion
- File listing
- File information
- File permissions
- File descriptor management
- Inode management
- Dynamic memory management

## 5. Data Structures Used

### Inode

The Inode stores information about a file, such as:

- File name
- Inode number
- File size
- Actual file size
- File type
- Reference count
- File permissions
- Data buffer

### File Table

The File Table stores information related to an opened file, such as:

- Read offset
- Write offset
- File mode
- Pointer to the corresponding Inode

### UAREA

UAREA stores process-related information and maintains the **User File Descriptor Table (UFDT)**.

### User File Descriptor Table (UFDT)

UFDT maintains references to the File Tables associated with file descriptors.

### Super Block

The Super Block stores information about the virtual file system, such as:

- Total number of Inodes
- Number of Free Inodes

## 6. Available Commands

| Command | Description |
|---|---|
| `help` | Displays available commands |
| `man <command>` | Displays information about a command |
| `clear` | Clears the terminal |
| `creat <name> <permission>` | Creates a new file |
| `ls` | Lists existing files |
| `ls -a` | Displays details of all files |
| `stat <name>` | Displays information about a specific file |
| `write <fd>` | Writes data into a file |
| `read <fd> <size>` | Reads data from a file |
| `unlink <name>` | Deletes a file |
| `exit` | Terminates CVFS |

## 7. File Permissions

CVFS supports the following permission values:

| Value | Permission |
|---|---|
| `1` | Read |
| `2` | Write |
| `3` | Read + Write |

### Example

```text
creat Demo.txt 3
```

This creates `Demo.txt` with **Read + Write** permission.

## 8. How to Run

### Compile

Using GCC:

```bash
gcc CVFS.c -o CVFS
```

### Run

#### Linux/macOS

```bash
./CVFS
```

#### Windows

```bash
CVFS.exe
```

## 9. Example

```text
Marvellous CVFS : > creat Demo.txt 3

File successfully created with fd : 3

Marvellous CVFS : > ls

Demo.txt

Marvellous CVFS : > write 3

Enter the data that you want to write the file
Hello CVFS

Marvellous CVFS : > read 3 10

Read operation is successful
Data from file is :
Hello CVFS

Marvellous CVFS : > stat Demo.txt

Marvellous CVFS : > unlink Demo.txt

Marvellous CVFS : > exit

Thank you for using Marvellous CVFS
```

## 10. Project Architecture

```text
                     +----------------+
                     |  Custom Shell  |
                     +-------+--------+
                             |
                             v
                     +----------------+
                     | Command Parser |
                     +-------+--------+
                             |
                             v
                     +----------------+
                     | File Operations|
                     +-------+--------+
                             |
          +----------+-------+-------+----------+
          |          |       |       |          |
          v          v       v       v          v
       Create      Read    Write   Delete      List
                             |
                             v
                           Stat
                             |
                             v
                         +-------+
                         |  UFDT |
                         +---+---+
                             |
                             v
                       +-----------+
                       | File Table|
                       +-----+-----+
                             |
                             v
                         +-------+
                         | Inode |
                         +---+---+
                             |
                             v
                       +-------------+
                       | Data Buffer |
                       +-------------+
```

## 11. Learning Outcomes

This project helped in understanding:

- Linux file system concepts
- Operating system internals
- Inodes and file tables
- UAREA and UFDT
- File descriptors
- File permissions
- Dynamic memory allocation
- Linked lists
- Command-line shell design
- System programming in C
- Low-level programming concepts

## 12. Future Improvements

- Add more file system commands
- Implement additional file operations
- Add persistent file storage
- Improve command handling
- Improve memory management
- Add more error handling
- Increase file system capacity

## 13. Author

**Pranav Avinash Narkhede**