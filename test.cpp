#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8080);

    if (bind(listen_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Bind failed\n";
        close(listen_fd);
        return 1;
    }

    if (listen(listen_fd, 10) < 0) {
        std::cerr << "Listen failed\n";
        close(listen_fd);
        return 1;
    }

    struct pollfd fds[1];
    fds[0].fd = listen_fd;
    fds[0].events = POLLIN;

    std::cout << "Waiting for connections on port 8080...\n";
    while (true) {
        int ret = poll(fds, 1, 5000); // 5 second timeout
        if (ret < 0) {
            std::cerr << "poll() error\n";
            break;
        } else if (ret == 0) {
            std::cout << "No incoming connections. Waiting...\n";
            continue;
        }

        if (fds[0].revents & POLLIN) {
            sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int client_fd = accept(listen_fd, (sockaddr*)&client_addr, &client_len);
            if (client_fd >= 0) {
                std::cout << "Accepted a new connection!\n";
                // Handle client_fd as needed
                close(client_fd);
            }
        }
    }

    close(listen_fd);
    return 0;
}

// c++ -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address test.cpp
