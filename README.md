# Customised Virtual File System (CVFS)

CVFS is a simple **in-memory Virtual File System** developed using **C Programming**.

It simulates basic file-system operations using concepts such as **Inode, File Table, UAREA, UFDT, File Descriptors, File Permissions, Pointers, Linked Lists, and Dynamic Memory Allocation**.

## Features

- Create, open and close files
- List files and display file information
- Read and write data
- Rename and truncate files
- File seeking using `lseek`
- File permission management
- Reference count management
- Read and write offset management
- Input validation and error handling
- Dynamic memory management
- Custom command-line shell

## Technologies Used

- C Programming
- Data Structures
- Pointers and Structures
- Linked List
- Dynamic Memory Allocation
- Bitwise Operations
- Operating System Concepts
- System Programming

## Commands

| Command | Description |
|---|---|
| `help` | Display available commands |
| `man` | Display command documentation |
| `clear` | Clear the screen |
| `creat <name> <permission>` | Create a new file |
| `open <name> <mode>` | Open an existing file |
| `close <fd>` | Close an opened file |
| `ls` | List existing files |
| `ls -a` | Display file details |
| `stat <name>` | Display file information |
| `write <fd>` | Write data into a file |
| `read <fd> <size>` | Read data from a file |
| `rename <old> <new>` | Rename a file |
| `truncate <name> <size>` | Change file size |
| `lseek <fd> <offset> <whence> <type>` | Change file offset |
| `unlink <name>` | Delete a file |
| `exit` | Exit CVFS |

## File Permissions

| Value | Permission |
|---:|---|
| `1` | Read |
| `2` | Write |
| `3` | Read + Write |
| `4` | Execute |
| `5` | Read + Execute |
| `6` | Write + Execute |
| `7` | Read + Write + Execute |

Permissions are implemented using **bitwise operations**.

## Project Architecture

```text
              +----------------+
              |  Custom Shell  |
              +-------+--------+
                      |
                      v
              +---------------+
              | Command Parser|
              +-------+-------+
                      |
                      v
              +---------------+
              |File Operations|
              +-------+-------+
                      |
                      v
                 +---------+
                 |  UAREA  |
                 +----+----+
                      |
                      v
                 +---------+
                 |  UFDT   |
                 +----+----+
                      |
                 File Descriptor
                      |
                      v
               +-----------+
               |File Table |
               +-----+-----+
                     |
                     v
                 +-------+
                 | Inode |
                 +---+---+
                     |
                     v
               +-----------+
               |Data Buffer|
               +-----------+
```

## How to Run

### Compile

```bash
gcc CVFS.c -o CVFS
```

### Run on Linux/macOS

```bash
./CVFS
```

### Run on Windows

```bash
CVFS.exe
```

## Example

```text
CVFS : > creat Demo.txt 3
File successfully created with fd : 3

CVFS : > write 3
Enter the data that you want to write to the file:
Hello CVFS

CVFS : > read 3 10
Read operation is successful
Data from file is:
Hello CVFS

CVFS : > stat Demo.txt
CVFS : > close 3
CVFS : > exit
Thank you for using CVFS
```

## Core Components

### Inode
Stores file name, inode number, file size, permissions, reference count and data buffer.

### File Table
Stores read offset, write offset, file mode and pointer to the corresponding Inode.

### UAREA and UFDT
Maintain process information and references to File Table entries for opened files.

### File Descriptor
Identifies an opened file and is used by operations such as `read`, `write`, `close` and `lseek`.

## Concepts Learned

- File System
- Inode and File Table
- UAREA and UFDT
- File Descriptors
- File Permissions
- Reference Counting
- File Offsets
- Linked Lists
- Pointers and Structures
- Dynamic Memory Allocation
- Bitwise Operations
- System Programming
- Input Validation and Error Handling

## Project Structure

```text
CVFS/
│
├── CVFS.c
├── README.md
├── DOCUMENTATION.md
├── TESTING.md
└── .gitignore
```

## Limitations

- CVFS is an in-memory file system.
- Files are not stored permanently.
- Data is lost when the program terminates.
- File system capacity is limited.
- Directory hierarchy is not currently implemented.

## Future Improvements

- Directory support
- Persistent file storage
- File import/export
- Copy and move operations
- Append functionality
- Automated testing

## Author

**Pranav Avinash Narkhede**
