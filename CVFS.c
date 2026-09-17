/////////////////////////////////////////////////////////////////////////////////////
//
//  Header Files Inclusion
//
////////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdbool.h>

/////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined Macros
//
////////////////////////////////////////////////////////////////////////////////////

# define MAXINODE 20
# define MAXFILESIZE 50
# define MAXOPENFILES 20

# define READ 1
# define WRITE 2
# define EXECUTE 4

# define START 0
# define CURRENT 1

# define END 2

# define EXECUTE_SUCCESS 0

# define REGULARFILE 1
# define SPECIALFILE 2


/////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined Macros for error handling
//
////////////////////////////////////////////////////////////////////////////////////

# define ERR_INVALID_PARAMETER -1

# define ERR_NO_INODES -2

# define ERR_FILE_ALREADY_EXISTS -3
# define ERR_FILE_NOT_EXISTS -4

# define ERR_PERMISSION_DENIED -5

# define ERR_INSUFFICIENT_SPACE -6
# define ERR_INSUFFICIENT_DATA -7

# define ERR_MAX_FILES_OPEN -8

# define ERR_INVALID_SIZE -9

/////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    Boot Block
//  Description :       It holds the information to boot the operating system   
//
////////////////////////////////////////////////////////////////////////////////////

struct BootBlock                    
{
    char Information[100];              
};

/////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    SuperBlock
//  Description :       It holds the information of complete File System
//
////////////////////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;

};

/////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    Inode 
//  Description :       It holds information of file
//
////////////////////////////////////////////////////////////////////////////////////    

#pragma pack(1)
struct Inode
{
    char FileName[20];              
    int InodeNumber ;
    int FileSize ;
    int ActualFileSize ;
    int FileType ;
    int ReferenceCount ;
    int Permission ;
    char *Buffer ;                      
    struct Inode *next;
};

typedef struct Inode INODE;
typedef struct Inode* PINODE;
typedef struct Inode** PPINODE;

/////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    FileTable
//  Description :       It holds information of opened files
//
////////////////////////////////////////////////////////////////////////////////////

#pragma pack(1)
struct FileTable
{
    int ReadOffset ;
    int WriteOffset ;
    int Mode ;                      
    PINODE ptrinode ;                   
};

typedef struct FileTable FILETABLE;
typedef struct FileTable* PFILETABLE;

/////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    UAREA
//  Description :       It holds information of process
//
////////////////////////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
};

////////////////////////////////////////////////////////////////////////////////////
//
//  Global Variables used in the project
//
////////////////////////////////////////////////////////////////////////////////////

struct BootBlock bootobj ;              
struct SuperBlock superobj ;
struct UAREA uareaobj ;

PINODE head = NULL ;   

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     InitialiseUAREA()   
//  Description   :     It is used to initialize UAREA
//  Author        :     Pranav Avinash Narkhede
//  Date          :     31/07/2026
//
////////////////////////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    int i = 0 ;

    strcpy(uareaobj.ProcessName , "Myexe");                     

    for(i = 0 ; i < MAXOPENFILES ; i++)
    {
        uareaobj.UFDT[i] = NULL ;
    }

    printf("Marvellous CVFS : UAREA gets initialize successfully\n");

}


/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     InitialiseSuperBlock()   
//  Description   :     It is used to initialize super block
//  Author        :     Pranav Avinash Narkhede
//  Date          :     31/07/2026
//
////////////////////////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE ;
    superobj.FreeInodes = MAXINODE ;

    printf("Marvellous CVFS : Super Block gets initialize successfully\n");
    
}


/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     CreateDILB()   
//  Description   :     It is used to create linked list of inodes 
//  Author        :     Pranav Avinash Narkhede
//  Date          :     31/07/2026
//
////////////////////////////////////////////////////////////////////////////////////


