
void print(char* str);

char _start(){
    print("Simple hello world!!");
    return 123;
}
void print(char* str)
{
    static unsigned short* VideoMemory = (unsigned short*)0xb8000;

    for(int i = 0; str[i] != '\0'; ++i)
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
}
