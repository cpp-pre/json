#ifndef PRE_JSON_MAPPING_HPP
#define PRE_JSON_MAPPING_HPP

#include <nlohmann/json.hpp>
#include <nlohmann/thirdparty/hedley/hedley_undef.hpp>

namespace pre { 
    namespace json {


        /**
         * @brief "Rername" a json key in the JSON object
         * 
         * @param jdoc 
         * @param psource source key name
         * @param pdest destination key name
         */
        inline void remap_property(nlohmann::json& jdoc, std::string psource, std::string pdest) {
            jdoc[pdest] = jdoc[psource];
            jdoc.erase(psource);
        }

        inline void remap_property(nlohmann::json& jdoc, 
            nlohmann::json::json_pointer psource, 
            nlohmann::json::json_pointer pdest
        ) {
            // get reference to parent of JSON pointer ptr
            const auto original_psource = psource;
            const auto last_path = psource.back();
            psource.pop_back();
            auto& parent = jdoc.at(psource);

            // remove child
            if (parent.is_object())
            {
                auto it = parent.find(last_path);
                if (it != parent.end())
                {
                    jdoc[pdest] = jdoc[original_psource];
                    parent.erase(it);
                }
                else
                {
                    throw std::runtime_error("Could not find json key at: " + original_psource.to_string());
                }
            }
            else if (parent.is_array())
            {
                auto ix_deletion = std::stoi(last_path);
                auto value = jdoc[original_psource]; // copy source before deleting it

                if(ix_deletion >= parent.size()) {
                    throw std::runtime_error("Index out of range when accessing array at: " + original_psource.to_string());
                }

                parent.erase(ix_deletion);   // no need to range check manually, erase() does it already
                jdoc[pdest] = value;
                
            }
        }
    }
}

#endif