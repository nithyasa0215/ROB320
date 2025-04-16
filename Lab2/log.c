#include "messages.h"
#include "util.h"

int ctrl_c_pressed = 0;
void sigint_handler(int signum) {
    if (signum == SIGINT) {
        ctrl_c_pressed = 1;
        printf("Exiting...\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <discovery_server_ip> <name> <port>\n", argv[0]);
        return 1;
    }

    const char* discovery_server_ip = argv[1];
    const char* name = argv[2];
    const unsigned short port = atoi(argv[3]);
    char* public_ip = get_public_ip();
    if (public_ip == NULL) {
        public_ip = strdup("127.0.0.1");
    }

    // TODO: Initialize signal handler for SIGINT
    signal(SIGINT, sigint_handler);

    int server_fd;
    // TODO: Create a socket file descriptor for the server
    //       use AF_INET for ipv4 and SOCK_STREAM for TCP
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // TODO: Set the socket to non-blocking mode
    int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in address;
    // TODO: Initialize the sockaddr_in struct for the server
    //       use INADDR_ANY for the address and the specified port
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    int status;
    // TODO: Bind the socket to the specified address
    status = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    if (status < 0) {
        perror("Bind failed");
        return 1;
    }

    // TODO: Listen for up to 32 incoming connections on the socket
    status = listen(server_fd, 32);
    if (status < 0) {
        perror("Listen failed");
        return 1;
    }

    int discovery_fd;
    // TODO: Create a socket file descriptor for the discovery server
    //       use AF_INET for ipv4 and SOCK_STREAM for TCP
    discovery_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (discovery_fd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // TODO: Set the socket to non-blocking mode
    flags = fcntl(discovery_fd, F_GETFL, 0);
    fcntl(discovery_fd, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in discovery_address;
    // TODO: Initialize the sockaddr_in struct for the discovery server
    memset(&discovery_address, 0, sizeof(discovery_address));
    discovery_address.sin_family = AF_INET;
    discovery_address.sin_port = htons(12345);  // Use actual port of the discovery server
    if (inet_pton(AF_INET, discovery_server_ip, &discovery_address.sin_addr) <= 0) {
        perror("Invalid address or Address not supported");
        return 1;
    }

    // TODO: Connect to the discovery server (use connect_until_success)
    if (connect_until_success(discovery_fd, (struct sockaddr *)&discovery_address, sizeof(discovery_address)) < 0) {
        perror("Failed to connect to discovery server");
        return 1;
    }

    UserMessage register_msg = {0};
    // TODO: Initialize a UserMessage struct with the register opcode
    //       and the user's port, address, and name
    register_msg.opcode = 1;  // Assuming 1 is the "register" opcode
    register_msg.port = port;
    strncpy(register_msg.address, public_ip, sizeof(register_msg.address));
    strncpy(register_msg.name, name, sizeof(register_msg.name));

    uint8_t* register_data;
    // TODO: Encode the UserMessage struct into a byte array
    register_data = encode_user_message(&register_msg);

    // TODO: Send the UserMessage to the discovery server (use send_until_success)
    if (send_until_success(discovery_fd, register_data, sizeof(UserMessage)) < 0) {
        perror("Failed to send register message");
        return 1;
    }

    // TODO: Close the connection to the discovery server
    close(discovery_fd);

    while (!ctrl_c_pressed) {

        int client_fd;
        // TODO: Accept incoming connections from clients (use accept_until_success)
        //       Set status to the return value of accept_until_success
        client_fd = accept_until_success(server_fd, NULL, NULL);
        
        // If status < 0, accept_until_success encountered an error
        if (client_fd < 0) {
            free(public_ip);
            perror("accept");
            return 1;
        } 
        // If status > 0, ctrl-c was pressed
        else if (client_fd > 0) {
            break;
        }

        uint8_t buffer[sizeof(ChatMessage)] = {0};
        // TODO: Receive a ChatMessage from the client (use recv_until_success)
        //       Set status to the return value of recv_until_success
        status = recv_until_success(client_fd, buffer, sizeof(ChatMessage));

        // If status < 0, recv_until_success encountered an error
        if (status < 0) {
            free(public_ip);
            return 1;
        } 
        // If status > 0, ctrl-c was pressed
        else if (status > 0) {
            break;
        }

        ChatMessage* chat_msg;
        // TODO: Decode the buffer into a ChatMessage
        chat_msg = decode_chat_message(buffer, sizeof(ChatMessage));

        // Print the received message
        printf("%s : \"%s\"\n", chat_msg->name, chat_msg->message);

        // TODO: Close the connection to the client
        close(client_fd);
    }

    // Reset ctrl_c_pressed
    ctrl_c_pressed = 0;

    // TODO: Create a socket file descriptor for the discovery server
    discovery_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (discovery_fd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // TODO: Set the socket to non-blocking mode
    flags = fcntl(discovery_fd, F_GETFL, 0);
    fcntl(discovery_fd, F_SETFL, flags | O_NONBLOCK);

    // TODO: Connect to the discovery server (use connect_until_success)
    if (connect_until_success(discovery_fd, (struct sockaddr *)&discovery_address, sizeof(discovery_address)) < 0) {
        perror("Failed to connect to discovery server");
        return 1;
    }
    
    UserMessage deregister_msg = {0};
    // TODO: Initialize a UserMessage struct with the deregister opcode
    //       and the user's port, address, and name
    deregister_msg.opcode = 2;  // Assuming 2 is the "deregister" opcode
    deregister_msg.port = port;
    strncpy(deregister_msg.address, public_ip, sizeof(deregister_msg.address));
    strncpy(deregister_msg.name, name, sizeof(deregister_msg.name));

    uint8_t* deregister_data;
    // TODO: Encode the UserMessage struct into a byte array
    deregister_data = encode_user_message(&deregister_msg);

    // TODO: Send the UserMessage to the discovery server (use send_until_success)
    if (send_until_success(discovery_fd, deregister_data, sizeof(UserMessage)) < 0) {
        perror("Failed to send deregister message");
        return 1;
    }

    // Free the public_ip string
    free(public_ip);

    return 0;
}