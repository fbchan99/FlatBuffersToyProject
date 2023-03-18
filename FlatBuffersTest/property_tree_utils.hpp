//
//  property_tree_utils.hpp
//  FlatBuffersTest
//
//  Created by fb on 3/16/23.
//

#ifndef property_tree_utils_hpp
#define property_tree_utils_hpp

#include "flatbuffers/flatbuffers.h"
#include "property_tree_generated.h"
#include <string>

namespace property_tree_utils {
    flatbuffers::DetachedBuffer create_property_tree();

    void read_property_tree(const uint8_t *buffer);

    flatbuffers::Offset<Property> clone_property(flatbuffers::FlatBufferBuilder &builder, const Property *property);

    flatbuffers::DetachedBuffer update_property_tree(const uint8_t *buffer, const std::string &property_name, const PropertyValue &new_value);

}

#endif /* property_tree_utils_hpp */
