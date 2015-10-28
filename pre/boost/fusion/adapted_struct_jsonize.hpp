#ifndef BOOST_FUSION_ADAPTED_STRUCT_JSONIZE_HPP
#define BOOST_FUSION_ADAPTED_STRUCT_JSONIZE_HPP

#include <boost/exception_ptr.hpp>

#include <boost/mpl/for_each.hpp>

#include <boost/variant.hpp>
#include <pre/boost/fusion/traits/is_boost_variant.hpp>
#include <pre/boost/fusion/traits/is_container.hpp>
#include <pre/boost/fusion/traits/is_associative_container.hpp>
#include <boost/fusion/include/tag_of.hpp>


#include <boost/fusion/include/is_sequence.hpp>
#include <pre/boost/fusion/for_each_member.hpp>

#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/boost_tuple.hpp>

#include <nlohmann/json.hpp>

namespace boost { namespace fusion {


  namespace detail {
    template<class T>
    using enable_if_is_not_sequence_nor_variant_t = typename std::enable_if<
      ! ( boost::fusion::traits::is_container<T>::value || 
          boost::fusion::traits::is_associative_container<T>::value ||
          boost::fusion::traits::is_sequence<T>::value 
          || traits::is_boost_variant<T>::value)
    ,T>::type;

    template<class T>
    using enable_if_is_other_sequence_and_not_variant_t = typename std::enable_if<
      boost::fusion::traits::is_sequence<T>::value
      && !std::is_same< 
            typename boost::fusion::traits::tag_of<T>::type, 
            boost::fusion::struct_tag
          >::value
      && !traits::is_boost_variant<T>::value 
    ,T>::type;

    template<class T>
    using enable_if_is_adapted_struct_t = typename std::enable_if< 
      std::is_same<
        typename boost::fusion::traits::tag_of<T>::type, 
        boost::fusion::struct_tag
      >::value
    ,T>::type;


    template<class T>
    using enable_if_is_variant_t = typename std::enable_if<
      traits::is_boost_variant<T>::value
    ,T>::type;

    template<class T>
    using enable_if_is_container_t = typename std::enable_if<
      traits::is_container<T>::value
    ,T>::type;

    template<class T>
    using enable_if_is_associative_container_t = typename std::enable_if<
      boost::fusion::traits::is_associative_container<T>::value 
    ,T>::type;


    struct adapted_struct_jsonize : public boost::static_visitor<> {

      adapted_struct_jsonize(nlohmann::json& json_object) 
        : boost::static_visitor<>(),  _json_object(json_object) {}

      template<class T>
      void operator()(const char* name, const T& value) const {
        nlohmann::json json_subobject;
        adapted_struct_jsonize subjsonizer(json_subobject);
        subjsonizer(value);

        _json_object[name] = json_subobject;
      }

      template<class T, 
        enable_if_is_adapted_struct_t<T>* = nullptr>
      void operator()(const T& value) const {
        boost::fusion::for_each_member(value, *this);
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
        enable_if_is_not_sequence_nor_variant_t<T>* = nullptr>
      void operator()(const T& value) const {
        _json_object = value;
      }

      template<class T, 
        enable_if_is_container_t<T>* = nullptr>
      void operator()(const T& value) const {
        for (const auto& each : value) {
          nlohmann::json json_subobject;
          adapted_struct_jsonize subjsonizer(json_subobject);
          subjsonizer(each);
          _json_object.push_back(json_subobject); 
        }
      }

      template<class T, 
        enable_if_is_associative_container_t<T>* = nullptr>
      void operator()(const T& value) const {
        for (const auto& each : value) {
          nlohmann::json json_subobject;
          adapted_struct_jsonize subjsonizer(json_subobject);
          subjsonizer(each.second);
          _json_object[each.first] = json_subobject; 
        }
      }

      
      private:
        nlohmann::json& _json_object;
      
    };

  }

  namespace adapted_struct_jsonize {
    template<class T>
    nlohmann::json jsonize(const T& value) {
      nlohmann::json json_object;
      detail::adapted_struct_jsonize jsonizer(json_object);
      jsonizer(value);
      return json_object;
    }
  }



  namespace detail {

    struct adapted_struct_dejsonize {

        adapted_struct_dejsonize(const nlohmann::json& json_object) : 
          _json_object(json_object) {}

        template<class T> 
        void operator()(const char* name, T& value) const {

          if (_json_object.find(name) != std::end(_json_object)) {
            adapted_struct_dejsonize dejsonizer(_json_object[name]);
            dejsonizer(value);
          } else {
            throw std::runtime_error(
              "Missing key " + std::string(name) + " in JSON Object : " + _json_object.dump());
          }
        }

        template<class T, 
          enable_if_is_adapted_struct_t<T>* = nullptr>
        void operator()(T& value) const {
          if (_json_object.is_object()) {
            boost::fusion::for_each_member(value, *this);
          } else {
            throw std::runtime_error("The JSON Object " + _json_object.dump() + " isn't an object");
          }
        }

        template<typename TVariant>
        struct variant_checker {
          
          variant_checker(const nlohmann::json& json_object, TVariant& value) : _json_object(json_object), value(value) {
          };

          template< typename U > void operator()(U& x) {
            if (!successed) {
              try {
                adapted_struct_dejsonize dejsonizer(_json_object);
                U test_value;
                dejsonizer(test_value);
                successed = true;
                value = test_value;
              } catch (...) { //XXX: LOGIC BASED ON Exceptions !!! ARgh
                //std::cout << "ERROR" << std::endl;
                //std::cout << boost::current_exception_diagnostic_information(true) << std::endl;
              }
            }
          }

          private:
            TVariant &value;
            const nlohmann::json& _json_object;
            bool successed = false;
        };

        template<class T, 
          enable_if_is_variant_t<T>* = nullptr>
        void operator()(T& value) const {
          //TODO: implement
          //TODO: Check what is in the json object to see if there is all the field we need for one of the types

          boost::mpl::for_each< typename T::types >( variant_checker<T>(_json_object, value) );
          
        }

        template<class T, 
          enable_if_is_not_sequence_nor_variant_t<T>* = nullptr>
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
              adapted_struct_dejsonize dejsonizer(entry_json);
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
              adapted_struct_dejsonize dejsonizer(it.value());
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

  }

  namespace adapted_struct_dejsonize {
    template<class T>
    T dejsonize(const nlohmann::json& json_object) {
      T object;
      detail::adapted_struct_dejsonize dejsonizer(json_object);
      dejsonizer(object);
      return object;
    }
  }


}}


#endif
