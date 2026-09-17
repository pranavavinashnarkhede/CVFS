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

    for(i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        if(newn == NULL)
        {
            // Deallocate already allocated inode nodes
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
    int iRet = 0 ;

    strcpy(bootobj.Information , "Booting process of Marvellous CVFS is completed");

    printf("%s\n",bootobj.Information);
    
    InitialiseUAREA();

    InitialiseSuperBlock();

    iRet = CreateDILB();

    if(iRet == ERR_INSUFFICIENT_SPACE)
    {
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
    printf("---------------------------------------------------------------------\n"); }


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
    PINODE temp = head ;
    bool bFlag = false;

    while(temp != NULL)
    {
        if(strcmp(temp->FileName ,name) == 0)
        {
            bFlag = true;
            break;
        }

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
                    int permission                  // file permission
                )
{

    PINODE temp = head ;
    int i = 0 ;

    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES ;
    }
    
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

    if(IsFileExists(name) == true)
    {
        return ERR_FILE_ALREADY_EXISTS;
    }

    if(strlen(name) >= 20)
    {
        return ERR_INVALID_PARAMETER ;
    }

    // if control here means every thing is ok
    // search for empty inode

    while(temp != NULL)
    {
        if(temp->FileType == 0)
        {
            break;
        }

        temp = temp->next ;
    }

    // temp contains free inode address

    // Rare case

    if(temp == NULL)
    {
        return ERR_NO_INODES;
    }

    // search empty UFDT entry 
    for(i = 3 ; i < MAXOPENFILES ; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN ;
    }

    // allocate memeory for file table

    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // check memory allocated or not
    if(uareaobj.UFDT[i] == NULL)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // initialize file table
    uareaobj.UFDT[i]->ReadOffset = 0 ;
    uareaobj.UFDT[i]->WriteOffset = 0 ;
    uareaobj.UFDT[i]->Mode = permission ;
    
    // connect File Table with the Inode

    uareaobj.UFDT[i]->ptrinode = temp ;

    // initialize all members of inode
    strcpy(temp->FileName, name);

    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE ;

    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0 ;
    
    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE ;

    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1 ;

    uareaobj.UFDT[i]->ptrinode->Permission = permission ; 

    // allocate the memory for files data (Data Block)

    uareaobj.UFDT[i]->ptrinode->Buffer = (char*)malloc(MAXFILESIZE);

    if(uareaobj.UFDT[i]->ptrinode->Buffer == NULL) // jr memory allocate zali nasel tr all characteristics default value la initialize kra.
    {
        free(uareaobj.UFDT[i]);
        uareaobj.UFDT[i] = NULL;

        strcpy(temp->FileName, "\0");
        temp->FileSize = 0;
        temp->ActualFileSize = 0;
        temp->FileType = 0;
        temp->ReferenceCount = 0;
        temp->Permission = 0;
        temp->Buffer = NULL ;

        return ERR_INSUFFICIENT_SPACE;
    }

    superobj.FreeInodes--;

    return i ;              // returning fd (UFDT index)
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
    PINODE temp = head ;

    printf("---------------------------------------------------------------------\n");
    printf("----------------- Marvellous CVFS Files Information ------------------\n");
    printf("---------------------------------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%s\n",temp->FileName);
        }
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
    PINODE temp = head ;
    int iCount = 0 ;
    int file_permission = 0 ;

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

    while(temp != NULL)
    {
        if(strlen(temp->FileName) == 0)
        {
            temp = temp->next ;
            continue; 
        }
        if(temp->FileType == REGULARFILE)
        {
            strcpy(filetype , "Regular");
        }
        else
        {
            strcpy(filetype , "Special");
        }
        
        file_permission = temp->Permission ;

        if((file_permission & READ) == READ)
        {
            permission[0] = 'r';
        }
        else
        {
            permission[0] = '-';
        }
        
        if((file_permission & WRITE) == WRITE)
        {
            permission[1] = 'w';
        }
        else
        {
            permission[1] = '-';
        }

        if((file_permission & EXECUTE) == EXECUTE)
        {
            permission[2] = 'x';
        }
        else
        {
            permission[2] = '-';
        }

        permission[3] = '\0';


        printf("%-20s %-10d %-10d %-12s %-12s\n",
       temp->FileName,
       temp->InodeNumber,
       temp->ActualFileSize,
       filetype,
       permission);

       iCount++;

       temp = temp->next ;
    }
    
    printf("---------------------------------------------------------------------\n");
    printf("Total Files : %d\n" , iCount);
    printf("Free Inodes : %d\n",superobj.FreeInodes);
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
                char name[]                 // name of file
            )
{
    PINODE temp = NULL ;
    int Permission = 0 ;
    int Type = 0 ;

    if(IsFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    temp = head ;

    while(temp != NULL)
    {
        if(strcmp(temp->FileName , name) == 0)
        {
            printf("---------------------------------------------------------------------\n");
            printf("------------------ Statistical Information of File ------------------\n");
            printf("---------------------------------------------------------------------\n");

            printf("File Name : %s\n",temp->FileName);

            printf("Inode Number : %d\n",temp->InodeNumber);
            
            printf("File Size : %d\n",temp->FileSize);
            
            printf("Actual File Size : %d\n",temp->ActualFileSize);

            printf("Reference Count : %d\n",temp->ReferenceCount);

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

            Type = temp->FileType ;

            if(Type == REGULARFILE)
            {
                printf("File Type : Regular File\n");
            }
            else if(Type == SPECIALFILE)
            {
                printf("File Type : Special File\n");
            }

            printf("---------------------------------------------------------------------\n");
            
            break;
        }

        temp = temp->next ;
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
                    char name[]         // name of file
                )
{

    PINODE temp = NULL ;
    
    if(IsFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXISTS ;
    }

    temp = head;

    // Travel the inode list to find filename

    while(temp != NULL)
    {
        if(strcmp(temp->FileName , name) == 0)
        {
            if(temp->ReferenceCount == 0)
            {
                // FileName is a character array, so we write '\0' at index 0 to make the string empty.
                
                temp->FileName[0] = '\0';

                temp->FileSize = 0 ;
                
                temp->ActualFileSize = 0 ;

                temp->FileType = 0 ;

                temp->Permission = 0 ;
                
                free(temp->Buffer);

                temp->Buffer = NULL;

                superobj.FreeInodes++;              // increase the free inode count
            
                break;      // IMPORTANT
            }
            else
            {
                return ERR_PERMISSION_DENIED ;
            }
        }// End of if

        temp = temp->next ;

    } // End of for

    return EXECUTE_SUCCESS;

}// End of unlink_file 


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

    if(fd < 0 || fd >= MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    temp = uareaobj.UFDT[fd]->ptrinode;

    // Check WRITE permission
    if((temp->Permission & WRITE) != WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Check available space
    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Write data into buffer
    strncpy(
        temp->Buffer + uareaobj.UFDT[fd]->WriteOffset,
        arr,
        size
    );

    // Move write offset
    uareaobj.UFDT[fd]->WriteOffset =
        uareaobj.UFDT[fd]->WriteOffset + size;

    // Calculate new end position
    iRet = uareaobj.UFDT[fd]->WriteOffset;

    // Update actual file size only if new end is greater
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

    // Validate file descriptor
    if(fd < 0 || fd >= MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check whether file is opened
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Validate size
    if(size < 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    temp = uareaobj.UFDT[fd]->ptrinode;

    // Check READ permission
    if((temp->Permission & READ) != READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Check whether read offset is at or beyond EOF
    if(uareaobj.UFDT[fd]->ReadOffset >= temp->ActualFileSize)
    {
        return 0;
    }

    // Calculate remaining data
    remainingData = temp->ActualFileSize - uareaobj.UFDT[fd]->ReadOffset;

    // If requested size is greater than remaining data,
    // read only the available data
    if(size > remainingData)
    {
        size = remainingData;
    }

    // Copy data from file buffer
    strncpy(arr, temp->Buffer + uareaobj.UFDT[fd]->ReadOffset, size );

    // Add string terminator
    arr[size] = '\0';

    // Move read offset
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
int openFile(char name[] , int mode)
{
    PINODE temp = NULL;
    int i = 0;

    // Check for invalid mode
    if(mode < READ || mode > (READ + WRITE))
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check whether file exists
    if(IsFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Search for the inode of the file
    temp = head;

    while(temp != NULL)
    {
        if(strcmp(temp->FileName, name) == 0)
        {
            break;
        }

        temp = temp->next;
    }

    // Check whether requested mode is permitted
    if((temp->Permission & mode) != mode)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Search for empty UFDT entry
    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    // No free UFDT entry
    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate memory for FileTable
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Check memory allocation
    if(uareaobj.UFDT[i] == NULL)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Initialize FileTable
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = mode;

    // Connect FileTable with Inode
    uareaobj.UFDT[i]->ptrinode = temp;

    // Increase reference count
    temp->ReferenceCount++;

    // Return file descriptor
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
    if(fd < 0 || fd >= MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER ;
    }

    if(uareaobj.UFDT[fd] != NULL)
    {
        uareaobj.UFDT[fd]->ptrinode->ReferenceCount--; 
        free(uareaobj.UFDT[fd]);

        uareaobj.UFDT[fd] = NULL ;
    }
    else
    {
        return ERR_FILE_NOT_EXISTS ;
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

    // Free FileTable structures
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            free(uareaobj.UFDT[i]);
            uareaobj.UFDT[i] = NULL;
        }
    }

    // Free inode buffers and inode structures
    temp = head;

    while(temp != NULL)
    {
        next = temp->next;

        if(temp->Buffer != NULL)
        {
            free(temp->Buffer);
            temp->Buffer = NULL;
        }

        free(temp);
        temp = next;
    }

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

    if(fileDescriptor < 0 || fileDescriptor >= MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fileDescriptor] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    if(whence < START || whence > END)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(offsetType != READ && offsetType != WRITE)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Get current offset
    if(offsetType == READ)
    {
        currentOffset = uareaobj.UFDT[fileDescriptor]->ReadOffset;
    }
    else
    {
        currentOffset = uareaobj.UFDT[fileDescriptor]->WriteOffset;
    }

    // Calculate new offset
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

    // Offset cannot be negative
    if(newOffset < 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Update required offset
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
    PINODE temp = NULL ;

    if((strlen(oldName) <= 0 || strlen(oldName) >= 20) || (strlen(newName) <= 0 || strlen(newName) >= 20))
    {
        return ERR_INVALID_PARAMETER ;
    }

    temp = head ;

    while(temp != NULL)
    {
        if(strcmp(temp->FileName , newName) == 0)
        {
            break;
        }

        temp = temp->next ;
    }

    if(temp != NULL)
    {
        return ERR_FILE_ALREADY_EXISTS ;
    }

    temp = head ;

    while(temp != NULL)
    {
        if(strcmp(temp->FileName , oldName) == 0)
        {
            break;
        }

        temp = temp->next;
    }

    if(temp == NULL)
    {
        return ERR_FILE_NOT_EXISTS ;
    }

    strcpy(temp->FileName , newName);

    return EXECUTE_SUCCESS ;
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

    // Validate size
    if(Size < 0)
    {
        return ERR_INVALID_SIZE;
    }

    // Find the inode
    while(temp != NULL)
    {
        if(strcmp(temp->FileName, FileName) == 0)
        {
            break;
        }

        temp = temp->next;
    }

    // File does not exist
    if(temp == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Cannot increase file size
    if(Size > temp->ActualFileSize)
    {
        return ERR_INVALID_SIZE;
    }

    // Check WRITE permission
    if((temp->Permission & WRITE) != WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Set new file size
    temp->ActualFileSize = Size;

    // Adjust offsets of all opened file descriptors
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

    if(str[0] == '\0')
        return 0;

    while(str[i] != '\0')
    {
        if(str[i] < '0' || str[i] > '9')
            return 0;

        i++;
    }

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
    if(name[0] == '\0')
        return false;

    if(strlen(name) >= 20)
        return false;

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Entry Point Function of the CVFS Project
//
////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // input command
    char str[80] = {'\0'};
    
    // Tokenised command
    char Command[5][20] = {{'\0'}};
    
    // used to store user input data for write function
    char InputBufferr[MAXFILESIZE] = {'\0'};

    int iRet = 0  ;                         // to store tokens
    int iCount = 0 ;
    int iSize = 0 ;
    
    char *EmptyBuffer = NULL ;              // for reading the data

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
    

    // Infinite Listening Shell
    while(1)
    {
        strcpy(str , "");

        printf("\nMarvellous CVFS : > ");
        fgets(str , sizeof(str) , stdin);

        // %19s read only 19 characters and add \0 at the end automatically
        iCount = sscanf(str , "%19s %19s %19s %19s %19s",Command[0] , Command[1] , Command[2] , Command[3] , Command[4] );   

        if(iCount == 1)
        {
            // Marvellous CVFS : > exit
           if(strcmp(Command[0],"exit") == 0)
            {
                printf("Marvellous CVFS : Deallocating all resources...\n");

                DeallocateResources();

                printf("Marvellous CVFS : All resources deallocated successfully\n");

                break;
            }
            // Marvellous CVFS : > help
           else if(strcmp(Command[0] , "help") == 0)
           {
                DisplayHelp();
           }

            // Marvellous CVFS : > clear
           else if(strcmp(Command[0] , "clear") == 0)
           {
                 #ifdef _WIN32                   
                    system("cls");

                #else 
                    system("clear");

                #endif
           }
           // Marvellous CVFS : > ls
           else if(strcmp(Command[0] , "ls") == 0)
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
        else if(iCount == 2)
        {
            // Marvellous CVFS : > man open
            if(strcmp(Command[0] , "man") == 0)
            {
                ManPageDisplay(Command[1]);
            }
            // Marvellous CVFS : > ls -a
            else if((strcmp(Command[0] , "ls") == 0) && strcmp(Command[1] , "-a") == 0)
            {
                LsFile_All();
            }
            // Marvellous CVFS : > stat Ganesh.txt
            else if(strcmp(Command[0] , "stat") == 0)
            {
                iRet = stat_file(Command[1]);

                if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : File not exist\n");
                }
            }
            // Marvellous CVFS : > unlink Ganesh.txt
            else if(strcmp(Command[0] , "unlink") == 0)
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
            // Marvellous CVFS : > write 1         1 -> fd
            else if(strcmp(Command[0] , "write") == 0)
            {
                if(IsNumber(Command[1]) == 0)
                {
                    printf("Error : Invalid Number \n");
                    continue;
                }

                if(atoi(Command[1]) < 3 || atoi(Command[1]) >= MAXOPENFILES)
                {
                    printf("Error : Invalid File Descriptor\n");
                    continue;
                }

                printf("Enter the data that you want to write the file \n");
                fgets(InputBufferr , MAXFILESIZE , stdin);      // the fgets add \n at the end 

                iSize = strlen(InputBufferr);

                if(iSize > 0 && InputBufferr[iSize - 1] == '\n')
                {
                    InputBufferr[iSize - 1] = '\0';
                    iSize--;
                }

                if(iSize == 0)
                {
                    printf("Error : Data cannot be empty\n");
                    continue;
                }

                iRet = write_file(atoi(Command[1]) , InputBufferr , iSize);

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
                    printf("Error : There is no permsiion to write the data \n");
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
                if(IsNumber(Command[1]) == 0)
                {
                    printf("Error : Invalid Number \n");
                    continue; 
                }

                if(atoi(Command[1]) < 3 || atoi(Command[1]) >= MAXOPENFILES)
                {
                    printf("Error : Invalid File Descriptor\n");
                    continue;
                }

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
        
        else if(iCount == 3)
        {
            // Marvellous CVFS : > creat Ganesh.txt 3
            if(strcmp(Command[0] , "creat") == 0)
            {
                if(IsNumber(Command[2]) == 0)
                {
                    printf("Error : Invalid Number \n");
                    continue;
                }

                if(IsValidFileName(Command[1]) == false)
                {
                    printf("Error : Invalid file name\n");
                    continue;
                }

                iRet = CreateFile(Command[1] , atoi(Command[2]));               

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
                    printf("Please use ls command to checks names of all files\n");
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
            // Marvellous CVFS : > read 3 10
            else if(strcmp(Command[0], "read") == 0)
            {
                if((IsNumber(Command[1]) == 0) || (IsNumber(Command[2]) == 0))
                {
                    printf("Error : Invalid Number \n");
                    continue;
                }

                if(atoi(Command[1]) < 3 || atoi(Command[1]) >= MAXOPENFILES)
                {
                    printf("Error : Invalid File Descriptor\n");
                    continue;
                }


                iSize = atoi(Command[2]);

                if(iSize <= 0 || iSize > MAXFILESIZE)
                {
                    printf("Error : Invalid file size\n");
                    continue;
                }

                EmptyBuffer = (char *)malloc(iSize + 1);

                if(EmptyBuffer == NULL)
                {
                    printf("ERROR : Unable to allocate memory\n");
                }
                else
                {
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
                        printf("Data from file is : \n");
                        printf("%s\n", EmptyBuffer);

                        free(EmptyBuffer);
                        EmptyBuffer = NULL;
                    }
                }
            }
            // open Demo.txt 3
            else if(strcmp(Command[0] , "open") == 0)
            {
                if(IsNumber(Command[2]) == 0)
                {
                    printf("Error : Invalid Number \n");
                    continue;
                }

                //                filename      mode
                iRet = openFile(Command[1] , atoi(Command[2]));

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
            // rename oldname newname
            else if(strcmp(Command[0], "rename") == 0)
            {
                iRet = renameFile(Command[1], Command[2]);

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
            // truncate filename newsize
            else if(strcmp(Command[0], "truncate") == 0)
            {
                if(IsNumber(Command[2]) == 0)
                {
                    printf("Error : Invalid Number \n");
                    continue;
                }

                iRet = truncateFile(Command[1], atoi(Command[2]));

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
        else if(iCount == 5)
        {   // lseek fd offset position offset_type 
            if(strcmp(Command[0], "lseek") == 0)
            {
                if((IsNumber(Command[1])) == 0 || (IsNumber(Command[2])) == 0)
                {
                    printf("Error : Invalid Number \n");
                    continue;
                }

                if(atoi(Command[1]) < 3 || atoi(Command[1]) >= MAXOPENFILES)
                {
                    printf("Error : Invalid File Descriptor\n");
                    continue;
                }

                int whence = -1;
                int offsetType = -1;

                // Check whence
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

                // Check offset type
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

                // Call lseek only when both parameters are valid
                if(whence != -1 && offsetType != -1)
                {
                    iRet = lseekFile(
                                atoi(Command[1]),
                                atoi(Command[2]),
                                whence,
                                offsetType
                            );

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
        
        else
        {
            printf("Command not found\n");
            printf("Please refer help option to get more information\n");
            printf("Please refer manual page of command using man\n");
        }
    } // end of while

    return 0 ;
} // end of main
