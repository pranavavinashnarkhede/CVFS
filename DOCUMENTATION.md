# CVFS Documentation

## 1. Overview

CVFS (Customised Virtual File System) is an in-memory file system implemented in C.

It provides a custom command-line shell for performing basic file operations and demonstrates concepts such as Inode, File Table, UAREA, UFDT, file descriptors, permissions, offsets, reference counting, and dynamic memory management.

---

## 2. Commands

| Command | Syntax | Description |
|---|---|---|
| `help` | `help` | Displays available commands |
| `man` | `man` | Displays command documentation |
| `clear` | `clear` | Clears the terminal |
| `creat` | `creat <name> <permission>` | Creates a new file |
| `open` | `open <name> <mode>` | Opens an existing file |
| `close` | `close <fd>` | Closes an opened file |
| `ls` | `ls` | Lists existing files |
| `ls -a` | `ls -a` | Displays detailed file information |
| `stat` | `stat <name>` | Displays file information |
| `write` | `write <fd>` | Writes data into a file |
| `read` | `read <fd> <size>` | Reads data from a file |
| `rename` | `rename <old> <new>` | Renames a file |
| `truncate` | `truncate <name> <size>` | Changes the logical file size |
| `lseek` | `lseek <fd> <offset> <whence> <type>` | Changes file offset |
| `unlink` | `unlink <name>` | Deletes a file |
| `exit` | `exit` | Terminates CVFS |

---

## 3. File Permissions

CVFS uses three basic permission values:

| Value | Permission |
|---:|---|
| `1` | Read |
| `2` | Write |
| `4` | Execute |

Permissions can be combined:

| Value | Permission |
|---:|---|
| `0` | No permission |
| `1` | Read |
| `2` | Write |
| `3` | Read + Write |
| `4` | Execute |
| `5` | Read + Execute |
| `6` | Write + Execute |
| `7` | Read + Write + Execute |

Example:

```text
creat Demo.txt 3
```

creates a file with Read + Write permission.

---

## 4. File Open Modes

| Mode | Description |
|---:|---|
| `1` | Read |
| `2` | Write |
| `3` | Read + Write |

Example:

```text
open Demo.txt 3
```

---

## 5. File Descriptor

A file descriptor identifies an opened file.

User file descriptors are allocated from:

```text
3 - 19
```

Example:

```text
open Demo.txt 3
```

If the command returns FD `3`, that FD can be used with:

```text
read 3 10
write 3
close 3
lseek 3 ...
```

---

## 6. Inode

The Inode stores information about a file:

- File name
- Inode number
- File size
- Actual file size
- File type
- Reference count
- Permission
- Data buffer
- Link to the next Inode

---

## 7. File Table

The File Table represents an opened file and stores:

- Read offset
- Write offset
- Open mode
- Pointer to the corresponding Inode

The relationship is:

```text
File Descriptor
       |
       v
      UFDT
       |
       v
  File Table
       |
       v
     Inode
       |
       v
  Data Buffer
```

---

## 8. UAREA and UFDT

### UAREA

UAREA stores process-related information and contains the UFDT.

### UFDT

UFDT maintains references to File Table entries for opened files.

---

## 9. Reference Count

Each Inode maintains a `ReferenceCount`.

Opening a file increases the reference count:

```text
open → ReferenceCount++
```

Closing a file decreases it:

```text
close → ReferenceCount--
```

A file can be deleted using `unlink` only when its reference count is zero.

---

## 10. Read and Write Offsets

CVFS maintains separate offsets:

```text
ReadOffset
WriteOffset
```

The read offset determines where the next read starts.

The write offset determines where the next write starts.

---

## 11. lseek

`lseek` changes the file offset.

### Whence Values

| Value | Meaning |
|---:|---|
| `0` | Start of file |
| `1` | Current position |
| `2` | End of file |

Example:

```text
lseek 3 5 0 1
```

This changes the selected offset to 5 positions from the start.

---

## 12. truncate

`truncate` changes the logical size of a file.

Example:

```text
truncate Demo.txt 5
```

If the file contains:

```text
Hello CVFS
```

the logical file size becomes 5.

---

## 13. rename

`rename` changes the name of an existing file.

Example:

```text
rename Demo.txt Notes.txt
```

The file contents and Inode remain associated with the renamed file.

---

## 14. unlink

`unlink` removes an existing file.

Example:

```text
unlink Notes.txt
```

The file can be removed only when it is not currently opened.

---

## 15. Memory Management

CVFS uses dynamic memory allocation for file-system structures and file buffers.

Memory is allocated using:

```c
malloc()
```

and released using:

```c
free()
```

Resources are also released before program termination.

---

## 16. Input Validation

CVFS validates important inputs such as:

- File names
- Permissions
- Open modes
- File descriptors
- Read size
- Truncate size
- `lseek` parameters
- File existence
- Duplicate file names

---

## 17. Basic Workflow

```text
Create File
     |
     v
   Inode
     |
     v
    Open
     |
     v
File Descriptor
     |
     v
Read / Write / lseek / stat
     |
     v
   Close
     |
     v
  Unlink
```

---

## 18. Example Session

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

CVFS : > unlink Demo.txt

CVFS : > exit
Thank you for using CVFS
```

---

## 19. Compilation

Compile using GCC:

```bash
gcc CVFS.c -o CVFS
```

Run on Linux/macOS:

```bash
./CVFS
```

Run on Windows:

```bash
CVFS.exe
```

---

## Author

**Pranav Avinash Narkhede**
