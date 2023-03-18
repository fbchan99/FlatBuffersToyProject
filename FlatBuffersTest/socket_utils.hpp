//
//  socket_utils.hpp
//  FlatBuffersTest
//
//  Created by fb on 3/16/23.
//

#ifndef socket_utils_hpp
#define socket_utils_hpp

#include "flatbuffers/flatbuffers.h"
#include "property_tree_generated.h"
#include <string>

namespace socket_utils {
    void send_property_tree(const std::string &ip, int port, const flatbuffers::FlatBufferBuilder &builder);

    std::vector<uint8_t> receive_property_tree(const std::string &ip, int port);
}

#endif /* socket_utils_hpp */
