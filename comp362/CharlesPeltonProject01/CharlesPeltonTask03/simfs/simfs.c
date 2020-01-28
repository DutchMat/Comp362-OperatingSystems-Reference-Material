
#include "simfs.h"

//Name: Charles Pelton
//Lab: Project01Task02
//Date: 12/13/2018

//////////////////////////////////////////////////////////////////////////
//
// allocation of the in-memory data structures
//
//////////////////////////////////////////////////////////////////////////

SIMFS_CONTEXT_TYPE *simfsContext; // all in-memory information about the system
SIMFS_VOLUME *simfsVolume;

//////////////////////////////////////////////////////////////////////////
//
// simfs function implementations
//
//////////////////////////////////////////////////////////////////////////

/*
 * Constructs in-memory directory of all files is the system.
 *
 * Starting with the file system root (pointed to from the superblock) traverses the hierarachy of directories
 * and adds en entry for each folder or file to the directory by hashing the name and adding a directory
 * entry node to the conflict resolution list for that entry. If the entry is NULL, the new node will be
 * the only element of that list.
 *
 * The function sets the current working directory to refer to the block holding the root of the volume. This will
 * be changed as the user navigates the file system hierarchy.
 *
 */

//A recursive block.
SIMFS_ERROR traverse(SIMFS_INDEX_TYPE index)
{
    short pos = 0;
    size_t size = 0;
    while(index < SIMFS_INDEX_SIZE && size < simfsVolume->block[index].content.fileDescriptor.size){
    if(simfsVolume->block[simfsVolume->block[index].content.index[pos]].type != INDEX_CONTENT_TYPE){
    unsigned long hashIndex = hash(simfsVolume->block[index].content.fileDescriptor.name);
            SIMFS_DIR_ENT *temp;
            SIMFS_DIR_ENT *block = malloc(sizeof(SIMFS_DIR_ENT));
            block->nodeReference = simfsVolume->block[index].content.index[pos];
	    temp = (simfsContext->directory[hashIndex]);
            if(temp != NULL){
            while (temp->next != NULL){
               temp = temp->next;
            }
               temp->next = block;
            }
            else
            {
                temp = block;
            }
            if(simfsVolume->block[block->nodeReference].type == FOLDER_CONTENT_TYPE)
            {
                traverse(block->nodeReference);
            }
            pos++;
            size++;
        }
        else
        {
            index = simfsVolume->block[index].content.index[pos];
            pos = 0;
        }
    }
    return SIMFS_NO_ERROR;
}

SIMFS_ERROR simfsMountFileSystem(char *simfsFileName)
{
    simfsContext = malloc(sizeof(SIMFS_CONTEXT_TYPE));
    if (simfsContext == NULL)
        return SIMFS_ALLOC_ERROR;

    simfsContext->processControlBlocks = malloc(sizeof(SIMFS_PROCESS_CONTROL_BLOCK_TYPE));
    if (simfsContext->processControlBlocks == NULL)
        return SIMFS_ALLOC_ERROR;

    simfsVolume = malloc(sizeof(SIMFS_VOLUME));
    if (simfsVolume == NULL)
        return SIMFS_ALLOC_ERROR;

    FILE *file = fopen(simfsFileName, "rb");
    if (file == NULL)
        return SIMFS_ALLOC_ERROR;

    fread(simfsVolume, 1, sizeof(SIMFS_VOLUME), file);
    fclose(file);

    simfsContext->processControlBlocks->currentWorkingDirectory = simfsVolume->superblock.attr.rootNodeIndex;
    simfsContext->processControlBlocks->numberOfOpenFiles = 0; // sets working

    return traverse(simfsVolume->superblock.attr.rootNodeIndex); //traverse volume and implements into context recursively

    // TODO: complete

}
//////////////////////////////////////////////////////////////////////////
// Initializes Block
void simfsInitializeBlock(SIMFS_INDEX_TYPE *loc, SIMFS_CONTENT_TYPE *type){
    simfsVolume->block[*loc].type = *type;
    simfsVolume->block[*loc].content.fileDescriptor.type = *type;
    simfsVolume->block[*loc].content.fileDescriptor.accessRights = umask(00000);
    simfsVolume->block[*loc].content.fileDescriptor.owner = getuid(); // arbitrarily simulated
    simfsVolume->block[*loc].content.fileDescriptor.size = 0;

    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    simfsVolume->block[*loc].content.fileDescriptor.creationTime = time.tv_sec;
    simfsVolume->block[*loc].content.fileDescriptor.lastAccessTime = time.tv_sec;
    simfsVolume->block[*loc].content.fileDescriptor.lastModificationTime = time.tv_sec;
}

