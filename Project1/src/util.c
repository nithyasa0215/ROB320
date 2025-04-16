#include "robot/robot.h"
#include "util.h"
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/time.h>

// declare functions
long long get_current_time_usec(void);
void msleep(int millis);

// Instantiate the `sigint_flag` variable
int sigint_flag = 0;


void signal_handler(int signum) {
    // TODO: Implement signal_handler function.
    //       If the signal is SIGINT, set the `sigint_flag` variable to 1

    if (signum == SIGINT) {
        sigint_flag = 1;
    }
}

void relay_drive_commands(int in_fd) {
    // TODO: Implement relay_drive_commands function.
    //       Read drive commands from `in_fd` and send them to the robot using
    //       the drive function defined in `include/robot/robot.h`. Stop when a
    //       SIGINT signal is received.

    // why is it 256?? Does my system have memort for this??
    //char buffer[512];
    drive_cmd_t cmd;
    /*float vx, vy, wz;
    char buffer[256];


    while (sigint_flag != 1) {

        ssize_t len = read_line(in_fd, buffer, sizeof(buffer));

        if (len <= 0) {
            break;
        }
        //drive(&cmd);
        if (sscanf(buffer, "%f %f %f", &vx, &vy, &wz) == 3) {
            cmd.vx = vx;
            cmd.vy = vy;
            cmd.wz = wz;
            drive(&cmd);
        }
    }
*/
while (sigint_flag != 1) {
    int readBytes = read(in_fd, &cmd, sizeof(cmd));
    if(readBytes <= 0) {
        break;
    }
    drive(&cmd);
}

    cmd.utime = 0;
    cmd.vx = 0;
    cmd.vy = 0;
    cmd.wz = 0;
    drive(&cmd);

}

ssize_t read_line(int fd, char *buffer, size_t size) {
    // TODO: Implement read_line function. Read characters from `fd` until a
    //       newline character is encountered, or `size` characters have been
    //       read. Store the characters in `buffer` and return the number of
    //       characters read. If an error occurs, return -1.
   /* char buffer;
    int index = 0;
    int bytes = read(fd, buffer, 1);

    // this will read all of the lines (the total number of lines)
    while (bytes != 0) {
        bytes = read(fd, buffer, 1);
        buffer[index] = buffer[bytes -1];
       index++;

       if(buffer[bytes-1] == '\n' ) {
            return bytes;
        }

        if(bytes == size){
            return bytes; 
        }
}
    return -1;*/

    int index = 0;
    char ch;
    ssize_t bytes;

    while (index < size - 1) {
        bytes = read(fd, &ch, 1);
        if (bytes <= 0) {
            return (index > 0) ? index : -1;
        }

        buffer[index] = ch;
        index++;

        if (ch == '\n') {
            break;
        }
    }

    buffer[index] = '\0';
    return index;
}

void parse_and_forward_commands(int in_fd, int out_fd) {
    // TODO: Implement parse_and_forward_commands function. Read a line from 
    //       `in_fd` and parse it as a drive command (Hint: see sscanf). Send 
    //       the drive command to `out_fd` and then sleep for the specified
    //       delay. Stop when a SIGINT signal is received. If an error occurs or
    //       if the command is not in the expected format, return.
    // The format of the input line is:
    // <id: string> <vx: float> <vy: float> <wz: float> <delay: int>

    char buffer[256], id[50];
    float vx, vy, wz;
    int delay;
    drive_cmd_t cmd;
    cmd.utime = 0;
    //int read = read_line(in_fd, buffer, sizeof(buffer));

    while (sigint_flag != 1) {
    int read = read_line(in_fd, buffer, sizeof(buffer));
    if(read <= 0) {
        break;
    }
            
    if (sscanf(buffer, "%s %f %f %f %d", id, &vx, &vy, &wz, &delay) != 5) {
            fprintf(stderr, "Invalid command format\n");
            continue;
    }

        cmd.vx = vx;
        cmd.vy = vy; 
        cmd.wz = wz;
        write(out_fd, &cmd, sizeof(cmd));
        usleep(delay * 1000);
        //read = read_line(in_fd, buffer, sizeof(buffer));
        }
    }


