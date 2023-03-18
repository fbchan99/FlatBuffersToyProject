//
//  socket_utils.cpp
//  FlatBuffersTest
//
//  Created by fb on 3/16/23.
//

#include "socket_utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace flatbuffers;

namespace socket_utils {
    void send_property_tree(const std::string &ip, int port, const flatbuffers::FlatBufferBuilder &builder) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "Error creating socket" << std::endl;
            return;
        }

        sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
            std::cerr << "Invalid address" << std::endl;
            close(sockfd);
            return;
        }

        if (connect(sockfd, reinterpret_cast<sockaddr *>(&serv_addr), sizeof(serv_addr)) < 0) {
            std::cerr << "Connection failed" << std::endl;
            close(sockfd);
            return;
        }

        uint32_t buffer_size = static_cast<uint32_t>(builder.GetSize());
        uint32_t buffer_size_network_order = htonl(buffer_size);
        send(sockfd, &buffer_size_network_order, sizeof(buffer_size_network_order), 0);
        send(sockfd, reinterpret_cast<const char *>(builder.GetBufferPointer()), buffer_size, 0);

        close(sockfd);
    }

    std::vector<uint8_t> receive_property_tree(const std::string &ip, int port) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "Error creating socket" << std::endl;
            return {};
        }

        sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

        if (bind(sockfd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) < 0) {
            std::cerr << "Error binding socket" << std::endl;
            close(sockfd);
            return {};
        }

        if (listen(sockfd, 1) < 0) {
            std::cerr << "Error listening on socket" << std::endl;
            close(sockfd);
            return {};
        }

        sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int new_socket = accept(sockfd, reinterpret_cast<sockaddr *>(&client_addr), &client_addr_size);
        if (new_socket < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            close(sockfd);
            return {};
        }

        uint32_t buffer_size = 0;
        if (recv(new_socket, reinterpret_cast<char *>(&buffer_size), sizeof(buffer_size), 0) != sizeof(buffer_size)) {
            std::cerr << "Error receiving buffer size" << std::endl;
            close(new_socket);
            close(sockfd);
            return {};
        }
        buffer_size = ntohl(buffer_size);

        std::vector<uint8_t> buffer(buffer_size);
        size_t received = 0;
        while (received < buffer_size) {
            ssize_t ret = recv(new_socket, reinterpret_cast<char *>(buffer.data()) + received, buffer_size - received, 0);
            if (ret <= 0) {
                std::cerr << "Error receiving data" << std::endl;
                close(new_socket);
                close(sockfd);
                return {};
            }
            received += ret;
        }

        close(new_socket);
        close(sockfd);
        return buffer;
    }
}
