#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include <stddef.h>

CHAR16* config;
char* kernelbinary;

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

void loadConfig(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
	EFI_FILE* configfile;
	if((configfile = loadFile(NULL,L"aboot.cfg",ImageHandle,SystemTable)) == NULL){
		Print(L"config Not found\r\n");
	}
	else{
		Print(L"config loaded\r\n");
	}
	UINTN fileinfosize = sizeof(EFI_FILE_INFO);
	EFI_FILE_INFO* fileinfo;
	SystemTable->BootServices->AllocatePool(EfiLoaderData,fileinfosize,(void**)&fileinfo);
	configfile->GetInfo(configfile,&gEfiFileInfoGuid,&fileinfosize,(void**)&fileinfo);
	char* binary;
	SystemTable->BootServices->AllocatePool(EfiLoaderData,fileinfo->FileSize,(void**)&binary);
	
	configfile->Read(configfile,&fileinfo->FileSize,(void*)binary);

	SystemTable->BootServices->AllocatePool(EfiLoaderData,(fileinfo->FileSize * 2) + 1,(void**)&config);
	
	for (size_t i = 0; i < fileinfo->FileSize; i++)
	{
		config[i] = (CHAR16)binary[i];
	}
	config[fileinfo->FileSize + 1] = 0; // add NULL terminator character
}
void loadKernel(CHAR16* kernelloc, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
	EFI_FILE* configfile;
	if((configfile = loadFile(NULL,kernelloc,ImageHandle,SystemTable)) == NULL){
		Print(L"kernel Not found\r\n");
	}
	else{
		Print(L"kernel loaded\r\n");
	}
	UINTN fileinfosize = sizeof(EFI_FILE_INFO);
	EFI_FILE_INFO* fileinfo;
	SystemTable->BootServices->AllocatePool(EfiLoaderData,fileinfosize,(void**)&fileinfo);
	configfile->GetInfo(configfile,&gEfiFileInfoGuid,&fileinfosize,(void**)&fileinfo);
	
	SystemTable->BootServices->AllocatePool(EfiLoaderData,fileinfo->FileSize,(void**)&kernelbinary);
	
	configfile->Read(configfile,&fileinfo->FileSize,(void*)kernelbinary);

}
EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle,SystemTable);
	loadConfig(ImageHandle,SystemTable);
	Print(L"Config:\n'\n");Print(config);Print(L"\n'\n");
	loadKernel(L"stage2.bin",ImageHandle,SystemTable);

	int (*kernelstart)() = ((__attribute__((sysv_abi)) int (*)()) kernelbinary);
	UINTN errorcode = kernelstart();
	Print(L"%d", errorcode);
	while(1){}

	return EFI_SUCCESS; // Exit the UEFI application(never)
}
//gonna
//give
//you
//up
