#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct File{
    unsigned int id;
    char name[63];
    unsigned int size;
    char data[1024];
    struct Directory *parentDirectory;
};
struct Directory{
    char name[63];
    struct File* files[32];// 2048/64==32
    struct Directory* directories[32];
    unsigned int nf;
    unsigned int nd;
    char path[2048];
};
void overwrite_to_file(struct File* file, const char* str){
    strcpy(file->data,str);
    file->size=strlen(file->data);
}
void append_to_file(struct File* file, const char* str){
    strcat(file->data,str);
    file->size=strlen(file->data);
}
void printp_file(struct File* file){
    char path[2048]="";
    //if parent is not root, add parent directory to path
    if (file->parentDirectory->path!="")
        strcat(path,file->parentDirectory->path);
    strcat(path,file->parentDirectory->name);
    char name_with_slash[64]="/";
    strcat(name_with_slash,file->name);
    strcat(path,name_with_slash);
    puts(path);
}
void add_file(struct File* file, struct Directory* dir){
    dir->nf++;
    dir->files[dir->nf-1]=file;
    //save parent directory address
    file->parentDirectory=dir;
}
void add_subdir(struct Directory* parent, struct Directory* child){
    parent->nd++;
    parent->directories[parent->nd-1]=child;
    //child directory path ./parent.name
    char path[2048]="/";
    strcpy(path,parent->path);
    strcat(path,"/");
    strcat(path,parent->name);
    strcpy(child->path,path);
}
int main(void){
    unsigned id_counter=0;
    struct File* files[3];
    struct Directory root={.name="",.directories={},.files={},.nd=0,.nf=0,.path=""};
    struct Directory home={.name="home",.directories={},.files={},.nd=0,.nf=0,};//path=""
    struct Directory bin={.name="bin",.directories={},.files={},.nd=0,.nf=0,};
    add_subdir(&root,&home);
    add_subdir(&root,&bin);

    struct File bash={.name="bash", .id=id_counter++, .data=""};
    files[0]=&bash;
    add_file(&bash,&bin);

    struct File ex3_1c={.name="ex3_1c", .id=id_counter++, .data=""};
    files[1]=&ex3_1c;
    add_file(&ex3_1c,&home);
    struct File ex3_2c={.name="ex3_2c", .id=id_counter++, .data=""};
    files[2]=&ex3_2c;
    add_file(&ex3_2c,&home);
    overwrite_to_file(&ex3_1c,"int printf(const char * format, ...)");
    overwrite_to_file(&ex3_2c,"This is a comment in C language");
    
    append_to_file(&bash,"Bourne Again Shell!!");
    append_to_file(&ex3_2c,"int main(){printf(”Hello World!”)}");

    for (int i=0;i<3;i++)
        printp_file(files[i]);
}