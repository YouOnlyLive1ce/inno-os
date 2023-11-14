#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <signal.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include <limits.h> 
#include <time.h>

#define PATH_LENGTH 4096
#define BUFFER_LENGTH (10 * (sizeof(struct inotify_event) + PATH_LENGTH + 1))

char *path;

void print_stat_info(const char *path) {
    struct stat stat_info;
    if (stat(path, &stat_info) == 0) {
        printf("Stat info for %s:\n", path);
        printf("Size: %ld bytes\n", stat_info.st_size);
        printf("Permissions: %o\n", stat_info.st_mode & (S_IRWXO | S_IRWXG | S_IRWXU));
        printf("Last access time: %s", ctime(&stat_info.st_atime));
        printf("Last modification time: %s\n", ctime(&stat_info.st_mtime));
    }
    fflush(stdout);
}

void print_all_stat_info(const char *path) {
    DIR *direrctory = opendir(path);
    if (direrctory==NULL) {
        perror("Error opening directory");
        return;
    }
    struct dirent *directory_entry;
    while ((directory_entry = readdir(direrctory)) != NULL) {
        if (directory_entry->d_name[0] != '.') { //So that it will not output unnecessary files
            char full_path[PATH_LENGTH];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, directory_entry->d_name);
            print_stat_info(full_path);
        }
    }
    closedir(direrctory);
}

// Signal handler for handling SIGINT
void signal_handler(int signum) {
    if (signum == SIGINT) {
        printf("\nReceived SIGINT. Printing stat info before termination:\n");
        print_all_stat_info(path);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    path = argv[1];

    signal(SIGINT, signal_handler);
    printf("Printing stat info on startup:\n");
    print_all_stat_info(path);

    int inotify_fd = inotify_init();
    int watch_fd = inotify_add_watch(inotify_fd, path, IN_ALL_EVENTS);

    char buffer[BUFFER_LENGTH];
    ssize_t num_bytes;
    while (true) {
        num_bytes = read(inotify_fd, buffer, BUFFER_LENGTH);
        struct inotify_event *event = (struct inotify_event *)buffer;

        if (event->name != NULL && event->len > 0) {// Exclude events with no relevant file name
            char full_path[PATH_LENGTH];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, event->name);
            switch (event->mask) {
                case IN_CREATE:
                    printf("%s created in watched directory\n", event->name);
                    break;
                case IN_DELETE:
                    printf("%s deleted from watched directory\n\n", event->name);
                    break;
                case IN_MODIFY:
                    printf("%s was modified\n", event->name);
                    break;
                case IN_ATTRIB:
                    printf("Metadata changed\n");
                    break;
                case IN_ACCESS:
                    printf("%s was accessed\n", event->name);
                    break;
                case IN_OPEN:
                    printf("%s was opened\n", event->name);
                    break;
                default:
                    continue;
            }
            fflush(stdout);
            print_stat_info(full_path);
        }
    }

    close(inotify_fd);
    exit(EXIT_SUCCESS);
}