/*
 * Depending on the type parameter the function creates a file or a folder in the current directory
 * of the process. If the process does not have an entry in the processControlBlock, then the root directory
 * is assumed to be its current working directory.
 *
 * Hashes the file name and check if the file with such name already exists in the in-memory directory.
 * If it is then it return SIMFS_DUPLICATE_ERROR.
 * Otherwise:
 *    - finds an available block in the storage using the in-memory bitvector and flips the bit to indicate
 *      that the block is taken
 *    - initializes a local buffer for the file descriptor block with the block type depending on the parameter type
 *      (i.e., folder or file)
 *    - creates an entry in the conflict resolution list for the corresponding in-memory directory entry
 *    - copies the local buffer to the disk block that was found to be free
 *    - copies the in-memory bitvector to the bitevector blocks on the simulated disk
 *
 *  The access rights and the the owner are taken from the context (umask and uid correspondingly).
 *
 */
SIMFS_ERROR simfsCreateFile(SIMFS_NAME_TYPE fileName, SIMFS_CONTENT_TYPE type)
{
	SIMFS_DIR_ENT *block = NULL;
	if(simfsGetFileInfo(fileName, NULL, NULL, &block) == SIMFS_NO_ERROR){ // used to find the end node
	return SIMFS_DUPLICATE_ERROR;
	}
	if(block != NULL){// new block
	block->next = calloc(1, sizeof(SIMFS_DIR_ENT));
	block = block->next;
	}
	else{ //first block
	block = calloc(1, sizeof(SIMFS_DIR_ENT));
	simfsContext->directory[hash(fileName)] = block;
	}
	block->next = NULL;
	block->nodeReference = simfsFindFreeBlock(simfsContext->bitvector);
	simfsSetBit(simfsVolume->bitvector, block->nodeReference);
	simfsSetBit(simfsContext->bitvector, block->nodeReference);
	strcpy(simfsVolume->block[block->nodeReference].content.fileDescriptor.name, fileName);
	simfsInitializeBlock(&(block->nodeReference), &type); // sets all block parameters and location
    // TODO done: implement
    return SIMFS_NO_ERROR;
}

//////////////////////////////////////////////////////////////////////////

/*
 * Deletes a file from the file system.
 *
 * Hashes the file name and check if the file is in the directory. If not, then it returns SIMFS_NOT_FOUND_ERROR.
 * Otherwise:
 *    - finds the reference to the file descriptor block
 *    - if the referenced block is a folder that is not empty, then returns SIMFS_NOT_EMPTY_ERROR.
 *    - Otherwise:
 *       - checks if the process owner can delete this file or folder; if not, it returns SIMFS_ACCESS_ERROR.
 *       - Otherwise:
 *          - frees all blocks belonging to the file by flipping the corresponding bits in the in-memory bitvector
 *          - frees the reference block by flipping the corresponding bit in the in-memory bitvector
 *          - clears the entry in the folder by removing the corresponding node in the list associated with
 *            the slot for this file
 *          - copies the in-memory bitvector to the bitvector blocks on the simulated disk
 */
SIMFS_ERROR simfsDeleteFile(SIMFS_NAME_TYPE fileName)
{
    // TODO: implement
	SIMFS_DIR_ENT *block = NULL; SIMFS_DIR_ENT *prev = NULL;
	if(simfsGetFileInfo(fileName, NULL, &block, &prev) == SIMFS_NOT_FOUND_ERROR){ // gets prev and current
	return SIMFS_NOT_FOUND_ERROR;
	}
	simfsClearBit(simfsVolume->bitvector, block->nodeReference); // clear bitvector
	simfsClearBit(simfsContext->bitvector, block->nodeReference);
	if(prev != NULL){ // links blocks
	prev->next = block->next;
	}
	else{
	simfsContext->directory[hash(fileName)] = block->next; //if no prev, set as directory
	}
	free(block); // free block
    return SIMFS_NO_ERROR;
}

//////////////////////////////////////////////////////////////////////////

/*
 * Finds the file in the in-memory directory and obtains the information about the file from the file descriptor
 * block referenced from the directory.
 *
 * If the file is not found, then it returns SIMFS_NOT_FOUND_ERROR
 */
