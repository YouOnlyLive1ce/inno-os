#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

int fd;
bool pressed_e = false;
bool pressed_x = false;
bool pressed_p = false;
bool pressed_c = false;
bool pressed_a = false;
char globalArray['z' - 'a' + 1];
//custom shortcut - cp. No need in additional flags
const char *KEYBOARD_PATH = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";

void printEvent(struct input_event *input_event) {
    const char *eventType;
    if (input_event->type == EV_KEY) {
        if (input_event->value == 0) {
            eventType = "RELEASED";
            if (input_event->code == KEY_E)
                pressed_e = false;
            if (input_event->code == KEY_P)
                pressed_p = false;
            if (input_event->code == KEY_C)
                pressed_c = false;
            if (input_event->code == KEY_X)
                pressed_x = false;
            if (input_event->code == KEY_A)
                pressed_a = false;
            if (input_event->code == KEY_P)
                pressed_p = false;
        } else if (input_event->value == 1) {
            eventType = "PRESSED";
            if (input_event->code == KEY_E)
                pressed_e = true;
            if (input_event->code == KEY_P)
                pressed_p = true;
            if (input_event->code == KEY_C)
                pressed_c = true;
            if (input_event->code == KEY_X)
                pressed_x = true;
            if (input_event->code == KEY_A)
                pressed_a = true;
            if (input_event->code == KEY_P)
                pressed_p = true;
        } else if (input_event->value == 2) {
            eventType = "REPEATED";
            if (input_event->code == KEY_E)
                pressed_e = true;
            if (input_event->code == KEY_P)
                pressed_p = true;
            if (input_event->code == KEY_C)
                pressed_c = true;
            if (input_event->code == KEY_X)
                pressed_x = true;
            if (input_event->code == KEY_A)
                pressed_a = true;
            if (input_event->code == KEY_P)
                pressed_p = true;
        } else
            return;

        FILE *outputFile = fopen("ex1.txt", "a");
        fprintf(outputFile, "%s 0x%04X (%d)\n", eventType, input_event->code, input_event->code);
        // ex bind
        if (pressed_e && pressed_x)
            exit(EXIT_SUCCESS);
        // pe bind
        if (pressed_p && pressed_e) {
            fprintf(outputFile, "I passed the Exam!\n");
            pressed_p = false;
            pressed_e=false;
        }
        // cap bind
        if (pressed_c && pressed_a && pressed_p) {
            fprintf(outputFile, "Get some cappuccino!\n");
            pressed_c = false;
            pressed_a = false;
            pressed_p=false;
        }
        // Funny thing. You may press c,p and not get result: order of whole sequence is important
        // When you press any other letters between pressed letters, it affect flags
        if (pressed_c && pressed_p){
            fprintf(outputFile, "You shouldn't have done that. The consequences of your action may be terrible for me\n");
            pressed_c = false;
            pressed_p=false;
        }
        fclose(outputFile);
    }
}

int main() {
    // Global variable fd got permission to read typed chars
    fd = open(KEYBOARD_PATH, O_RDONLY);
    struct input_event input_event;
    while ("Tatarstan") {
        ssize_t bytesRead = read(fd, &input_event, sizeof(input_event));
        if (bytesRead == sizeof(input_event)) 
            if (input_event.type == EV_KEY) 
                printEvent(&input_event);
    }
    //Close, so that thing stop spy on me
    close(fd);
    return 0;
}