#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include <stddef.h>

//typedef unsigned long long size_t;

EFI_FILE* loadFile(EFI_FILE* Dir,CHAR16* path,EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
	EFI_FILE* LoadedFile;

	EFI_LOADED_IMAGE_PROTOCOL* loadedimage;
	SystemTable->BootServices->HandleProtocol(ImageHandle,&gEfiLoadedImageProtocolGuid,(void**)&loadedimage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* filesystem;
	SystemTable->BootServices->HandleProtocol(loadedimage->DeviceHandle,&gEfiSimpleFileSystemProtocolGuid ,(void**)&filesystem);

	if(Dir == NULL)
		filesystem->OpenVolume(filesystem,&Dir);

	EFI_STATUS s = Dir->Open(Dir,&LoadedFile,path,EFI_FILE_MODE_READ,EFI_FILE_READ_ONLY);

	if (s == EFI_SUCCESS){
		return LoadedFile;
	}
	return NULL;
}

int memcmp(const void* aptr,const void* bptr,size_t n){
	const unsigned char* a = aptr,*b = bptr;
	for (size_t i = 0; i < n; i++)
	{
		if(a[i] < b[i]) return -1;
		else if(a[i] > b[i]) return 1;
	}
	return 0;
	
}

EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

	InitializeLib(ImageHandle,SystemTable);
	EFI_FILE* stage2;
	if((stage2 = loadFile(NULL,L"stage2.bin",ImageHandle,SystemTable)) == NULL){
		Print(L"Stage2 Not found\r\n");
	}
	else{
		Print(L"Stage2 loaded\r\n");
	}
	UINTN fileinfosize;
	EFI_FILE_INFO* fileinfo;
	stage2->GetInfo(stage2,&gEfiFileInfoGuid,&fileinfosize,NULL);
	SystemTable->BootServices->AllocatePool(EfiLoaderData,fileinfosize,(void**)&fileinfo);
	stage2->GetInfo(stage2,&gEfiFileInfoGuid,&fileinfosize,(void**)&fileinfo);
	
	UINTN binarysize = 255;
	char* binary;
	//Print(L"%d", fileinfo->FileSize);
	SystemTable->BootServices->AllocatePool(EfiLoaderData,binarysize,(void**)&binary);
	
	stage2->Read(stage2,&binarysize,(void*)binary);




	int (*stage2start)() = ((__attribute__((sysv_abi)) int (*)()) binary);
	UINTN errorcode = stage2start();
	Print(L"%d", errorcode);
	while(1){}

	return EFI_SUCCESS; // Exit the UEFI application(never)
}
//gonna
//give
//you
//up