SIMFS_ERROR simfsGetFileInfo(SIMFS_NAME_TYPE fileName, SIMFS_FILE_DESCRIPTOR_TYPE *infoBuffer, SIMFS_DIR_ENT **currentNode, SIMFS_DIR_ENT **endNode)
{
    // TODO done: implement - its awesome, handles everything by changing parameters
	SIMFS_DIR_ENT *location = simfsContext->directory[hash(fileName)]; 
	while(location != NULL){ 
	if(strcmp(fileName, simfsVolume->block[location->nodeReference].content.fileDescriptor.name) == 0){
	if(infoBuffer != NULL){
	*infoBuffer = (simfsVolume->block[location->nodeReference].content.fileDescriptor);
	}
	if(currentNode != NULL){
	*currentNode = location;
	}
	return SIMFS_NO_ERROR;
	}
	if(endNode != NULL){
	*endNode = location;
	}
	location = location->next;
	}
	return SIMFS_NOT_FOUND_ERROR;

}

//////////////////////////////////////////////////////////////////////////
	SIMFS_ERROR traverseFileTable(SIMFS_DIR_ENT **block, int **index){ // traverse file table

	SIMFS_PROCESS_CONTROL_BLOCK_TYPE *temp;
	temp = simfsContext->processControlBlocks;
		while(temp != NULL){
		if(temp->numberOfOpenFiles != 0){
		for(int x = 0; x < SIMFS_MAX_NUMBER_OF_OPEN_FILES_PER_PROCESS; x++){
		if(temp->openFileTable[x].globalEntry == NULL){
		break;}
		if(strcmp(simfsVolume->block[temp->openFileTable[x].globalEntry->fileDescriptor].content.fileDescriptor.name, simfsVolume->block[(*block)->nodeReference].content.fileDescriptor.name) == 0){
		**index = temp->openFileTable[x].globalEntry->fileDescriptor;
		return SIMFS_NO_ERROR;
		}
	}
}
		temp = temp->next;
		}
	return SIMFS_NOT_FOUND_ERROR;
	}

	SIMFS_ERROR traverseGlobal(SIMFS_DIR_ENT **block){ // traverse global
	
		for(int x = 0; x < SIMFS_MAX_NUMBER_OF_OPEN_FILES; x++){
			if(strcmp(simfsVolume->block[simfsContext->globalOpenFileTable[x].fileDescriptor].content.fileDescriptor.name, simfsVolume->block[(*block)->nodeReference].content.fileDescriptor.name) == 0){
			simfsContext->globalOpenFileTable[x].referenceCount++;
			return SIMFS_NO_ERROR;
			}
			if(strcmp(simfsVolume->block[simfsContext->globalOpenFileTable[x].fileDescriptor].content.fileDescriptor.name,"") == 0){
			simfsContext->globalOpenFileTable[x].fileDescriptor = (*block)->nodeReference;
			return SIMFS_NOT_FOUND_ERROR;
			}	
			}
	return SIMFS_ALLOC_ERROR;
	}
/*
 * Hashes the name and searches for it in the in-memory directory. If the file does not exist,
 * the SIMFS_NOT_FOUND_ERROR is returned.
 *
 * Otherwise:
 *    - checks the per-process open file table for the process, and if the file has already been opened
 *      it returns the index of the openFileTable with the entry of the file through the parameter fileHandle, and
 *      returns SIMFS_DUPLICATE_ERROR as the return value
 *
 *    - otherwise, checks if there is a global entry for the file, and if so, then:
 *       - it increases the reference count for this file
 *
 *       - otherwise, it creates an entry in the global open file table for the file copying the information
 *         from the file descriptor block referenced from the entry for this file in the directory
 *
 *       - if the process does not have its process control block in the processControlBlocks list, then
 *         a file control block for the process is created and added to the list; the current working directory
 *         is initialized to the root of the volume and the number of the open files is initialized to 0
 *
 *       - if an entry for this file does not exits in the per-process open file table, the function finds an
 *         empty slot in the table and fills it with the information including the reference to the entry for
 *         this file in the global open file table.
 *
 *       - returns the index to the new element of the per-process open file table through the parameter fileHandle
 *         and SIMFS_NO_ERROR as the return value
 *
 * If there is no free slot for the file in either the global file table or in the per-process
 * file table, or if there is any other allocation problem, then the function returns SIMFS_ALLOC_ERROR.
 *
 */
