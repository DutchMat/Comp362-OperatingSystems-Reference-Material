#include "simfs.h"

#include <stdio.h>

#define SIMFS_FILE_NAME "simfsFile.dta"
//Name: Charles Pelton
//Lab: Project01Task02
//Date: 12/13/2018
int main()
{
//    srand(time(NULL)); // uncomment to get true random values in get_context()

    if (simfsCreateFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);
	

    if (simfsMountFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);

    // TODO: implement thorough testing of all the functionality
	printf("Stagnant Creation and use of the File System(TEST):\n");
	simfsUmountFileSystem("Start.dta");

	if (simfsCreateFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);
	

    	if (simfsMountFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);

	for(int x = 0; x < 2; x++){
	if (simfsCreateFile("/home/bin", FOLDER_CONTENT_TYPE) != SIMFS_NO_ERROR)
	printf("/home/bin folder created unsuccessfully, already exists\n");
	else
	printf("/home/bin folder created successfully\n");
	}

	for(int x = 0; x < 2; x++){
	if (simfsDeleteFile("/home/bin") != SIMFS_NO_ERROR)
	printf("/home/bin folder deleted unsuccessfully, does not exist\n");
	else
	printf("/home/bin folder deleted successfully\n");
	}

	if (simfsCreateFile("/home/bin", FOLDER_CONTENT_TYPE) != SIMFS_NO_ERROR)
	printf("/home/bin folder created unsuccessfully, already exists\n");
	else
	printf("/home/bin folder created successfully\n");

	if (simfsCreateFile("/home/start", FILE_CONTENT_TYPE) != SIMFS_NO_ERROR)
	printf("/home/start FILE created unsuccessfully, already exists\n");
	else
	printf("/home/start FILE created successfully\n");

	if (simfsCreateFile("/home/desktop", DATA_CONTENT_TYPE) != SIMFS_NO_ERROR)
	printf("/home/desktop 'data' created unsuccessfully, already exists\n");
	else
	printf("/home/desktop 'data' created successfully\n");

	printf("Remount!\n");

	simfsUmountFileSystem("Test Once");
	simfsMountFileSystem("Test Once");
	printf("Remount once!\n");
	simfsUmountFileSystem("Test Twice");
	simfsMountFileSystem("Test Twice");
	printf("Remount twice!\n");
	
	printf("Should be no data loss!\n");

	if (simfsCreateFile("/home/bin", FOLDER_CONTENT_TYPE) != SIMFS_NO_ERROR)
	printf("/home/bin folder created unsuccessfully, already exists\n");
	else
	printf("/home/bin folder created successfully\n");

	for(int x = 0; x < 2; x++){
	if (simfsDeleteFile("/home/bin") != SIMFS_NO_ERROR)
	printf("/home/bin folder deleted unsuccessfully, does not exist\n");
	else
	printf("/home/bin folder deleted successfully\n");
	}
	
	for(int x = 0; x < 2; x++){
	if (simfsDeleteFile("/home/start") != SIMFS_NO_ERROR)
	printf("/home/start FILE deleted unsuccessfully, does not exist\n");
	else
	printf("/home/start FILE deleted successfully\n");
	}

	if (simfsCreateFile("/home/bin", FOLDER_CONTENT_TYPE) != SIMFS_NO_ERROR)
	printf("/home/bin folder created unsuccessfully, already exists\n");
	else
	printf("/home/bin folder created successfully\n");

	if (simfsCreateFile("/home/start", FILE_CONTENT_TYPE) != SIMFS_NO_ERROR)
	printf("/home/start FILE created unsuccessfully, already exists\n");
	else
	printf("/home/start FILE created successfully\n");

	if (simfsCreateFile("/home/desktop", DATA_CONTENT_TYPE) != SIMFS_NO_ERROR)
	printf("/home/desktop 'data' created unsuccessfully, already exists\n");
	else
	printf("/home/desktop 'data' created successfully\n");
	SIMFS_FILE_HANDLE_TYPE fileHandle;
	
	if(simfsOpenFile("/home/start", &fileHandle) == SIMFS_NO_ERROR);
	printf("/home/start has been opened\n");

	if(simfsOpenFile("/home/start", &fileHandle) == SIMFS_DUPLICATE_ERROR);
	printf("/home/start Already Exists/Open\n");

	if(simfsOpenFile("/home/desktop", &fileHandle) == SIMFS_NO_ERROR);
	printf("/home/desktop has been opened\n");
	
	if(simfsOpenFile("/home/vortex", &fileHandle) == SIMFS_NOT_FOUND_ERROR);
	printf("/home/vortex does not exist\n");
	
	printf("Remount!\n");

	simfsUmountFileSystem("Test Once");
	simfsMountFileSystem("Test Once");
	printf("Remount once!\n");
	simfsUmountFileSystem("Test Twice");
	simfsMountFileSystem("Test Twice");
	printf("Remount twice!\n");

	if(simfsOpenFile("/home/start", &fileHandle) == SIMFS_NO_ERROR);
	printf("/home/start has been opened\n");

	if(simfsOpenFile("/home/start", &fileHandle) == SIMFS_DUPLICATE_ERROR);
	printf("/home/start Already Exists/Open\n");

	if(simfsCloseFile(fileHandle) == SIMFS_NO_ERROR);
	printf("/home/start has been closed\n");

	if(simfsOpenFile("/home/desktop", &fileHandle) == SIMFS_NO_ERROR);
	printf("/home/desktop has been opened\n");
	
	if(simfsCloseFile(fileHandle) == SIMFS_NO_ERROR);
	printf("/home/desktop has been closed\n");

	if(simfsCloseFile(fileHandle) == SIMFS_NOT_FOUND_ERROR);
	printf("/home/desktop not has been found\n");
	
	if(simfsOpenFile("/home/desktop", &fileHandle) == SIMFS_NO_ERROR);
	printf("/home/desktop has been opened\n");
	
	if(simfsOpenFile("/home/start", &fileHandle) == SIMFS_NO_ERROR);
	printf("/home/start has been opened\n");
	
	if(simfsOpenFile("/home/vortex", &fileHandle) == SIMFS_NOT_FOUND_ERROR);
	printf("/home/vortex does not exist\n");

	if (simfsCreateFile("/home/bin", FOLDER_CONTENT_TYPE) != SIMFS_NO_ERROR)
	printf("/home/bin folder created unsuccessfully, already exists\n");
	else
	printf("/home/bin folder created successfully\n");

	for(int x = 0; x < 2; x++){
	if (simfsDeleteFile("/home/bin") != SIMFS_NO_ERROR)
	printf("/home/bin folder deleted unsuccessfully, does not exist\n");
	else
	printf("/home/bin folder deleted successfully\n");
	}
	
	for(int x = 0; x < 2; x++){
	if (simfsDeleteFile("/home/start") != SIMFS_NO_ERROR)
	printf("/home/start FILE deleted unsuccessfully, does not exist\n");
	else
	printf("/home/start FILE deleted successfully\n");
	}

	if (simfsCreateFile("/home/bin", FOLDER_CONTENT_TYPE) != SIMFS_NO_ERROR)
	printf("/home/bin folder created unsuccessfully, already exists\n");
	else
	printf("/home/bin folder created successfully\n");

	if (simfsCreateFile("/home/start", FILE_CONTENT_TYPE) != SIMFS_NO_ERROR)
	printf("/home/start FILE created unsuccessfully, already exists\n");
	else
	printf("/home/start FILE created successfully\n");

	if (simfsCreateFile("/home/desktop", DATA_CONTENT_TYPE) != SIMFS_NO_ERROR)
	printf("/home/desktop 'data' created unsuccessfully, already exists\n");
	else
	printf("/home/desktop 'data' created successfully\n");
	

	printf("\nSample Simulated Code:\n");

    // the following is just some sample code for simulating user and process identifiers that are
    // needed in the simfs functions
    int count = 10;
    char *content;
    struct fuse_context *context;
    for (int i = 0; i < count; i++)
    {
        context = simfs_debug_get_context();
        printf("user ID = %02i, process ID = %02i, group ID = %02i, umask = %04o\n",
               context->uid, context->pid, context->gid, context->umask);
        content = simfsGenerateContent(i * 10);
        printf("content = \"%s\"\nhash(content) = %ld\n", content, hash((unsigned char *) content));
    }

    if (simfsUmountFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);

    if (simfsMountFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);

    if (simfsUmountFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);

    unsigned char testBitVector[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    simfsFlipBit(testBitVector, 44);
    printf("Found free block at %d\n", simfsFindFreeBlock(testBitVector));
    simfsClearBit(testBitVector, 33);
    printf("Found free block at %d\n", simfsFindFreeBlock(testBitVector));
    simfsSetBit(testBitVector, 33);
    printf("Found free block at %d\n", simfsFindFreeBlock(testBitVector));

    return EXIT_SUCCESS;
}
