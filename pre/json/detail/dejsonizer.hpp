#ifndef PRE_JSON_DETAIL_DEJSONIZER_HPP
#define PRE_JSON_DETAIL_DEJSONIZER_HPP

#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/type_index.hpp>

#include <boost/mpl/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>

#include <pre/fusion/for_each_member.hpp>

#include <nlohmann/json.hpp>

#include <pre/json/detail/sfinae_enabler.hpp>
#include <pre/enums/to_underlying.hpp>

#include <pre/variant/for_each_type.hpp>

namespace pre { namespace json { namespace detail {

  struct dejsonizer {

    dejsonizer(const nlohmann::json& json_object) : 
      _json_object(json_object) {}

    template<class T> 
    void operator()(const char* name, T& value) const {

      if (_json_object.find(name) != std::end(_json_object)) {
        dejsonizer dejsonizer(_json_object.at(name));
        dejsonizer(value);
      } else {
        throw std::runtime_error(
          "Missing key " + std::string(name) + " in JSON Object : " + _json_object.dump());
      }
    }

    template<class T> 
    void operator()(const char* name, boost::optional<T>& value) const {
      // boost::optional doesn't need to be in the json object
      if (_json_object.find(name) != std::end(_json_object)) {
        dejsonizer dejsonizer(_json_object.at(name));
        dejsonizer(value);
      }
    }

    template<class T>
    void operator()(boost::optional<T>& value) const {
      // boost::optional doesn't need to be in the json object
      if (!_json_object.empty()) {
        dejsonizer dejsonizer(_json_object);

        T optional_value;
        dejsonizer(optional_value);
        value = optional_value;
      }
    }

    template<class T, 
      enable_if_is_adapted_struct_t<T>* = nullptr>
    void operator()(T& value) const {
      if (_json_object.is_object()) {
        pre::fusion::for_each_member(value, *this);
      } else {
        throw std::runtime_error("The JSON Object " + _json_object.dump() + " isn't an object that we can map to an internal type");
      }
    }

    template<typename TVariant>
    struct variant_checker {
      
      variant_checker(const nlohmann::json& json_object, TVariant& value) : _json_object(json_object), value(value) {
      };

      template< typename U > void operator()(U&& x) {
        if (!successed) {
            auto struct_it = _json_object.find("struct");
            
            if ( (struct_it != _json_object.end()) && 
                (*struct_it == boost::typeindex::type_id<U>().pretty_name()) ) {

              dejsonizer dejsonizer(_json_object);
              U test_value;
              dejsonizer(test_value);
              successed = true;
              value = test_value;
            }
        }
      }

      private:
        const nlohmann::json& _json_object;
        TVariant &value;
        bool successed = false;
    };

    template<class T, 
      enable_if_is_variant_t<T>* = nullptr>
    void operator()(T& value) const {
      pre::variant::for_each_type( value, variant_checker<T>(_json_object, value) );
    }

    template<class T,
      typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
    void operator()(T& value) const {
      // XXX : How could we safe range check enum assignement ?
      value = static_cast<T>(_json_object.get<typename std::underlying_type<T>::type>());
    }

    template<class T,
      enable_if_is_chrono_duration_t<T>* = nullptr>
    void operator()(T& value) const {
      value = T{_json_object.get<typename T::rep>()};
    }

    template<class T, 
      enable_if_is_directly_serializable_t<T>* = nullptr>
    void operator()(T& value) const {
      //TODO: Better diagnostic, current exception is : std::exception::what: type must be number, but is string, we should reoutput the json object in this case. 
      value = _json_object.get<T>();
    }

    template<class T, 
      enable_if_is_container_t<T>* = nullptr>
    void operator()(T& value) const {
      if (_json_object.is_array()) {

        value.clear(); //XXX: Needed to clear if already somehow full ?
        // XXX: Not supported by all containers : value.reserve(array.size());
        for (auto entry_json : _json_object) { 
          typename T::value_type entry_deser;
          dejsonizer dejsonizer(entry_json);
          dejsonizer(entry_deser);
          value.push_back(entry_deser);
        }

      } else {
        throw std::runtime_error("Expected " + _json_object.dump() + " to be a json array.");
      }

    }

    template<class T,
      enable_if_is_associative_container_t<T>* = nullptr>
    void operator()(T& value) const {
      if (_json_object.is_object()) {

        value.clear();

        for (nlohmann::json::const_iterator it = _json_object.begin(); it != _json_object.end(); ++it) {
          typename T::mapped_type entry_deser;
          dejsonizer dejsonizer(it.value());
          dejsonizer(entry_deser);
          value[it.key()] = entry_deser;
        }

      } else {
        throw std::runtime_error("Expected " + _json_object.dump() + " to be a json object.");
      }
    }
      
    private:
      const nlohmann::json& _json_object; // XXX: Invert to be the same as jsonizer
    
  };

}}}

#endif