void drive_square(int fd, char start_dir, float speed, int delay_ms) {
    // TODO: Implement drive_square function. Send drive commands to the robot to
    //       drive in a square pattern going clock-wise starting in the start direction.
    //       The robot should drive at the specified speed and sleep for the 
    //       specified delay after each command.
    //       Stop when a SIGINT signal is received.
    // Hint: If the start direction is 'N', the robot should drive
    //       North, East, South, West, and then stop.
    // Hint: Consider using a state machine to implement this function.

    drive_cmd_t cmd;
    int delay = delay_ms * 1000;
    cmd.utime = 0;
    cmd.vx = 0;
    cmd.vy = 0;
    cmd.wz = 0;
   // switch command
   while(sigint_flag != 1) {
        switch (start_dir) {
          case 'N':
          // N E S W
            cmd.vx = speed;
            cmd.vy = 0;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            cmd.vx = 0;
            //cmd.vy = -1;
            cmd.vy = -speed;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            cmd.vx = -speed ;
            cmd.vy = 0;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            cmd.vx = 0;
            //cmd.vy = 1;
            cmd.vy = speed;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            break;

            case 'E':
            // E S W N
            cmd.vx = 0;
            cmd.vy = -speed;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            cmd.vx = -speed ;
            cmd.vy = 0;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            cmd.vx = 0;
            cmd.vy = speed;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            cmd.vx = speed;
            cmd.vy = 0;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            break;
            case 'S':
            // S W N E
            cmd.vx = -speed ;
            cmd.vy = 0;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            cmd.vx = 0;
            cmd.vy = speed;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            cmd.vx = speed;
            cmd.vy = 0;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            cmd.vx = 0;
            cmd.vy = -speed;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            break;
            case 'W':
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            cmd.vx = 0;
            cmd.vy = speed;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            cmd.vx = speed;
            cmd.vy = 0;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            cmd.vx = 0;
            cmd.vy = -speed;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            cmd.vx = -speed ;
            cmd.vy = 0;
            write(fd, &cmd, sizeof(cmd));
            usleep(delay);
            break;
            default:
            continue;
        }
        cmd.vx = 0;
        cmd.vy = 0;
        write(fd, &cmd, sizeof(cmd));
        break;
    }
    
}

void convert_keys_to_drive_commands(int in_fd, int out_fd) {
    // TODO: Implement convert_keys_to_drive_commands function. Read characters
    //       from `in_fd` and convert them to drive commands. Send the drive
    //       commands to `out_fd`. Stop when a SIGINT signal is received.
    //       Your linear speed should be 0.5 m/s and your angular speed should
    //       be 1.5 rad/s. 
    // The keys to drive the robot are:
    //       - 'w': forward
    //       - 'a': left
    //       - 's': backward
    //       - 'd': right
    //       - 'q': rotate left
    //       - 'e': rotate right
    //       - ' ': stop

    float vx, vy, wz;
    float Lspeed = 0.5;
    float Aspeed = 1.5; 
    drive_cmd_t cmd;
    char key;
    int readLine = 1;
    cmd.utime = 0;



    while ((sigint_flag == 0)) {

        usleep(1000);
        readLine = read(in_fd, &key, 1);

        if(readLine < 0) {
            break;
        } else if(readLine == 0) {
            continue;
        }


        switch(key){
            case 'w':
                vx = Lspeed;
                vy = 0;
                wz = 0;
                break;
            case 'a':
                vx = 0;
                vy = Lspeed;
                wz = 0;
                break;
            case 's':
                vx = -Lspeed;
                vy = 0;
                wz = 0;
                break;
            case 'd':
                vx = 0;
                vy = -Lspeed;
                wz = 0;
                break;
            case 'q':   
                vx = 0;
                vy = 0;
                wz = Aspeed;
                break;
            case 'e':
                vx = 0;
                vy = 0;
                wz = -Aspeed;
                break;
            case ' ':
              vx = 0;
              vy = 0;
              wz = 0;
              break;
              //continue;
            default:
                continue;
        }

        cmd.vx= vx;
        cmd.vy = vy;
        cmd.wz = wz;


        write(out_fd, &cmd, sizeof(cmd));
        }

    }