SIMFS_ERROR simfsOpenFile(SIMFS_NAME_TYPE fileName, SIMFS_FILE_HANDLE_TYPE *fileHandle)
{
    // TODO doneish: implement
	SIMFS_FILE_DESCRIPTOR_TYPE info;
	SIMFS_DIR_ENT *block = NULL; 
	if(simfsGetFileInfo(fileName, &info, &block, NULL) == SIMFS_NOT_FOUND_ERROR){ // DNE
	return SIMFS_NOT_FOUND_ERROR;
	}
	if(traverseFileTable(&block, &fileHandle) == SIMFS_NO_ERROR){ // table
	return SIMFS_DUPLICATE_ERROR;
	}
	traverseGlobal(&block);
	SIMFS_PROCESS_CONTROL_BLOCK_TYPE *temp;
	temp = simfsContext->processControlBlocks;
	while(temp->next != NULL){ // find last PCB
	temp = temp->next;
	}
	temp->next = calloc(1, sizeof(SIMFS_PROCESS_CONTROL_BLOCK_TYPE)); // set all stuff
	temp->currentWorkingDirectory = simfsVolume->superblock.attr.rootNodeIndex;
        temp->numberOfOpenFiles = 0;
	temp->openFileTable[0].globalEntry = malloc(sizeof(SIMFS_OPEN_FILE_GLOBAL_TABLE_TYPE));
	temp->openFileTable[0].globalEntry->fileDescriptor = block->nodeReference;
	temp->openFileTable[0].globalEntry->referenceCount = 0;
    return SIMFS_NO_ERROR;
}

//////////////////////////////////////////////////////////////////////////

/*
 * The function replaces content of a file with new one pointed to by the parameter writeBuffer.
 *
 * Checks if the file handle points to a valid file descriptor of an open file. If the entry is invalid
 * (e.g., if the reference to the global table is NULL, or if the entry in the global table is INVALID_CONTENT_TYPE),
 * then it returns SIMFS_NOT_FOUND_ERROR.
 *
 * Otherwise, it checks the access rights for writing. If the process owner is not allowed to write to the file,
 * then the function returns SIMFS_ACCESS_ERROR.
 *
 * Then, the functions calculates the space needed for the new content and checks if the write buffer can fit into
 * the remaining free space in the file system. If not, then the SIMFS_ALLOC_ERROR is returned.
 *
 * Otherwise, the function removes all blocks currently held by this file, and then acquires new blocks as needed
 * modifying bits in the in-memory bitvector as needed.
 *
 * It then copies the characters pointed to by the parameter writeBuffer (until '\0' but excluding it) to the
 * new blocks that belong to the file. The function copies any modified block of the in-memory bitvector to
 * the corresponding bitvector block on the disk.
 *
 * Finally, the file descriptor is modified to reflect the new size of the file, and the times of last modification
 * and access.
 *
 * The function returns SIMFS_WRITE_ERROR in response to exception not specified earlier.
 *
 */
SIMFS_ERROR simfsWriteFile(SIMFS_FILE_HANDLE_TYPE fileHandle, char *writeBuffer)
{
    // TODO: implement

    return SIMFS_NO_ERROR;
}

//////////////////////////////////////////////////////////////////////////

/*
 * The function returns the complete content of the file to the caller through the parameter readBuffer.
 *
 * Checks if the file handle points to a valid file descriptor of an open file. If the entry is invalid
 * (e.g., if the reference to the global table is NULL, or if the entry in the global table is INVALID_CONTENT_TYPE),
 * then it returns SIMFS_NOT_FOUND_ERROR.
 *
 * Otherwise, it checks the user's access right to read the file. If the process owner is not allowed to read the file,
 * then the function returns SIMFS_ACCESS_ERROR.
 *
 * Otherwise, the function allocates memory sufficient to hold the read content with an appended end of string
 * character; the pointer to newly allocated memory is passed back through the readBuffer parameter. All the content
 * of the blocks is concatenated using the allocated space, and an end of string character is appended at the end of
 * the concatenated content.
 *
 * The function returns SIMFS_READ_ERROR in response to exception not specified earlier.
 *
 */
SIMFS_ERROR simfsReadFile(SIMFS_FILE_HANDLE_TYPE fileHandle, char **readBuffer)
{
    // TODO: implement

    return SIMFS_NO_ERROR;
}

//////////////////////////////////////////////////////////////////////////

