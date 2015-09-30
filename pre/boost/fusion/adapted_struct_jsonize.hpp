#ifndef BOOST_FUSION_ADAPTED_STRUCT_JSONIZE_HPP
#define BOOST_FUSION_ADAPTED_STRUCT_JSONIZE_HPP

#include <boost/variant.hpp>
#include <pre/boost/fusion/traits/is_boost_variant.hpp>
#include <pre/boost/fusion/traits/is_container.hpp>
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
      boost::fusion::traits::is_container<T>::value && !boost::fusion::traits::is_sequence<T>::value && !traits::is_boost_variant<T>::value
    ,T>::type;

    struct adapted_struct_jsonize : public boost::static_visitor<> {

      adapted_struct_jsonize(nlohmann::json& json_object) 
        : boost::static_visitor<>(),  _json_object(json_object) {}

      template<class T, 
        enable_if_is_adapted_struct_t<T>* = nullptr>
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
      void operator()(const char* name, const T& value) const {
        nlohmann::json json_subobject;
        adapted_struct_jsonize subjsonizer(json_subobject);
        subjsonizer(value);

        _json_object[name] = json_subobject;
      }

      template<class T, 
        enable_if_is_other_sequence_and_not_variant_t<T>* = nullptr>
      void operator()(const T& value) const {
        boost::fusion::for_each(value, *this);
        // HMMMMM ???
      }

      template<class T, 
        enable_if_is_variant_t<T>* = nullptr>
      void operator()(const char* name, const T& value) const {
        nlohmann::json json_subobject;
        adapted_struct_jsonize subjsonizer(json_subobject);
        subjsonizer(value);

        _json_object[name] = json_subobject;
      }

      template<class T, 
        enable_if_is_variant_t<T>* = nullptr>
      void operator()(const T& value) const {
        boost::apply_visitor(*this, value);
      }

      template<class T, 
        enable_if_is_not_sequence_nor_variant_t<T>* = nullptr>
      void operator()(const char* name, const T& value) const {
        nlohmann::json json_subobject;
        adapted_struct_jsonize subjsonizer(json_subobject);
        subjsonizer(value);

        _json_object[name] = json_subobject;
      }

      template<class T, 
        enable_if_is_not_sequence_nor_variant_t<T>* = nullptr>
      void operator()(const T& value) const {
        _json_object = value;
      }

      template<class T, 
        enable_if_is_container_t<T>* = nullptr>
      void operator()(const char* name, const T& value) const {
        nlohmann::json json_subobject;
        adapted_struct_jsonize subjsonizer(json_subobject);
        subjsonizer(value);

        _json_object[name] = json_subobject;
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
      
      private:
      nlohmann::json& _json_object;
      
    };

  }

  namespace adapted_struct_jsonize {
    template<class T, 
      detail::enable_if_is_adapted_struct_t<T>* = nullptr>
    nlohmann::json jsonize(const T& value) {
      nlohmann::json json_object;
      detail::adapted_struct_jsonize jsonizer(json_object);
      jsonizer(value);
      return json_object;
    }

  }



  namespace detail {

    template<typename TDejsonize>
    struct adapted_struct_dejsonize : public boost::static_visitor<> {

        adapted_struct_dejsonize(TDejsonize& object, const nlohmann::json& json_object) 
          : boost::static_visitor<>(),  _object(object), _json_object(json_object) {}

        //template<class T, 
        //  enable_if_is_adapted_struct_t<T>* = nullptr>
        //void operator()(const char* name, T& value) const {
        //  std::cout << "name is " << name << std::endl;
        //  value = _object[name];
        //}

        template<class T, 
          enable_if_is_adapted_struct_t<T>* = nullptr>
        void operator()(T& value) const {
          boost::fusion::for_each_member(value, *this);
        }

        //template<class T, 
        //  enable_if_is_other_sequence_and_not_variant_t<T>* = nullptr>
        //void operator()(const char* name, const T& value) const {
        //  nlohmann::json json_subobject;
        //  adapted_struct_jsonize subjsonizer(json_subobject);
        //  subjsonizer(value);

        //  _json_object[name] = json_subobject;
        //}

        //template<class T, 
        //  enable_if_is_other_sequence_and_not_variant_t<T>* = nullptr>
        //void operator()(const T& value) const {
        //  boost::fusion::for_each(value, *this);
        //  // HMMMMM ???
        //}

        //template<class T, 
        //  enable_if_is_variant_t<T>* = nullptr>
        //void operator()(const char* name, const T& value) const {
        //  nlohmann::json json_subobject;
        //  adapted_struct_jsonize subjsonizer(json_subobject);
        //  subjsonizer(value);

        //  _json_object[name] = json_subobject;
        //}

        //template<class T, 
        //  enable_if_is_variant_t<T>* = nullptr>
        //void operator()(const T& value) const {
        //  boost::apply_visitor(*this, value);
        //}

        template<class T, 
          enable_if_is_not_sequence_nor_variant_t<T>* = nullptr>
        void operator()(const char* name, T& value) const {
          std::cout << "name is " << name << std::endl;
          value = _json_object[name].get<T>();
        }

        //template<class T, 
        //  enable_if_is_not_sequence_nor_variant_t<T>* = nullptr>
        //void operator()(const T& value) const {
        //  _json_object = value;
        //}

        //template<class T, 
        //  enable_if_is_container_t<T>* = nullptr>
        //void operator()(const char* name, T& value) const {
        //}

        //template<class T, 
        //  enable_if_is_container_t<T>* = nullptr>
        //void operator()(const T& value) const {
        //  for (const auto& each : value) {
        //    nlohmann::json json_subobject;
        //    adapted_struct_jsonize subjsonizer(json_subobject);
        //    subjsonizer(each);
        //    _json_object.push_back(json_subobject); 
        //  }
        //}
        
        private:
          TDejsonize& _object;
          const nlohmann::json& _json_object;
        
      };

  }



  namespace adapted_struct_dejsonize {
    template<class T,
      detail::enable_if_is_adapted_struct_t<T>* = nullptr>
    T dejsonize(const nlohmann::json& json_object) {
      T object;
      detail::adapted_struct_dejsonize<T> dejsonizer(object, json_object);
      dejsonizer(object);
      return object;
    }

  }


}}


#endif
