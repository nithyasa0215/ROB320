#include "robot/robot.h"
#include "util.h"
#include <signal.h>
#include <ctype.h>

// the same set up as the previous files

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <use_serial>\n", argv[0]);
        exit(1);
    }

    // TODO: Parse command line arguments.
    // Hint: Use `atoi` to convert a string to an integer
    // Hint: argv is an array of strings, where argv[0] is the program name
    //       and argv[1] is the first argument
    int use_serial = atoi(argv[1]);

    // TODO: Set up signal handler for SIGINT
    signal(SIGINT, signal_handler);

    // TODO: Initialize the robot. Use the parsed argument to determine if 
    //       serial should be used
    if (init_mbot(use_serial) == -1) {
        fprintf(stderr, "Failed to initialize MBot.\n");
        return 1;
    }

    // TODO: Relay drive commands from STDIN to the robot
    relay_drive_commands(STDIN_FILENO);

    // TODO: Deinitialize the robot
    deinit_mbot();

    return 0;
}