/*
 * Removes the entry for the file with the file handle provided as the parameter from the open file table
 * for this process. It decreases the number of open files for in the process control block of this process, and
 * if it becomes zero, then the process control block for this process is removed from the processControlBlocks list.
 *
 * Decreases the reference count in the global open file table, and if that number is 0, it also removes the entry
 * for this file from the global open file table.
 *
 */

SIMFS_ERROR simfsCloseFile(SIMFS_FILE_HANDLE_TYPE fileHandle)
{
    // TODO doneish: implement
	// does not use global open file table(not in this version)
	SIMFS_PROCESS_CONTROL_BLOCK_TYPE *temp;
	SIMFS_PROCESS_CONTROL_BLOCK_TYPE *prev; // finds blocks
	temp = simfsContext->processControlBlocks;
	while(temp != NULL){
		for(int x = 0; x < SIMFS_MAX_NUMBER_OF_OPEN_FILES_PER_PROCESS; x++){ // traversal
		if(temp->openFileTable[x].globalEntry == NULL)
		break;
		if(fileHandle == temp->openFileTable[x].globalEntry->fileDescriptor){
			if(temp->openFileTable[0].globalEntry->referenceCount != 0){ // counts down reference
			temp->openFileTable[0].globalEntry->referenceCount--;
			}
			else{
			prev->next = temp->next;
			free(temp); // if last reference, remove
			}
			return SIMFS_NO_ERROR;
		}
		}
	prev = temp;
	temp = temp->next;
	}
    return SIMFS_NOT_FOUND_ERROR;
}

//////////////////////////////////////////////////////////////////////////
//
// The following functions are provided only for testing without FUSE.
//
// When linked to the FUSE library, both user ID and process ID can be obtained by calling fuse_get_context().
// See: http://libfuse.github.io/doxygen/structfuse__context.html
//
//////////////////////////////////////////////////////////////////////////

/*
 * Simulates FUSE context to get values for user ID, process ID, and umask through fuse_context
 */

struct fuse_context *simfs_debug_get_context() {

    // TODO: replace its use with FUSE's fuse_get_context()

    struct fuse_context *context = malloc(sizeof(struct fuse_context));

    context->fuse = NULL;
    context->uid = (uid_t) rand()%10+1;
    context->pid = (pid_t) rand()%10+1;
    context->gid = (gid_t) rand()%10+1;
    context->private_data = NULL;
    context->umask = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; // can be changed as needed

    return context;
}

char *simfsGenerateContent(int size)
{
    size = (size <= 0 ? rand()%1000 : size); // arbitrarily chosen as an example

    char *content = malloc(size);

    int firstPrintable = ' ';
    int len = '~' - firstPrintable;

    for (int i=0; i<size-1; i++)
        *(content+i) = firstPrintable + rand()%len;

    content[size - 1] = '\0';
    return content;
}


//////////////////////////////////////////////////////////////////////////
//
// simfs function implementations
//
//////////////////////////////////////////////////////////////////////////

/*
 * Retuns a hash value within the limits of the directory.
 */
inline unsigned long hash(unsigned char *str)
{
    register unsigned long hash = 5381;
    register unsigned char c;

    while ((c = *str++) != '\0')
        hash = ((hash << 5) + hash) ^ c; /* hash * 33 + c */

    return hash % SIMFS_DIRECTORY_SIZE;
}

/*
 * Find a free block in a bit vector.
 */
inline unsigned short simfsFindFreeBlock(unsigned char *bitvector)
{
    unsigned short i = 0;
    while (bitvector[i] == 0xFF)
        i += 1;

    register unsigned char mask = 0x80;
    unsigned short j = 0;
    while (bitvector[i] & mask) {
        mask >>= 1;
        ++j;
    }
    return (i * 8) + j; // i bytes and j bits are all "1", so this formula points to the first "0"
}


/*
 * Three functions for bit manipulation.
 */
inline void simfsFlipBit(unsigned char *bitvector, unsigned short bitIndex)
{
    unsigned short blockIndex = bitIndex / 8;
    unsigned short bitShift = bitIndex % 8;

    register unsigned char mask = 0x80;
    bitvector[blockIndex] ^= (mask >> bitShift);
}

inline void simfsSetBit(unsigned char *bitvector, unsigned short bitIndex)
{
    unsigned short blockIndex = bitIndex / 8;
    unsigned short bitShift = bitIndex % 8;

    register unsigned char mask = 0x80;
    bitvector[blockIndex] |= (mask >> bitShift);}

