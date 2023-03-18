//
//  property_tree_utils.cpp
//  FlatBuffersTest
//
//  Created by fb on 3/16/23.
//

#include "property_tree_utils.hpp"
#include <iostream>

using namespace flatbuffers;
namespace property_tree_utils {
    flatbuffers::DetachedBuffer create_property_tree() {
        flatbuffers::FlatBufferBuilder builder;

        auto name = builder.CreateString("property_name");

        PropertyValueBuilder value_builder(builder);
        value_builder.add_value_type(PropertyType::PropertyType_Int);
        value_builder.add_int_value(318);
        auto value = value_builder.Finish();

        std::vector<flatbuffers::Offset<Property>> properties;

        PropertyBuilder property_builder(builder);
        property_builder.add_name(name);
        property_builder.add_property_type(PropertyType::PropertyType_Int);
        property_builder.add_value(value);
        auto property = property_builder.Finish();
        properties.push_back(property);

        PropertyTreeBuilder property_tree_builder(builder);
        property_tree_builder.add_properties(builder.CreateVector(properties));
        auto property_tree = property_tree_builder.Finish();

        builder.Finish(property_tree);
        return builder.Release();
    }

    void read_property_tree(const uint8_t *buffer) {
        auto property_tree = GetPropertyTree(buffer);
        std::cout << "Number of properties: " << property_tree->properties()->size() << std::endl;
    }

    flatbuffers::Offset<Property> clone_property(flatbuffers::FlatBufferBuilder &builder, const Property *property) {
      auto name = builder.CreateString(property->name()->str());
      auto string_value__ = property->value()->string_value() ? builder.CreateString(property->value()->string_value()) : 0;
      auto value = CreatePropertyValue(builder, property->value()->value_type(), string_value__, property->value()->int_value(), property->value()->float_value(), property->value()->bool_value());
      std::vector<flatbuffers::Offset<Property>> sub_properties_offsets;
      for (auto it = property->sub_properties()->begin(); it != property->sub_properties()->end(); ++it) {
          sub_properties_offsets.push_back(clone_property(builder, *it));
      }
      auto sub_properties_vector = builder.CreateVector(sub_properties_offsets);
      return CreateProperty(builder, name, property->property_type(), value, sub_properties_vector);
    }

    flatbuffers::DetachedBuffer update_property_tree(const uint8_t *buffer, const std::string &property_name, const PropertyValue &new_value) {
      auto old_property_tree = GetPropertyTree(buffer);
      auto old_properties = old_property_tree->properties();

      flatbuffers::FlatBufferBuilder builder;

      std::vector<flatbuffers::Offset<Property>> new_properties;
      new_properties.reserve(old_properties->size());

        for (const auto old_property : *old_properties) {
        if (old_property->name()->str() == property_name) {
          // Create an updated property with the new value
            auto updated_property = clone_property(builder, old_property);
          new_properties.push_back(updated_property);
        } else {
          // Clone the existing property
            auto cloned_property = clone_property(builder, old_property);
          new_properties.push_back(cloned_property);
        }
      }

      auto properties_vector = builder.CreateVector(new_properties);
      auto new_property_tree = CreatePropertyTree(builder, properties_vector);
      builder.Finish(new_property_tree);

      return builder.Release();
    }
}

