/********************************************
 * This server.cpp file includes all logic
 * behide client part of the network:
 *          1. creating a socket
 *          2. binding to IP and port
 *          3. sending messages
 *          4. receiving echoed messages
 *          5. closing the socket
*********************************************/

#include "shared.h"

void run_client() {
    // create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Can't create a socket";
        return;
    }

    // create a hint structure for the server we're connecting with
    int port = 54000;
    std::string ip_address = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ip_address.c_str(), &hint.sin_addr);

    // connect to the server
    int connect_result = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connect_result == -1) {
        std::cerr << "Can't connect to server";
        return;
    }

    // while loop to send and receive data
    char buff[4096];
    std::string user_input;
    do {
        // get user input
        std::cout << "> ";
        std::getline(std::cin, user_input);

        if (user_input.size() > 0) {
            // send the text
            int send_result = send(sock, user_input.c_str(), user_input.size() + 1, 0);
            if (send_result == -1) {
                std::cerr << "Could not send to server";
                continue;
            }

            // wait for response
            memset(buff, 0, 4096);
            int bytes_recv = recv(sock, buff, 4096, 0);
            if (bytes_recv > 0) {
                std::cout << "SERVER> " << std::string(buff, 0, bytes_recv) << std::endl;
            }
        }
    } while(true);

    // close the socket
    close(sock);
}