inline void simfsClearBit(unsigned char *bitvector, unsigned short bitIndex)
{
    unsigned short blockIndex = bitIndex / 8;
    unsigned short bitShift = bitIndex % 8;

    register unsigned char mask = 0x80;
    bitvector[blockIndex] &= ~(mask >> bitShift);
}

/*
 * Allocates space for the file system and saves it to disk.
 */
SIMFS_ERROR simfsCreateFileSystem(char *simfsFileName)
{
	
    FILE *file = fopen(simfsFileName, "wb");
    if (file == NULL)
        return SIMFS_ALLOC_ERROR;
	
    simfsContext = malloc(sizeof(SIMFS_CONTEXT_TYPE));
    if (simfsContext == NULL)
        return SIMFS_ALLOC_ERROR;

    simfsContext->processControlBlocks = malloc(sizeof(SIMFS_PROCESS_CONTROL_BLOCK_TYPE));
    if (simfsContext->processControlBlocks == NULL)
        return SIMFS_ALLOC_ERROR;

    simfsVolume = malloc(sizeof(SIMFS_VOLUME));
    if (simfsVolume == NULL)
        return SIMFS_ALLOC_ERROR;
    // initialize the superblock
    SIMFS_INDEX_TYPE x = 0;
    simfsVolume->superblock.attr.rootNodeIndex = x;
    simfsVolume->superblock.attr.blockSize = SIMFS_BLOCK_SIZE;
    simfsVolume->superblock.attr.numberOfBlocks = SIMFS_NUMBER_OF_BLOCKS;
    simfsContext->processControlBlocks->currentWorkingDirectory = simfsVolume->superblock.attr.rootNodeIndex;
    simfsContext->processControlBlocks->numberOfOpenFiles = 0;
	
    // initialize the blocks holding the root folder
    // initialize the root folder
	SIMFS_CONTENT_TYPE type = FOLDER_CONTENT_TYPE;
        simfsInitializeBlock(&x, &(type));
	strcpy(simfsVolume->block[x].content.fileDescriptor.name, "/");
    // initialize the index block of the root folder

    // first, point from the root file descriptor to the index block
    simfsVolume->block[0].content.fileDescriptor.block_ref = 1;

    simfsVolume->block[1].type = INDEX_CONTENT_TYPE;

    // indicate that the blocks #0 and #1 are allocated

    // using the function to find a free block for testing purposes
    simfsFlipBit(simfsVolume->bitvector, simfsFindFreeBlock(simfsVolume->bitvector)); // should be 0
    simfsFlipBit(simfsVolume->bitvector, simfsFindFreeBlock(simfsVolume->bitvector)); // should be 1

    // sample alternative #1 - illustration of bit-wise operations
//    simfsVolume->bitvector[0] = 0;
//    simfsVolume->bitvector[0] |= 0x01 << 7; // set the first bit of the bit vector
//    simfsVolume->bitvector[0] += 0x80 >> 1; // flip the first bit of the bit vector

    // sample alternative #2 - less educational, but fastest
//     simfsVolume->bitvector[0] = 0xC0;
    // 0xC0 is 11000000 in binary (showing the root block and root's index block taken)

    fwrite(simfsVolume, 1, sizeof(SIMFS_VOLUME), file);

    fclose(file);

    return SIMFS_NO_ERROR;
}

/*
 * Loads the file system from a disk and constructs in-memory directory of all files is the system.
 *
 * Starting with the file system root (pointed to from the superblock) traverses the hierarachy of directories
 * and adds en entry for each folder or file to the directory by hashing the name and adding a directory
 * entry node to the conflict resolution list for that entry. If the entry is NULL, the new node will be
 * the only element of that list.
 *
 * The function sets the current working directory to refer to the block holding the root of the volume. This will
 * be changed as the user navigates the file system hierarchy.
 *
 */

/*
 * Saves the file system to a disk and de-allocates the memory.
 *
 * Assumes that all synchronization has been done.
 *
 */
SIMFS_ERROR simfsUmountFileSystem(char *simfsFileName)
{
    FILE *file = fopen(simfsFileName, "wb");
    if (file == NULL)
        return SIMFS_ALLOC_ERROR;

    fwrite(simfsVolume, 1, sizeof(SIMFS_VOLUME), file);
    fclose(file);

    free(simfsVolume);
    free(simfsContext->processControlBlocks);
    free(simfsContext);

    return SIMFS_NO_ERROR;
}

//////////////////////////////////////////////////////////////////////////

