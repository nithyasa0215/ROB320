#include "robot/robot.h"
#include "util.h"
#include <signal.h>


#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>


int main() {
    // TODO: Set up signal handler for SIGINT
    /*struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);*/
    signal(SIGINT, signal_handler);


    // TODO: Create FIFO for receiving teleop commands. The name of this FIFO
    //       should be "/tmp/teleop_cmd" (Hint: use 0666 as the mode in mkfifo.
    //       This means that all users can read and write but cannot execute 
    //       the file/folder. 
    //       See chmod: https://man7.org/linux/man-pages/man2/chmod.2.html)

    const char *fifo_name = "/tmp/teleop_cmd";
    if (mkfifo(fifo_name, 0666) == -1) {
        perror("mkfifo");
        exit(1);
    }

    // TODO: Open FIFO to get file descriptor for reading teleop commands    int fd = open(fifo_name, O_RDONLY);
    int fd = open(fifo_name, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // TODO: Convert keys to drive commands
    convert_keys_to_drive_commands(fd, STDOUT_FILENO);


    // TODO: Close the FIFO
    close(fd);

    // TODO: Unlink the FIFO
    unlink(fifo_name);

    return 0; 
    
}
