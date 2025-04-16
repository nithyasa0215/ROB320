#include "robot/robot.h"
#include "util.h"
#include <signal.h>
#include <ctype.h>


int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <use_serial> <start_dir> <speed> <delay_ms>\n", argv[0]);
        exit(1);
    }

    int use_serial, delay_ms;
    char start_dir;
    float speed;
    // TODO: Parse command line arguments.
    // Hint: Use `atoi` to convert a string to an integer
    // Hint: argv is an array of strings, where argv[0] is the program name
    //       and argv[1] is the first argument

    // intialize the variables that were decalred before the hint

    use_serial = atoi(argv[1]);
    start_dir = toupper(argv[2][0]);
    speed = atof(argv[3]);
    // this will give up the delay????
    delay_ms = atoi(argv[4]);

    // check to seee what direction the robot is going
    if (!(start_dir == 'N' || start_dir == 'S' || start_dir == 'E' || start_dir == 'W')) {
        // if it is not an error make sure to exit
        exit(1);
    }

    if (speed < 0) {
        // if it is not an error make sure to exit
        exit(1);
    }

    // TODO: Set up signal handler for SIGINT
    signal(SIGINT, signal_handler);

    int fd[2];
    // TODO: Create pipe
    if (pipe(fd) == -1) {
        //perror("pipe failed");
        exit(1);
    }

   /* if(write(fd[1], &start_dir, 1) == -1) {
        exit(1);
    }*/

    pid_t pid;
    // TODO: Fork process

    pid = fork();
    //if (pid < 0) {
       // exit(1);
        // this would be an error 
     //} else 
     if (pid == 0) {
        // Child process

        // TODO: Close unused end of pipe
        close(fd[1]);

        // TODO: Initialize the robot
       /* if (use_serial) {
            if (init_mbot(use_serial) == -1) {
                close(fd[0]);
                exit(1);
            }
        }*/

        init_mbot(use_serial);

        // TODO: Relay drive commands from the pipe to the robot
        relay_drive_commands(fd[0]);

       /* char direction;
        while (read(fd[0], &direction, 1) == 1 && (sigint_flag == 0)) {
            drive_cmd_t cmd = {0};

            // set the speed based on the direcion that it is going

            if (direction == 'N') {
                cmd.vx = speed;
            } else if (direction == 'S') {
                cmd.vx = -speed;
            } else if (direction == 'E') {
                cmd.vy = -speed;
            } else if (direction == 'W') {
                cmd.vy = speed;
            }

            if (use_serial) {
                drive(&cmd);
            } else {
                printf("Driving: vx=%.2f, vy=%.2f, wz=%.2f\n", cmd.vx, cmd.vy, cmd.wz);
            }

            usleep(delay_ms * 1000);

            drive_cmd_t stop = {0};
            if (use_serial) {
                drive(&stop);
            } else {
                printf("STOP\n");
            }
        }*/


        // TODO: Deinitialize the robot
        deinit_mbot();

        // TODO: Close file descriptor for the used end of the pipe

        close(fd[0]);
       //exit(0);

    } else {
        // Parent process

        // TODO: Close unused end of pipe

        close(fd[0]);

        // TODO: Send drive commands to the pipe

        // make the commands based on the different directions 
        /*char square[4];
        if (start_dir == 'N') {
            square[0] = 'N'; square[1] = 'E'; square[2] = 'S'; square[3] = 'W';
        } else if (start_dir == 'E') {
            square[0] = 'E'; square[1] = 'S'; square[2] = 'W'; square[3] = 'N';
        } else if (start_dir == 'S') {
            square[0] = 'S'; square[1] = 'W'; square[2] = 'N'; square[3] = 'E';
        } else {
            square[0] = 'W'; square[1] = 'N'; square[2] = 'E'; square[3] = 'S';
        }

        // make sure to write it all 
        int idex = 0;
        while (sigint_flag == 0) {
            char dir = square[idex % 4];
            if (write(fd[1], &dir, 1) == -1) {
                break;
            }
            usleep(delay_ms * 1000);
            idex++;
        }*/

        drive_square(fd[1], start_dir, speed, delay_ms);

        // TODO: Close file descriptor for the used end of the pipe
        close(fd[1]);
        wait(NULL);
        
    }

    return 0;
}
