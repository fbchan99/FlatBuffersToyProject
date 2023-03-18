//
//  reflection_utils.hpp
//  FlatBuffersTest
//
//  Created by fb on 3/16/23.
//

#ifndef reflection_utils_hpp
#define reflection_utils_hpp

#include "flatbuffers/flatbuffers.h"
#include "property_tree_generated.h"
#include <string>

namespace reflection_utils {
    void parse_property_tree_buffer(const std::vector<uint8_t> &schema_data, const std::vector<uint8_t> &buffer);
}
#endif /* reflection_utils_hpp */
