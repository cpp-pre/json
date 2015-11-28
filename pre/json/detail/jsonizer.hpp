#ifndef PRE_JSON_DETAIL_JSONIZER_HPP
#define PRE_JSON_DETAIL_JSONIZER_HPP

#include <boost/variant.hpp>
#include <boost/optional.hpp>

#include <boost/fusion/include/for_each.hpp>

#include <pre/fusion/for_each_member.hpp>

#include <nlohmann/json.hpp>

#include <pre/json/detail/sfinae_enabler.hpp>
#include <pre/enums/to_underlying.hpp>

//TODO: What about tuples ?

namespace pre { namespace json { namespace detail {

  struct jsonizer : public boost::static_visitor<> {

    jsonizer(nlohmann::json& json_object) 
      : boost::static_visitor<>(),  _json_object(json_object) {}

    template<class T>
    void operator()(const char* name, const T& value) const {
      nlohmann::json json_subobject;
      jsonizer subjsonizer(json_subobject);
      subjsonizer(value);

      _json_object[std::string(name)] = json_subobject;
    }

    template<class T>
    void operator()(const char* name, const boost::optional<T>& value) const {
      if (value != boost::none) {
        this->operator()(name, value.get());
      }
    }

    template<class T>
    void operator()(const boost::optional<T>& value) const {
      if (value != boost::none) {
        jsonizer subjsonizer(_json_object);
        subjsonizer(*value);
      } else {
        _json_object = nullptr;
      }
    }

    template<class T, 
      enable_if_is_adapted_struct_t<T>* = nullptr>
    void operator()(const T& value) const {
      pre::fusion::for_each_member(value, *this);
    }

    template<class T, 
      enable_if_is_other_sequence_and_not_variant_t<T>* = nullptr>
    void operator()(const T& value) const {
      boost::fusion::for_each(value, *this);
      // HMMMMM ???
    }

    template<class T, 
      enable_if_is_variant_t<T>* = nullptr>
    void operator()(const T& value) const {
      boost::apply_visitor(*this, value);
    }

    template<class T,
      typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
    void operator()(const T& value) const {
      _json_object = pre::enums::to_underlying(value);
    }

    template<class T,
      enable_if_is_chrono_duration_t<T>* = nullptr>
    void operator()(const T& value) const {
      _json_object = value.count();
    }

    template<class T, 
      enable_if_is_directly_serializable_t<T>* = nullptr>
    void operator()(const T& value) const {
      _json_object = value;
    }

    template<class T, 
      enable_if_is_container_t<T>* = nullptr>
    void operator()(const T& value) const {
      for (const auto& each : value) {
        nlohmann::json json_subobject;
        jsonizer subjsonizer(json_subobject);
        subjsonizer(each);
        _json_object.push_back(json_subobject); 
      }
    }

    template<class T, 
      enable_if_is_associative_container_t<T>* = nullptr>
    void operator()(const T& value) const {
      for (const auto& each : value) {
        nlohmann::json json_subobject;
        jsonizer subjsonizer(json_subobject);
        subjsonizer(each.second);
        _json_object[each.first] = json_subobject; 
      }
    }

    
    private:
      nlohmann::json& _json_object;
    
  };


}}}

#endif
