#ifndef HEADER_KRPC_DECODER
#define HEADER_KRPC_DECODER

#include "krpc/krpc.pb.hpp"
#include "krpc/object.hpp"
#include <boost/exception/all.hpp>
#include <google/protobuf/message.h>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <string>

namespace krpc {
  namespace decoder {

    struct DecodeFailed : virtual boost::exception, virtual std::exception {};

    const char OK_MESSAGE[] = { 0x4F, 0x4B };
    const size_t OK_MESSAGE_LENGTH = 2;
    const size_t GUID_LENGTH = 16;

    std::string guid(const std::string& data);

    void decode(float& value, const std::string& data, Client* client = NULL);
    void decode(double& value, const std::string& data, Client* client = NULL);
    void decode(google::protobuf::int32& value, const std::string& data, Client* client = NULL);
    void decode(google::protobuf::int64& value, const std::string& data, Client* client = NULL);
    void decode(google::protobuf::uint32& value, const std::string& data, Client* client = NULL);
    void decode(google::protobuf::uint64& value, const std::string& data, Client* client = NULL);
    void decode(bool& value, const std::string& data, Client* client = NULL);
    void decode(std::string& value, const std::string& data, Client* client = NULL);
    void decode(google::protobuf::Message& message, const std::string& data, Client* client = NULL);
    template <typename T> void decode(Object<T>& object, const std::string& data, Client* client = NULL);

    template <typename T> void decode(std::vector<T>& list, const std::string& data, Client* client = NULL);
    template <typename K, typename V> void decode(std::map<K,V>& dictionary, const std::string& data, Client* client = NULL);
    template <typename T> void decode(std::set<T>& set, const std::string& data, Client* client = NULL);

    template <typename T0> void decode(boost::tuple<T0>& tuple, const std::string& data, Client* client = NULL);
    template <typename T0, typename T1> void decode(boost::tuple<T0,T1>& tuple, const std::string& data, Client* client = NULL);
    template <typename T0, typename T1, typename T2> void decode(boost::tuple<T0,T1,T2>& tuple, const std::string& data, Client* client = NULL);
    template <typename T0, typename T1, typename T2, typename T3> void decode(boost::tuple<T0,T1,T2,T3>& tuple, const std::string& data, Client* client = NULL);
    template <typename T0, typename T1, typename T2, typename T3, typename T4> void decode(boost::tuple<T0,T1,T2,T3,T4>& tuple, const std::string& data, Client* client = NULL);

    template <typename T> void decode_enum(T& value, const std::string& data, Client* client);

    void decode_delimited(google::protobuf::Message& message, const std::string& data);

    std::pair<google::protobuf::uint32, google::protobuf::uint32> decode_size_and_position(const std::string& data);

    template <typename T>
    inline void decode(Object<T>& object, const std::string& data, Client* client) {
      google::protobuf::uint64 id;
      decode(id, data, client);
      object.client = client;
      object.id = id;
    }

    template <typename T>
    inline void decode(std::vector<T>& list, const std::string& data, Client* client) {
      list.clear();
      krpc::schema::List listMessage;
      listMessage.ParseFromString(data);
      for (int i = 0; i < listMessage.items_size(); i++) {
        T value;
        decode(value, listMessage.items(i), client);
        list.push_back(value);
      }
    }

    template <typename K, typename V>
    inline void decode(std::map<K,V>& dictionary, const std::string& data, Client* client) {
      dictionary.clear();
      krpc::schema::Dictionary dictionaryMessage;
      dictionaryMessage.ParseFromString(data);
      for (int i = 0; i < dictionaryMessage.entries_size(); i++) {
        const schema::DictionaryEntry& entry = dictionaryMessage.entries(i);
        K key;
        V value;
        decode(key, entry.key(), client);
        decode(value, entry.value(), client);
        dictionary[key] = value;
      }
    }

    template <typename T>
    inline void decode(std::set<T>& set, const std::string& data, Client* client) {
      set.clear();
      krpc::schema::Set setMessage;
      setMessage.ParseFromString(data);
      for (int i = 0; i < setMessage.items_size(); i++) {
        T value;
        decode(value, setMessage.items(i), client);
        set.insert(value);
      }
    }

    template <typename T0>
    inline void decode(boost::tuple<T0>& tuple, const std::string& data, Client* client) {
      krpc::schema::Tuple tupleMessage;
      tupleMessage.ParseFromString(data);
      decode(boost::get<0>(tuple), tupleMessage.items(0), client);
    }

    template <typename T0, typename T1>
    inline void decode(boost::tuple<T0,T1>& tuple, const std::string& data, Client* client) {
      krpc::schema::Tuple tupleMessage;
      tupleMessage.ParseFromString(data);
      decode(boost::get<0>(tuple), tupleMessage.items(0), client);
      decode(boost::get<1>(tuple), tupleMessage.items(1), client);
    }

    template <typename T0, typename T1, typename T2>
    inline void decode(boost::tuple<T0,T1,T2>& tuple, const std::string& data, Client* client) {
      krpc::schema::Tuple tupleMessage;
      tupleMessage.ParseFromString(data);
      decode(boost::get<0>(tuple), tupleMessage.items(0), client);
      decode(boost::get<1>(tuple), tupleMessage.items(1), client);
      decode(boost::get<2>(tuple), tupleMessage.items(2), client);
    }

    template <typename T0, typename T1, typename T2, typename T3>
    inline void decode(boost::tuple<T0,T1,T2,T3>& tuple, const std::string& data, Client* client) {
      krpc::schema::Tuple tupleMessage;
      tupleMessage.ParseFromString(data);
      decode(boost::get<0>(tuple), tupleMessage.items(0), client);
      decode(boost::get<1>(tuple), tupleMessage.items(1), client);
      decode(boost::get<2>(tuple), tupleMessage.items(2), client);
      decode(boost::get<3>(tuple), tupleMessage.items(3), client);
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4>
    inline void decode(boost::tuple<T0,T1,T2,T3,T4>& tuple, const std::string& data, Client* client) {
      krpc::schema::Tuple tupleMessage;
      tupleMessage.ParseFromString(data);
      decode(boost::get<0>(tuple), tupleMessage.items(0), client);
      decode(boost::get<1>(tuple), tupleMessage.items(1), client);
      decode(boost::get<2>(tuple), tupleMessage.items(2), client);
      decode(boost::get<3>(tuple), tupleMessage.items(3), client);
      decode(boost::get<4>(tuple), tupleMessage.items(4), client);
    }

    template <typename T>
    inline void decode_enum(T& value, const std::string& data, Client* client) {
      google::protobuf::int32 x;
      decode(x, data, client);
      value = static_cast<T>(x);
    }

  }
}

#endif
