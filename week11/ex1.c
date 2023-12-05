#include <dirent.h>
#include <stdio.h>

int main() {
    DIR *dir = opendir("./lofsdisk");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) 
        printf("%s\n", entry->d_name);
    closedir(dir);
}