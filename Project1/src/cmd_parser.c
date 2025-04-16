#include "robot/robot.h"
#include "util.h"
#include <signal.h>
#include <ctype.h>

int main() {
    // TODO: Set up signal handler for SIGINT
    signal(SIGINT, signal_handler);

    // TODO: Parse commands from STDIN and forward them to STDOUT
    parse_and_forward_commands(STDIN_FILENO, STDOUT_FILENO);

    return 0;
}
