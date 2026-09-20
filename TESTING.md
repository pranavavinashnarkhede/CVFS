# CVFS Testing

This document contains basic test cases used to verify the functionality of the Customised Virtual File System (CVFS).

## 1. File Creation

| Test | Command | Expected Result |
|---|---|---|
| Create valid file | `creat Demo.txt 3` | File is created successfully |
| Duplicate file | `creat Demo.txt 3` | File already exists error |
| Invalid permission | `creat Test.txt 8` | Invalid permission error |
| Invalid file name | `creat "" 3` | Invalid file name error |

## 2. File Open and Close

| Test | Command | Expected Result |
|---|---|---|
| Open file | `open Demo.txt 3` | File opened successfully |
| Invalid mode | `open Demo.txt 5` | Invalid mode error |
| Close file | `close 3` | File closed successfully |
| Invalid FD | `close 25` | Invalid file descriptor error |

## 3. Write Operation

| Test | Command | Expected Result |
|---|---|---|
| Write data | `write 3` | Data written successfully |
| Write without permission | Open read-only and write | Permission denied |
| Invalid FD | `write 25` | Invalid file descriptor error |

## 4. Read Operation

| Test | Command | Expected Result |
|---|---|---|
| Read data | `read 3 10` | Data read successfully |
| Read without permission | Open write-only and read | Permission denied |
| Invalid size | `read 3 0` | Invalid size error |
| Invalid FD | `read 25 10` | Invalid file descriptor error |

## 5. File Information

| Test | Command | Expected Result |
|---|---|---|
| Display information | `stat Demo.txt` | File information displayed |
| List files | `ls` | Existing files displayed |
| Detailed list | `ls -a` | Detailed file information displayed |

## 6. Rename

| Test | Command | Expected Result |
|---|---|---|
| Rename file | `rename Demo.txt Notes.txt` | File renamed successfully |
| Duplicate name | Rename to existing name | Error displayed |
| Invalid name | Invalid file name | Validation error |

## 7. Truncate

| Test | Command | Expected Result |
|---|---|---|
| Reduce file size | `truncate Notes.txt 5` | File size reduced |
| Invalid size | Negative size | Invalid size error |
| Size greater than file | Size > actual file size | Error displayed |

## 8. lseek

| Test | Command | Expected Result |
|---|---|---|
| Seek from start | `lseek 3 5 0 1` | Read/write offset changed |
| Seek from current | `lseek 3 2 1 1` | Offset changed from current position |
| Seek from end | `lseek 3 0 2 1` | Offset moved to end |
| Invalid FD | `lseek 25 5 0 1` | Invalid file descriptor error |

## 9. Unlink

| Test | Command | Expected Result |
|---|---|---|
| Delete closed file | `unlink Notes.txt` | File deleted successfully |
| Delete opened file | `unlink Demo.txt` | File cannot be deleted while opened |
| Non-existing file | `unlink Test.txt` | File not found error |

## 10. Resource Management

Verify that:

- File Table entries are released after `close`.
- File buffers are released after `unlink`.
- Inode resources are released during program termination.
- Reference count is updated after `open` and `close`.

## Testing Result

The implemented CVFS operations were tested using valid and invalid inputs to verify file operations, permissions, file descriptors, offsets, validation, and resource management.