# Customised Virtual File System (CVFS)

CVFS is a simple **Virtual File System** developed using **C Programming**.

It simulates basic file system operations using concepts such as **Inode, File Table, UAREA, UFDT, File Descriptors, File Permissions, Pointers, and Dynamic Memory Allocation**.

## Features

- Create a file
- List files
- Display file information
- Write data into a file
- Read data from a file
- Delete a file
- File permission management
- Custom command-line shell
- Help and manual commands

## Technologies Used

- C Programming
- Data Structures
- Pointers
- Linked List
- Dynamic Memory Allocation
- Operating System Concepts

## Commands

| Command | Description |
|---|---|
| `help` | Display available commands |
| `man <command>` | Display command information |
| `clear` | Clear the screen |
| `creat <name> <permission>` | Create a new file |
| `ls` | List files |
| `ls -a` | Display file details |
| `stat <name>` | Display file information |
| `write <fd>` | Write data into a file |
| `read <fd> <size>` | Read data from a file |
| `unlink <name>` | Delete a file |
| `exit` | Exit the CVFS |

## How to Run

### Compile

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

## Example

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

Marvellous CVFS : > unlink Demo.txt

Marvellous CVFS : > exit
Thank you for using Marvellous CVFS
```

## Project Structure

```text
CVFS/
│
├── CVFS.c
├── README.md
└── .gitignore
```

## Concepts Learned

- File System
- Inode
- File Table
- UAREA
- UFDT
- File Descriptors
- File Permissions
- Linked List
- Dynamic Memory Allocation
- System Programming
- Command-Line Interface

## Author

**Pranav Avinash Narkhede**