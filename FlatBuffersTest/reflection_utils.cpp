//
//  reflection_utils.cpp
//  FlatBuffersTest
//
//  Created by fb on 3/16/23.
//

#include "reflection_utils.hpp"
#include <iostream>
#include <vector>
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/reflection.h>
#include <flatbuffers/idl.h>
#include <flatbuffers/util.h>

using namespace flatbuffers;

namespace reflection_utils {
    void parse_property_tree_buffer(const std::vector<uint8_t> &schema_data, const std::vector<uint8_t> &buffer) {
        // Load the schema
        flatbuffers::Verifier verifier(schema_data.data(), schema_data.size());
        if (!reflection::VerifySchemaBuffer(verifier)) {
            std::cerr << "Failed to load schema" << std::endl;
            return;
        }

        auto schema = flatbuffers::GetRoot<reflection::Schema>(schema_data.data());

        // Get the root of the buffer
        auto property_tree = flatbuffers::GetRoot<PropertyTree>(buffer.data());

        // Access the key and value fields
        const reflection::Object *property_tree_obj = schema->objects()->Get(0);
        auto key_field = property_tree_obj->fields()->LookupByKey("key");
        auto value_field = property_tree_obj->fields()->LookupByKey("value");

        // Read the key and value
        auto property_table = reinterpret_cast<const flatbuffers::Table *>(property_tree);
        auto key_ptr = property_table->GetPointer<const flatbuffers::String *>(key_field->offset());
        auto value_ptr = property_table->GetPointer<const flatbuffers::String *>(value_field->offset());

        std::string key = key_ptr ? key_ptr->str() : "";
        std::string value = value_ptr ? value_ptr->str() : "";

        // Print the key and value
        std::cout << "Key: " << key << ", Value: " << value << std::endl;
    }
}