int CreateDILB() 
{ 
    PINODE temp = NULL; 
    PINODE newn = NULL; 
    PINODE current = NULL; 
    PINODE next = NULL; 
 
    int i = 0; 
 
    temp = head; 
 
    // Allocate and initialise all inode nodes
    for(i = 1; i <= MAXINODE; i++) 
    { 
        newn = (PINODE)malloc(sizeof(INODE)); 
 
        if(newn == NULL) 
        { 
            // Release previously allocated inode nodes
            current = head; 
 
            while(current != NULL) 
            { 
                next = current->next; 
                free(current); 
                current = next; 
            } 
 
            head = NULL; 
 
            return ERR_INSUFFICIENT_SPACE; 
        } 
 
        newn->InodeNumber = i; 
        strcpy(newn->FileName, "\0"); 
        newn->FileSize = 0; 
        newn->ActualFileSize = 0; 
        newn->FileType = 0; 
        newn->ReferenceCount = 0; 
        newn->Permission = 0; 
        newn->Buffer = NULL; 
        newn->next = NULL; 
 
        if(temp == NULL) 
        { 
            head = newn; 
            temp = head; 
        } 
        else 
        { 
            temp->next = newn; 
            temp = temp->next; 
        } 
    } 
 
    printf("Marvellous CVFS : DILB gets created successfully\n"); 
 
    return EXECUTE_SUCCESS; 
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     StartAuxillaryDataInitialisation()   
//  Description   :     It is used to call such functions which are used to 
//                      initialise auxillary data. 
//  Author        :     Pranav Avinash Narkhede
//  Date          :     31/07/2026
//
////////////////////////////////////////////////////////////////////////////////////

int StartAuxillaryDataInitialisation() 
{ 
    int iRet = 0; 
 
    // Set the boot completion message
    strcpy(bootobj.Information, "Booting process of Marvellous CVFS is completed"); 
 
    printf("%s\n", bootobj.Information); 
     
    // Initialise User Area
    InitialiseUAREA(); 
 
    // Initialise Super Block
    InitialiseSuperBlock(); 
 
    // Create and initialise the inode list
    iRet = CreateDILB(); 
 
    if(iRet == ERR_INSUFFICIENT_SPACE) 
    { 
        // Stop initialisation if inode allocation fails
        return ERR_INSUFFICIENT_SPACE;   
    } 
 
    return EXECUTE_SUCCESS; 
}
/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     DisplayHelp()   
//  Description   :     It is used to display help to the user of project  
//                      initialise auxillary data. 
//  Author        :     Pranav Avinash Narkhede
//  Date          :     01/08/2026
//
////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp() 
{ 
    printf("---------------------------------------------------------------------\n"); 
    printf("-------------------- Marvellous CVFS Help Page ----------------------\n"); 
    printf("---------------------------------------------------------------------\n"); 
    printf("help : It is used to display the help page\n"); 
    printf("man : It is used to display the manual page of a command\n"); 
    printf("clear : It is used to clear the terminal screen\n"); 
    printf("ls : It is used to list all files\n"); 
    printf("creat : It is used to create a new regular file\n"); 
    printf("open : It is used to open an existing file\n"); 
    printf("close : It is used to close an opened file\n"); 
    printf("write : It is used to write data into an opened file\n"); 
    printf("read : It is used to read data from an opened file\n"); 
    printf("stat : It is used to display information of a specific file\n"); 
    printf("unlink : It is used to delete an existing file\n"); 
    printf("rename : It is used to rename an existing file\n"); 
    printf("lseek : It is used to change the read or write offset of an opened file\n");
    printf("\ttruncate\t\t: Truncate an existing file\n"); 
    printf("exit : It is used to terminate Marvellous CVFS\n"); 
    printf("---------------------------------------------------------------------\n"); 
}


/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     ManPageDisplay()
//  Description   :     It is used to display the manual page of a specific command.
//  Input         :     Name of command
//  Output        :     None
//  Author        :     Pranav Avinash Narkhede
//  Date          :     16/09/2026
//
////////////////////////////////////////////////////////////////////////////////////

void ManPageDisplay(char Name[])
{
    if(strcmp(Name, "help") == 0)
    {
        printf("\nHELP\n");
        printf("About  : Display all available CVFS commands.\n");
        printf("Usage  : help\n");
    }
    else if(strcmp(Name, "man") == 0)
    {
        printf("\nMAN\n");
        printf("About  : Display manual information for a command.\n");
        printf("Usage  : man Command_Name\n");
    }
    else if(strcmp(Name, "clear") == 0)
    {
        printf("\nCLEAR\n");
        printf("About  : Clear the terminal screen.\n");
        printf("Usage  : clear\n");
    }
    else if(strcmp(Name, "ls") == 0)
    {
        printf("\nLS\n");
        printf("About  : Display files available in CVFS.\n");
        printf("Usage  : ls\n");
        printf("Usage  : ls -a\n");
        printf("ls -a  : Display detailed file information.\n");
    }
    else if(strcmp(Name, "creat") == 0)
    {
        printf("\nCREAT\n");
        printf("About  : Create a new regular file.\n");
        printf("Usage  : creat File_Name Permission\n");
        printf("Permission:\n");
        printf("  1 : Read\n");
        printf("  2 : Write\n");
        printf("  3 : Read + Write\n");
        printf("  4 : Execute\n");
        printf("  5 : Read + Execute\n");
        printf("  6 : Write + Execute\n");
        printf("  7 : Read + Write + Execute\n");
    }
    else if(strcmp(Name, "open") == 0)
    {
        printf("\nOPEN\n");
        printf("About  : Open an existing file.\n");
        printf("Usage  : open File_Name Mode\n");
        printf("Mode:\n");
        printf("  1 : Read\n");
        printf("  2 : Write\n");
        printf("  3 : Read + Write\n");
    }
    else if(strcmp(Name, "close") == 0)
    {
        printf("\nCLOSE\n");
        printf("About  : Close an opened file.\n");
        printf("Usage  : close File_Descriptor\n");
    }
    else if(strcmp(Name, "write") == 0)
    {
        printf("\nWRITE\n");
        printf("About  : Write data into an opened file.\n");
        printf("Usage  : write File_Descriptor\n");
        printf("Note   : File must have WRITE permission.\n");
    }
    else if(strcmp(Name, "read") == 0)
    {
        printf("\nREAD\n");
        printf("About  : Read data from an opened file.\n");
        printf("Usage  : read File_Descriptor Size\n");
        printf("Note   : File must have READ permission.\n");
    }
    else if(strcmp(Name, "stat") == 0)
    {
        printf("\nSTAT\n");
        printf("About  : Display information about a file.\n");
        printf("Usage  : stat File_Name\n");
        printf("Shows  : File name, inode, size, permissions and type.\n");
    }
    else if(strcmp(Name, "unlink") == 0)
    {
        printf("\nUNLINK\n");
        printf("About  : Delete an existing file.\n");
        printf("Usage  : unlink File_Name\n");
        printf("Note   : File must not be opened.\n");
    }
    else if(strcmp(Name, "rename") == 0)
    {
        printf("\nRENAME\n");
        printf("About  : Rename an existing file.\n");
        printf("Usage  : rename Old_File_Name New_File_Name\n");
    }
    else if(strcmp(Name, "lseek") == 0)
    {
        printf("\nLSEEK\n");
        printf("About  : Change the read or write offset of an opened file.\n");
        printf("Usage  : lseek File_Descriptor Offset Whence Offset_Type\n");
        printf("Whence : start, current, end\n");
        printf("Type   : read or write\n");
        printf("Example: lseek 3 5 start read\n");
    }
    else if(strcmp(Name, "truncate") == 0)
    {
        printf("\nTRUNCATE\n");
        printf("About  : Reduce the size of an existing file.\n");
        printf("Usage  : truncate File_Name Size\n");
        printf("Note   : New size must not be greater than current size.\n");
        printf("Note   : File must have WRITE permission.\n");
        printf("Example: truncate test.txt 10\n");
    }
    else if(strcmp(Name, "exit") == 0)
    {
        printf("\nEXIT\n");
        printf("About  : Terminate CVFS.\n");
        printf("Usage  : exit\n");
    }
    else
    {
        printf("No manual entry found for %s\n", Name);
    }
}


/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     IsFileExists()   
//  Description   :     It is used to check whether the file is present or not .   
//  Input         :     Name of file 
//  Output        :     True if present 
//                      False if not present
//  Author        :     Pranav Avinash Narkhede
//  Date          :     01/08/2026
//
////////////////////////////////////////////////////////////////////////////////////

bool IsFileExists(
                        char name[]         // name of file
                ) 
{ 
    PINODE temp = head; 
    bool bFlag = false; 
 
    // Traverse the inode list to search for the given file name
    while(temp != NULL) 
    { 
        if(strcmp(temp->FileName, name) == 0) 
        { 
            // File found in the inode list
            bFlag = true; 
            break; 
        } 
 
        // Move to the next inode
        temp = temp->next; 
    } 
 
    return bFlag; 
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     CreateFile()   
//  Description   :     It is used to create new file .   
//  Input         :     Name of file & permissions
//  Output        :     File Descriptor
//  Author        :     Pranav Avinash Narkhede
//  Date          :     01/08/2026
//
////////////////////////////////////////////////////////////////////////////////////

int CreateFile(
                    char name[] ,                   // Name of File 
                    int permission                  // File permission
                )
{
    PINODE temp = head;
    int i = 0;

    // Check whether a free inode is available
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    // Validate the file permission
    // permission = 1 -> read
    // permission = 2 -> write
    // permission = 3 -> read + write
    // permission = 4 -> execute
    // permission = 5 -> read + execute
    // permission = 6 -> write + execute
    // permission = 7 -> read + write + execute

    if(permission < 1 || permission > 7)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check whether a file with the same name already exists
    if(IsFileExists(name) == true)
    {
        return ERR_FILE_ALREADY_EXISTS;
    }

    // Validate the file name length
    if(strlen(name) >= 20)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Search the inode list for a free inode
    while(temp != NULL)
    {
        if(temp->FileType == 0)
        {
            break;
        }

        temp = temp->next;
    }

    // Check whether a free inode was found
    if(temp == NULL)
    {
        return ERR_NO_INODES;
    }

    // Search the UFDT for a free file descriptor entry
    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    // Check whether all UFDT entries are occupied
    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate memory for the file table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Check whether file table memory was allocated successfully
    if(uareaobj.UFDT[i] == NULL)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Initialise the file table
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = permission;

    // Link the file table with the allocated inode
    uareaobj.UFDT[i]->ptrinode = temp;

    // Initialise the inode information
    strcpy(temp->FileName, name);

    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;
    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;
    uareaobj.UFDT[i]->ptrinode->Permission = permission;

    // Allocate memory for the file data buffer
    uareaobj.UFDT[i]->ptrinode->Buffer = (char*)malloc(MAXFILESIZE);

    // Roll back the file creation if buffer allocation fails
    if(uareaobj.UFDT[i]->ptrinode->Buffer == NULL)
    {
        free(uareaobj.UFDT[i]);
        uareaobj.UFDT[i] = NULL;

        // Reset the inode to its free state
        strcpy(temp->FileName, "\0");
        temp->FileSize = 0;
        temp->ActualFileSize = 0;
        temp->FileType = 0;
        temp->ReferenceCount = 0;
        temp->Permission = 0;
        temp->Buffer = NULL;

        return ERR_INSUFFICIENT_SPACE;
    }

    // Update the number of available inodes
    superobj.FreeInodes--;

    return i;              // Return the file descriptor
}


/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     LsFile()   
//  Description   :     It is used to display names of all files  .   
//  Input         :     None
//  Output        :     None
//  Author        :     Pranav Avinash Narkhede
//  Date          :     01/08/2026
//
////////////////////////////////////////////////////////////////////////////////////

void LsFile()
{
    PINODE temp = head;

    printf("---------------------------------------------------------------------\n");
    printf("----------------- Marvellous CVFS Files Information ------------------\n");
    printf("---------------------------------------------------------------------\n");

    // Traverse the inode list to display all existing files
    while(temp != NULL)
    {
        // Display the file name if the inode represents a valid file
        if(temp->FileType != 0)
        {
            printf("%s\n", temp->FileName);
        }

        // Move to the next inode
        temp = temp->next;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     LsFile_All()   
//  Description   :     It is used to display all details of all files  .   
//  Input         :     None
//  Output        :     None
//  Author        :     Pranav Avinash Narkhede
//  Date          :     16/09/2026
//
////////////////////////////////////////////////////////////////////////////////////

void LsFile_All()
{
    PINODE temp = head;
    int iCount = 0;
    int file_permission = 0;

    char filetype[20] = {'\0'};
    char permission[4] = {'\0'};

    printf("---------------------------------------------------------------------\n");
    printf("----------------- Marvellous CVFS Files Information ------------------\n");
    printf("---------------------------------------------------------------------\n");

    printf("---------------------------------------------------------------------\n");
    printf("%-20s %-10s %-10s %-12s %-12s\n",
        "File Name",
        "Inode",
        "Size",
        "Type",
        "Permission");
    printf("---------------------------------------------------------------------\n");

    // Traverse the inode list to display information about all existing files
    while(temp != NULL)
    {
        // Skip unused inodes
        if(strlen(temp->FileName) == 0)
        {
            temp = temp->next;
            continue;
        }

        // Determine the type of the current file
        if(temp->FileType == REGULARFILE)
        {
            strcpy(filetype, "Regular");
        }
        else
        {
            strcpy(filetype, "Special");
        }

        // Get the permission value of the current file
        file_permission = temp->Permission;

        // Convert read permission into 'r'
        if((file_permission & READ) == READ)
        {
            permission[0] = 'r';
        }
        else
        {
            permission[0] = '-';
        }

        // Convert write permission into 'w'
        if((file_permission & WRITE) == WRITE)
        {
            permission[1] = 'w';
        }
        else
        {
            permission[1] = '-';
        }

        // Convert execute permission into 'x'
        if((file_permission & EXECUTE) == EXECUTE)
        {
            permission[2] = 'x';
        }
        else
        {
            permission[2] = '-';
        }

        // Terminate the permission string
        permission[3] = '\0';

        // Display the current file information
        printf("%-20s %-10d %-10d %-12s %-12s\n",
            temp->FileName,
            temp->InodeNumber,
            temp->ActualFileSize,
            filetype,
            permission);

        // Increment the total file count
        iCount++;

        // Move to the next inode
        temp = temp->next;
    }

    printf("---------------------------------------------------------------------\n");
    printf("Total Files : %d\n", iCount);
    printf("Free Inodes : %d\n", superobj.FreeInodes);
    printf("---------------------------------------------------------------------\n");
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     stat_file()   
//  Description   :     It is used to display all details of specific files  .   
//  Input         :     File Name
//  Output        :     Exit status of function
//  Author        :     Pranav Avinash Narkhede
//  Date          :     02/08/2026
//
////////////////////////////////////////////////////////////////////////////////////

int stat_file(
                char name[]                 // Name of file
            )
{
    PINODE temp = NULL;
    int Permission = 0;
    int Type = 0;

    // Check whether the requested file exists
    if(IsFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    temp = head;

    // Search the inode list for the requested file
    while(temp != NULL)
    {
        if(strcmp(temp->FileName, name) == 0)
        {
            printf("---------------------------------------------------------------------\n");
            printf("------------------ Statistical Information of File ------------------\n");
            printf("---------------------------------------------------------------------\n");

            // Display basic file information
            printf("File Name : %s\n", temp->FileName);
            printf("Inode Number : %d\n", temp->InodeNumber);
            printf("File Size : %d\n", temp->FileSize);
            printf("Actual File Size : %d\n", temp->ActualFileSize);
            printf("Reference Count : %d\n", temp->ReferenceCount);

            // Retrieve and display the file permissions
            Permission = temp->Permission;

            if((Permission & READ) == READ)
            {
                printf("File Permission : READ ");
            }

            if((Permission & WRITE) == WRITE)
            {
                printf("WRITE ");
            }

            if((Permission & EXECUTE) == EXECUTE)
            {
                printf("EXECUTE ");
            }

            printf("\n");

            // Retrieve and display the file type
            Type = temp->FileType;

            if(Type == REGULARFILE)
            {
                printf("File Type : Regular File\n");
            }
            else if(Type == SPECIALFILE)
            {
                printf("File Type : Special File\n");
            }

            printf("---------------------------------------------------------------------\n");

            // Stop searching after finding the requested file
            break;
        }

        // Move to the next inode
        temp = temp->next;
    }

    return EXECUTE_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     unlink_file()   
//  Description   :     It is used to delete the specific file .   
//  Input         :     File Name
//  Output        :     Exit status of function
//  Author        :     Pranav Avinash Narkhede
//  Date          :     02/08/2026
//
////////////////////////////////////////////////////////////////////////////////////

int unlink_file(
                    char name[]         // Name of file
                )
{
    PINODE temp = NULL;

    // Check whether the requested file exists
    if(IsFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    temp = head;

    // Traverse the inode list to find the requested file
    while(temp != NULL)
    {
        if(strcmp(temp->FileName, name) == 0)
        {
            // A file can be deleted only when it is not currently open
            if(temp->ReferenceCount == 0)
            {
                // Reset the inode to its free state
                temp->FileName[0] = '\0';
                temp->FileSize = 0;
                temp->ActualFileSize = 0;
                temp->FileType = 0;
                temp->Permission = 0;

                // Release the memory allocated for file data
                free(temp->Buffer);
                temp->Buffer = NULL;

                // Make the inode available for future file creation
                superobj.FreeInodes++;

                break;
            }
            else
            {
                // Prevent deletion while the file is open
                return ERR_PERMISSION_DENIED;
            }
        }

        // Move to the next inode
        temp = temp->next;
    }

    return EXECUTE_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     write_file()   
//  Description   :     It is used to write the data into specific file .   
//  Input         :     File Descriptor , Data that we want to write , Size of Data
//  Output        :     Number of bytes successfully written
//  Author        :     Pranav Avinash Narkhede
//  Date          :     02/08/2026
//
////////////////////////////////////////////////////////////////////////////////////

int write_file(int fd, char *arr, int size)
{
    int iRet = 0;
    PINODE temp = NULL;

    // Validate the file descriptor
    if(fd < 0 || fd >= MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check whether the file is currently open
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    temp = uareaobj.UFDT[fd]->ptrinode;

    // Verify WRITE permission for the file
    if((temp->Permission & WRITE) != WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Check whether sufficient space is available in the file buffer
    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Copy data into the file buffer at the current write offset
    strncpy(temp->Buffer + uareaobj.UFDT[fd]->WriteOffset, arr, size);

    // Advance the write offset by the number of bytes written
    uareaobj.UFDT[fd]->WriteOffset = uareaobj.UFDT[fd]->WriteOffset + size;

    // Get the new write position
    iRet = uareaobj.UFDT[fd]->WriteOffset;

    // Update actual file size when data extends the end of the file
    if(iRet > temp->ActualFileSize)
    {
        temp->ActualFileSize = iRet;
    }

    return size;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     read_file()   
//  Description   :     It is used to read the data from specific file .   
//  Input         :     File Descriptor , Address of empty buffer , Size of Data
//  Output        :     Number of bytes successfully read
//  Author        :     Pranav Avinash Narkhede
//  Date          :     02/08/2026
//
////////////////////////////////////////////////////////////////////////////////////

int read_file(int fd, char *arr, int size)
{
    int iRet = 0;
    int remainingData = 0;
    PINODE temp = NULL;

    // Validate the file descriptor
    if(fd < 0 || fd >= MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check whether the file is currently open
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Validate the requested read size
    if(size < 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    temp = uareaobj.UFDT[fd]->ptrinode;

    // Verify READ permission for the file
    if((temp->Permission & READ) != READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Check whether the read offset has reached the end of the file
    if(uareaobj.UFDT[fd]->ReadOffset >= temp->ActualFileSize)
    {
        return 0;
    }

    // Calculate the amount of data remaining from the current read offset
    remainingData = temp->ActualFileSize - uareaobj.UFDT[fd]->ReadOffset;

    // Limit the read size to the available file data
    if(size > remainingData)
    {
        size = remainingData;
    }

    // Copy data from the file buffer into the output buffer
    strncpy(arr , temp->Buffer + uareaobj.UFDT[fd]->ReadOffset, size);

    // Terminate the output string
    arr[size] = '\0';

    // Advance the read offset by the number of bytes read
    uareaobj.UFDT[fd]->ReadOffset = uareaobj.UFDT[fd]->ReadOffset + size;

    iRet = size;

    return iRet;
}
/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     openFile()
//  Description   :     It is used to open a specific file in the specified mode.
//  Input         :     File Name , Mode
//  Output        :     File Descriptor
//  Author        :     Pranav Avinash Narkhede
//  Date          :     15/09/2026
//
/////////////////////////////////////////////////////////////////////////////////////
int openFile(char name[], int mode)
{
    PINODE temp = NULL;
    int i = 0;

    // Validate the requested access mode
    if(mode < READ || mode > (READ + WRITE))
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check whether the requested file exists
    if(IsFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Search the inode list for the requested file
    temp = head;

    while(temp != NULL)
    {
        if(strcmp(temp->FileName, name) == 0)
        {
            break;
        }

        // Move to the next inode
        temp = temp->next;
    }

    // Verify whether the requested access mode is permitted
    if((temp->Permission & mode) != mode)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Search the UFDT for a free file descriptor entry
    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    // Check whether a free UFDT entry is available
    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate memory for the file table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Check whether file table memory was allocated successfully
    if(uareaobj.UFDT[i] == NULL)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Initialise the file table
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = mode;

    // Link the file table with the corresponding inode
    uareaobj.UFDT[i]->ptrinode = temp;

    // Increment the reference count for the opened file
    temp->ReferenceCount++;

    // Return the allocated file descriptor
    return i;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     closeFile()
//  Description   :     It is used to close an opened file.
//  Input         :     File Descriptor
//  Output        :     Success / Error code
//  Author        :     Pranav Avinash Narkhede
//  Date          :     16/09/2026
//
/////////////////////////////////////////////////////////////////////////////////////
int closeFile(int fd)
{
    // Validate the file descriptor
    if(fd < 0 || fd >= MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check whether the file descriptor is currently in use
    if(uareaobj.UFDT[fd] != NULL)
    {
        // Decrease the reference count of the associated inode
        uareaobj.UFDT[fd]->ptrinode->ReferenceCount--;

        // Release the memory allocated for the file table
        free(uareaobj.UFDT[fd]);

        // Mark the UFDT entry as available
        uareaobj.UFDT[fd] = NULL;
    }
    else
    {
        return ERR_FILE_NOT_EXISTS;
    }

    return EXECUTE_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     DeallocateResources()
//  Description   :     It is used to deallocate all the resources allocated
//                      by the CVFS before terminating the program.
//  Input         :     None
//  Output        :     None
//  Author        :     Pranav Avinash Narkhede
//  Date          :     16/09/2026
//
/////////////////////////////////////////////////////////////////////////////////////
void DeallocateResources()
{
    int i = 0;
    PINODE temp = NULL;
    PINODE next = NULL;

    // Release all allocated FileTable structures
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            free(uareaobj.UFDT[i]);
            uareaobj.UFDT[i] = NULL;
        }
    }

    // Traverse the inode list and release file buffers and inode structures
    temp = head;

    while(temp != NULL)
    {
        // Store the next inode before releasing the current inode
        next = temp->next;

        // Release the memory allocated for file data
        if(temp->Buffer != NULL)
        {
            free(temp->Buffer);
            temp->Buffer = NULL;
        }

        // Release the current inode
        free(temp);

        // Move to the next inode
        temp = next;
    }

    // Reset the inode list
    head = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     lseekFile()
//  Description   :     It is used to change the read or write offset of an
//                      opened file based on the specified position.
//  Input         :     File Descriptor, Offset, Whence, Offset Type
//  Output        :     New File Offset / Error Code
//  Author        :     Pranav Avinash Narkhede
//  Date          :     16/09/2026
//
/////////////////////////////////////////////////////////////////////////////////////

int lseekFile(int fileDescriptor, int offset, int whence, int offsetType)
{
    int currentOffset = 0;
    int newOffset = 0;

    // Validate the file descriptor
    if(fileDescriptor < 0 || fileDescriptor >= MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check whether the file descriptor is currently in use
    if(uareaobj.UFDT[fileDescriptor] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Validate the reference position
    if(whence < START || whence > END)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Validate the offset type
    if(offsetType != READ && offsetType != WRITE)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Get the current read or write offset
    if(offsetType == READ)
    {
        currentOffset = uareaobj.UFDT[fileDescriptor]->ReadOffset;
    }
    else
    {
        currentOffset = uareaobj.UFDT[fileDescriptor]->WriteOffset;
    }

    // Calculate the new offset based on the specified reference position
    if(whence == START)
    {
        newOffset = offset;
    }
    else if(whence == CURRENT)
    {
        newOffset = currentOffset + offset;
    }
    else
    {
        newOffset =
            uareaobj.UFDT[fileDescriptor]->ptrinode->ActualFileSize + offset;
    }

    // Prevent the offset from becoming negative
    if(newOffset < 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Update the selected read or write offset
    if(offsetType == READ)
    {
        uareaobj.UFDT[fileDescriptor]->ReadOffset = newOffset;
    }
    else
    {
        uareaobj.UFDT[fileDescriptor]->WriteOffset = newOffset;
    }

    return newOffset;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     renameFile()
//  Description   :     It is used to rename an existing file.
//  Input         :     Old file name and New file name
//  Output        :     Success / Error code
//  Author        :     Pranav Avinash Narkhede
//  Date          :     16/09/2026
//
/////////////////////////////////////////////////////////////////////////////////////
int renameFile(char oldName[], char newName[])
{
    PINODE temp = NULL;

    // Validate the old and new file names
    if((strlen(oldName) <= 0 || strlen(oldName) >= 20) ||
       (strlen(newName) <= 0 || strlen(newName) >= 20))
    {
        return ERR_INVALID_PARAMETER;
    }

    temp = head;

    // Check whether the new file name is already in use
    while(temp != NULL)
    {
        if(strcmp(temp->FileName, newName) == 0)
        {
            break;
        }

        // Move to the next inode
        temp = temp->next;
    }

    // Prevent renaming if the new name already exists
    if(temp != NULL)
    {
        return ERR_FILE_ALREADY_EXISTS;
    }

    temp = head;

    // Search the inode list for the file to be renamed
    while(temp != NULL)
    {
        if(strcmp(temp->FileName, oldName) == 0)
        {
            break;
        }

        // Move to the next inode
        temp = temp->next;
    }

    // Check whether the old file was found
    if(temp == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Update the file name in the corresponding inode
    strcpy(temp->FileName, newName);

    return EXECUTE_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     truncateFile()
//  Description   :     It is used to reduce the size of an existing file.
//  Input         :     File name and new file size
//  Output        :     Success / Error code
//  Author        :     Pranav Avinash Narkhede
//  Date          :     17/09/2026
//
/////////////////////////////////////////////////////////////////////////////////////

int truncateFile(char FileName[], int Size)
{
    PINODE temp = NULL;
    int i = 0;

    temp = head;

    // Validate the requested file size
    if(Size < 0)
    {
        return ERR_INVALID_SIZE;
    }

    // Search the inode list for the requested file
    while(temp != NULL)
    {
        if(strcmp(temp->FileName, FileName) == 0)
        {
            break;
        }

        // Move to the next inode
        temp = temp->next;
    }

    // Check whether the requested file exists
    if(temp == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Prevent increasing the file size using truncate
    if(Size > temp->ActualFileSize)
    {
        return ERR_INVALID_SIZE;
    }

    // Verify WRITE permission for the file
    if((temp->Permission & WRITE) != WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Update the actual file size
    temp->ActualFileSize = Size;

    // Adjust offsets of all open file descriptors associated with this inode
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL && uareaobj.UFDT[i]->ptrinode == temp)
        {
            if(uareaobj.UFDT[i]->ReadOffset > Size)
            {
                uareaobj.UFDT[i]->ReadOffset = Size;
            }

            if(uareaobj.UFDT[i]->WriteOffset > Size)
            {
                uareaobj.UFDT[i]->WriteOffset = Size;
            }
        }
    }

    return EXECUTE_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     IsNumber()
//  Description   :     It is used to check whether the given string contains
//                      only numeric characters.
//  Input         :     String
//  Output        :     True if string contains only numbers
//                      False if string contains non-numeric characters
//  Author        :     Pranav Avinash Narkhede
//  Date          :     17/09/2026
//
/////////////////////////////////////////////////////////////////////////////////////
int IsNumber(char str[])
{
    int i = 0;

    // Check whether the input string is empty
    if(str[0] == '\0')
        return 0;

    // Check whether every character is a numeric digit
    while(str[i] != '\0')
    {
        if(str[i] < '0' || str[i] > '9')
            return 0;

        i++;
    }

    // Return true when the string contains only numeric characters
    return 1;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     IsValidFileName()
//  Description   :     It is used to check whether the given file name is valid.
//  Input         :     String
//  Output        :     True if file name is valid
//                      False if file name is invalid
//  Author        :     Pranav Avinash Narkhede
//  Date          :     17/09/2026
//
/////////////////////////////////////////////////////////////////////////////////////
bool IsValidFileName(char name[])
{
    // Check whether the file name is empty
    if(name[0] == '\0')
        return false;

    // Check whether the file name exceeds the maximum allowed length
    if(strlen(name) >= 20)
        return false;

    // File name satisfies the required validation rules
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Entry Point Function of the CVFS Project
//
////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // Input command buffer
    char str[80] = {'\0'};

    // Tokenised command
    char Command[5][20] = {{'\0'}};

    // Buffer used to store data entered for the write operation
    char InputBufferr[MAXFILESIZE] = {'\0'};

    int iRet = 0;
    int iCount = 0;
    int iSize = 0;

    // Buffer used to store data returned by the read operation
    char *EmptyBuffer = NULL;

    // Initialise CVFS auxiliary data structures
    iRet = StartAuxillaryDataInitialisation();

    if(iRet == ERR_INSUFFICIENT_SPACE)
    {
        printf("Error : Unable to initialise Marvellous CVFS\n");
        printf("Reason : Insufficient memory to create inode structure\n");
        return ERR_INSUFFICIENT_SPACE;
    }

    printf("---------------------------------------------------------------------\n");
    printf("--------------- Marvellous CVFS started Successfully ----------------\n");
    printf("---------------------------------------------------------------------\n");

    // Start the command-line shell
    while(1)
    {
        strcpy(str, "");

        printf("\nMarvellous CVFS : > ");
        fgets(str, sizeof(str), stdin);

        // Tokenise the user command and its arguments
        iCount = sscanf(str, "%19s %19s %19s %19s %19s", Command[0] ,Command[1] ,Command[2] ,Command[3] ,Command[4]);

        // Process commands that require no arguments
        if(iCount == 1)
        {
            // exit
            if(strcmp(Command[0], "exit") == 0)
            {
                printf("Marvellous CVFS : Deallocating all resources...\n");

                // Release all dynamically allocated resources
                DeallocateResources();

                printf("Marvellous CVFS : All resources deallocated successfully\n");

                break;
            }

            // help
            else if(strcmp(Command[0], "help") == 0)
            {
                DisplayHelp();
            }

            // clear
            else if(strcmp(Command[0], "clear") == 0)
            {
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
            }

            // ls
            else if(strcmp(Command[0], "ls") == 0)
            {
                LsFile();
            }

            else
            {
                printf("Command not found\n");
                printf("Please refer help option to get more information\n");
                printf("Please refer manual page of command using man\n");
            }
        }

        // Process commands that require one argument
        else if(iCount == 2)
        {
            // man command
            if(strcmp(Command[0], "man") == 0)
            {
                ManPageDisplay(Command[1]);
            }

            // ls -a
            else if((strcmp(Command[0], "ls") == 0) && (strcmp(Command[1], "-a") == 0))
            {
                LsFile_All();
            }

            // stat filename
            else if(strcmp(Command[0], "stat") == 0)
            {
                iRet = stat_file(Command[1]);

                if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : File not exist\n");
                }
            }

            // unlink filename
            else if(strcmp(Command[0], "unlink") == 0)
            {
                iRet = unlink_file(Command[1]);

                if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : File not exist\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : File is currently opened\n");
                }
                else
                {
                    printf("File unlinked successfully\n");
                }
            }

            // write fd
            else if(strcmp(Command[0], "write") == 0)
            {
                // Validate the file descriptor input
                if(IsNumber(Command[1]) == 0)
                {
                    printf("Error : Invalid Number\n");
                    continue;
                }

                // Validate the file descriptor range
                if(atoi(Command[1]) < 3 || atoi(Command[1]) >= MAXOPENFILES)
                {
                    printf("Error : Invalid File Descriptor\n");
                    continue;
                }

                printf("Enter the data that you want to write the file\n");

                // Read data to be written into the file
                fgets(InputBufferr, MAXFILESIZE, stdin);

                iSize = strlen(InputBufferr);

                // Remove the newline character added by fgets()
                if(iSize > 0 && InputBufferr[iSize - 1] == '\n')
                {
                    InputBufferr[iSize - 1] = '\0';
                    iSize--;
                }

                // Prevent writing empty data
                if(iSize == 0)
                {
                    printf("Error : Data cannot be empty\n");
                    continue;
                }

                // Perform the write operation
                iRet = write_file( atoi(Command[1]), InputBufferr, iSize);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameter\n");
                    printf("Please read man page for more information\n");
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : File not exist\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : There is no permission to write the data\n");
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : There is no space to write the data\n");
                }
                else
                {
                    printf("%d bytes gets successfully written into the file\n",iRet);
                }
            }

            // close fd
            else if(strcmp(Command[0], "close") == 0)
            {
                // Validate the file descriptor input
                if(IsNumber(Command[1]) == 0)
                {
                    printf("Error : Invalid Number\n");
                    continue;
                }

                // Validate the file descriptor range
                if(atoi(Command[1]) < 3 || atoi(Command[1]) >= MAXOPENFILES)
                {
                    printf("Error : Invalid File Descriptor\n");
                    continue;
                }

                // Close the specified file descriptor
                iRet = closeFile(atoi(Command[1]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid File Descriptor\n");
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : File is not opened\n");
                }
                else
                {
                    printf("File closed successfully\n");
                }
            }

            else
            {
                printf("Command not found\n");
                printf("Please refer help option to get more information\n");
                printf("Please refer manual page of command using man\n");
            }
        }

        // Process commands that require two arguments
        else if(iCount == 3)
        {
            // creat filename permission
            if(strcmp(Command[0], "creat") == 0)
            {
                // Validate the permission input
                if(IsNumber(Command[2]) == 0)
                {
                    printf("Error : Invalid Number\n");
                    continue;
                }

                // Validate the file name
                if(IsValidFileName(Command[1]) == false)
                {
                    printf("Error : Invalid file name\n");
                    continue;
                }

                // Create the requested file
                iRet = CreateFile(Command[1],atoi(Command[2]));

                if(iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because there is no free inode\n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because parameters of command are invalid.\n");
                    printf("Please use man page to get actual parameters\n");
                }
                else if(iRet == ERR_FILE_ALREADY_EXISTS)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because the file name is already present\n");
                    printf("Please use ls command to check names of all files\n");
                }
                else if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because the UFDT is full\n");
                    printf("Please close some opened file\n");
                }
                else
                {
                    printf("File successfully created with fd : %d\n",iRet);
                }
            }

            // read fd size
            else if(strcmp(Command[0], "read") == 0)
            {
                // Validate numeric arguments
                if((IsNumber(Command[1]) == 0) || (IsNumber(Command[2]) == 0))
                {
                    printf("Error : Invalid Number\n");
                    continue;
                }

                // Validate the file descriptor
                if(atoi(Command[1]) < 3 || atoi(Command[1]) >= MAXOPENFILES)
                {
                    printf("Error : Invalid File Descriptor\n");
                    continue;
                }

                // Validate the requested read size
                iSize = atoi(Command[2]);

                if(iSize <= 0 || iSize > MAXFILESIZE)
                {
                    printf("Error : Invalid file size\n");
                    continue;
                }

                // Allocate memory for the data to be read
                EmptyBuffer = (char *)malloc(iSize + 1);

                if(EmptyBuffer == NULL)
                {
                    printf("ERROR : Unable to allocate memory\n");
                }
                else
                {
                    // Perform the read operation
                    iRet = read_file(atoi(Command[1]), EmptyBuffer, iSize);

                    if(iRet == ERR_INVALID_PARAMETER)
                    {
                        free(EmptyBuffer);
                        EmptyBuffer = NULL;
                        printf("Error : Invalid Parameter\n");
                    }
                    else if(iRet == ERR_FILE_NOT_EXISTS)
                    {
                        free(EmptyBuffer);
                        EmptyBuffer = NULL;
                        printf("Error : File not exists\n");
                    }
                    else if(iRet == ERR_PERMISSION_DENIED)
                    {
                        free(EmptyBuffer);
                        EmptyBuffer = NULL;
                        printf("Error : Permission denied\n");
                    }
                    else if(iRet == 0)
                    {
                        free(EmptyBuffer);
                        EmptyBuffer = NULL;

                        printf("End of the file reached\n");
                    }
                    else
                    {
                        EmptyBuffer[iRet] = '\0';

                        printf("Read operation is successful\n");
                        printf("Data from file is :\n");
                        printf("%s\n", EmptyBuffer);

                        free(EmptyBuffer);
                        EmptyBuffer = NULL;
                    }
                }
            }

            // open filename mode
            else if(strcmp(Command[0], "open") == 0)
            {
                // Validate the mode input
                if(IsNumber(Command[2]) == 0)
                {
                    printf("Error : Invalid Number\n");
                    continue;
                }

                // Open the requested file
                iRet = openFile(Command[1], atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Unable to open the file\n");
                    printf("Reason : Invalid parameter\n");
                    printf("Please use the man page for correct command usage\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Unable to open the file\n");
                    printf("Reason : Permission denied\n");
                    printf("The requested access mode is not permitted for this file\n");
                }
                else if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to open the file\n");
                    printf("Reason : Maximum number of files are already open\n");
                    printf("Please close some opened files\n");
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : Unable to open the file\n");
                    printf("Reason : File does not exist\n");
                    printf("Please use ls command to check available files\n");
                }
                else
                {
                    printf("File opened successfully with fd : %d\n",iRet);
                }
            }

            // rename oldName newName
            else if(strcmp(Command[0], "rename") == 0)
            {
                // Rename the requested file
                iRet = renameFile(Command[1],Command[2]);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameter\n");
                }
                else if(iRet == ERR_FILE_ALREADY_EXISTS)
                {
                    printf("Error : File already exists\n");
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : File not exists\n");
                }
                else if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File renamed successfully\n");
                }
            }

            // truncate filename newSize
            else if(strcmp(Command[0], "truncate") == 0)
            {
                // Validate the size input
                if(IsNumber(Command[2]) == 0)
                {
                    printf("Error : Invalid Number\n");
                    continue;
                }

                // Perform the truncate operation
                iRet = truncateFile(Command[1],atoi(Command[2]));

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File truncated successfully\n");
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : File does not exist\n");
                }
                else if(iRet == ERR_INVALID_SIZE)
                {
                    printf("Error : Invalid size\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Write permission denied\n");
                }
            }

            else
            {
                printf("Command not found\n");
                printf("Please refer help option to get more information\n");
                printf("Please refer manual page of command using man\n");
            }
        }

        // Process commands that require four arguments
        else if(iCount == 5)
        {
            // lseek fd offset position offsetType
            if(strcmp(Command[0], "lseek") == 0)
            {
                // Validate numeric arguments
                if((IsNumber(Command[1]) == 0) || (IsNumber(Command[2]) == 0))
                {
                    printf("Error : Invalid Number\n");
                    continue;
                }

                // Validate the file descriptor
                if(atoi(Command[1]) < 3 || atoi(Command[1]) >= MAXOPENFILES)
                {
                    printf("Error : Invalid File Descriptor\n");
                    continue;
                }

                int whence = -1;
                int offsetType = -1;

                // Convert the whence argument into its corresponding constant
                if(strcmp(Command[3], "start") == 0)
                {
                    whence = START;
                }
                else if(strcmp(Command[3], "current") == 0)
                {
                    whence = CURRENT;
                }
                else if(strcmp(Command[3], "end") == 0)
                {
                    whence = END;
                }
                else
                {
                    printf("Error : Invalid whence\n");
                }

                // Convert the offset type into its corresponding constant
                if(strcmp(Command[4], "read") == 0)
                {
                    offsetType = READ;
                }
                else if(strcmp(Command[4], "write") == 0)
                {
                    offsetType = WRITE;
                }
                else
                {
                    printf("Error : Invalid offset type\n");
                }

                // Execute lseek only when both arguments are valid
                if(whence != -1 && offsetType != -1)
                {
                    iRet = lseekFile(atoi(Command[1]), atoi(Command[2]), whence, offsetType);

                    if(iRet == ERR_INVALID_PARAMETER)
                    {
                        printf("Error : Invalid parameter\n");
                    }
                    else if(iRet == ERR_FILE_NOT_EXISTS)
                    {
                        printf("Error : File is not opened\n");
                    }
                    else
                    {
                        printf("File offset changed successfully\n");
                        printf("New Offset : %d\n", iRet);
                    }
                }
            }
        }

        // Handle commands with invalid syntax or argument count
        else
        {
            printf("Command not found\n");
            printf("Please refer help option to get more information\n");
            printf("Please refer manual page of command using man\n");
        }
    }

    return 0;
}