#include "protos.h"

int main(int argc,char *argv[])
{
    if(argc<2){
        return printf("Usage: [missing file.txt] %s\n",argv[0]), -1;
    }
    char *file_name= argv[1];
    struct simplexe *s = read_data(file_name);
    maximisation(s);
    free(s);

    return 0;
}