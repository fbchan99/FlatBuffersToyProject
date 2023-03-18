//
//  main.cpp
//  FlatBuffersTest
//
//  Created by fb on 3/16/23.
//
#include "flatbuffers/util.h"
#include "property_tree_utils.hpp"
#include "socket_utils.hpp"
#include "reflection_utils.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace flatbuffers;
using namespace property_tree_utils;
using namespace socket_utils;
using namespace reflection_utils;

int main(int argc, const char * argv[]) {
    // Load the binary schema
    std::string schema_path = "property_tree.bfbs";
    std::vector<uint8_t> schema_data;
    std::string schema_data_str(schema_data.begin(), schema_data.end());
    if (!flatbuffers::LoadFile(schema_path.c_str(), true, &schema_data_str)) {
        std::cerr << "Error loading binary schema file: " << schema_path << std::endl;
        return 1;
    }

    // Create a sample property tree buffer
    flatbuffers::DetachedBuffer buffer = property_tree_utils::create_property_tree();

    // Send the property tree buffer over TCP socket
    flatbuffers::FlatBufferBuilder builder;
    socket_utils::send_property_tree("127.0.0.1", 8080, builder);

    // Receive the property tree buffer over TCP socket
    std::vector<uint8_t> received_buffer = socket_utils::receive_property_tree("127.0.0.1", 8080);
    if (received_buffer.empty()) {
        std::cerr << "Error receiving property tree buffer" << std::endl;
        return 1;
    }

    // Read the property tree
    property_tree_utils::read_property_tree(received_buffer.data());

    // Use reflection_utils to parse the received property tree buffer
    reflection_utils::parse_property_tree_buffer(schema_data, received_buffer);

    return 0;
}
