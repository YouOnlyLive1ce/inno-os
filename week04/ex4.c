#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define COMMAND_LINE_LENGTH 128
#define ARGS_LENGTH 32

void executeCommand(char *command) {
    char *command_line[ARGS_LENGTH];
    char *char_iterator;
    int i = 0;

    // Tokenize the command
    char_iterator = strtok(command, " \t\n");
    while (char_iterator != NULL) {
        command_line[i++] = char_iterator;
        char_iterator = strtok(NULL, " \t\n");
    }
    command_line[i] = NULL;

    // Check if the command should be run in the background
    int background = 0;
    if (strcmp(command_line[i - 1], "&") == 0) {
        background = 1;
        command_line[i - 1] = NULL; // Remove the "&" argument
    }

    // Fork a child process
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Execute child process
        if (execvp(command_line[0], command_line) == -1) {
            perror("Execution failed");
            exit(1);
        }
    } else {
        // if process is parent, wait for child process
        if (!background) {
            int status;
            waitpid(pid, &status, 0);
        }
    }
}

int main() {
    char command_line[COMMAND_LINE_LENGTH];
    //Endless loop
    while ("Tatarstan") {
        printf("$ ");
        if (fgets(command_line, sizeof(command_line), stdin) == NULL) {
            break; // End of input
        }
        executeCommand(command_line);
    }
    return 0;
}