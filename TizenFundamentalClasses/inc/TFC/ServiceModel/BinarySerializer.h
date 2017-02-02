/*
 * KeyValuePairSerializer.h
 *
 *  Created on: Jan 30, 2017
 *      Author: gilang
 */

#ifndef TFC_SERVICEMODEL_BINARYSERIALIZER_H_
#define TFC_SERVICEMODEL_BINARYSERIALIZER_H_

#include <string>
#include <vector>
#include <sstream>

#include "TFC/Core/Introspect.h"
#include "TFC/Core/Reflection.h"
#include "TFC/ServiceModel/Includes.h"
#include "TFC/ServiceModel/ServerEndpoint.h"
#include "TFC/Serialization/ClassSerializer.h"
#include "TFC/Serialization/DiscriminatedUnionSerializer.h"

namespace TFC {
namespace ServiceModel {

class BinarySerializer
{
public:
	typedef std::vector<uint8_t> SerializedType;



	BinarySerializer();

	void Serialize(uint32_t args);
	void Serialize(int64_t args);
	void Serialize(int args);
	void Serialize(bool args);
	void Serialize(double args);
	void Serialize(std::string const& args);

	BinarySerializer CreateScope();
	void Serialize(BinarySerializer& p);

	SerializedType EndPack();

	~BinarySerializer();

private:
	bool doDestruction;
	SerializedType* buffer;
	BinarySerializer(SerializedType* bufferRef);
};

struct BinaryDeserializer
{
	typedef std::vector<uint8_t> SerializedType;

private:
	SerializedType const& bufferRef;
	size_t currentPos;



	template<typename T>
	T DeserializeImpl()
	{
		T tmp = 0;
		uint8_t* ref = (uint8_t*)&tmp;

		for(size_t i = 0; i < sizeof(tmp); i++)
		{
			ref[i] = bufferRef[currentPos + i];
		}

		currentPos += sizeof(tmp);
		return tmp;
	}

	BinaryDeserializer(SerializedType const& p, size_t currentPos);

public:
	BinaryDeserializer(SerializedType const& p);

	template<typename, typename> friend struct DeserializeSelector;

	template<typename T, typename = void>
	struct DeserializerSelector
	{
		static T Deserialize(BinaryDeserializer& deser)
		{
			return deser.DeserializeImpl<T>();
		}
	};

	void Deserialize(int8_t& target);
	void Deserialize(int16_t& target);
	void Deserialize(int32_t& target);
	void Deserialize(int64_t& target);

	void Deserialize(uint8_t& target);
	void Deserialize(uint16_t& target);
	void Deserialize(uint32_t& target);
	void Deserialize(uint64_t& target);

	void Deserialize(std::string& target);
	void Deserialize(bool& target);
	void Deserialize(double& target);

	//void Deserialize(SerializedType& composite);

	BinaryDeserializer& DeserializeScope() { return *this; }

	void Finalize();
};

template<typename T>
struct BinaryDeserializer::DeserializerSelector<T, Core::Metaprogramming::Void_T<typename Serialization::TypeSerializationInfoSelector<T>::Type>>
{
	static T Deserialize(BinaryDeserializer& deser)
	{
		using namespace TFC::Serialization;
		return ClassDeserializer<BinaryDeserializer, T>::Deserialize(deser);
	}
};

template<typename T>
struct BinaryDeserializer::DeserializerSelector<T, Core::Metaprogramming::Void_T<typename std::enable_if<std::is_enum<T>::value>::type>>
{
	static T Deserialize(BinaryDeserializer& deser)
	{

	}
};

template<typename T>
struct BinaryDeserializer::DeserializerSelector<T, Core::Metaprogramming::Void_T<typename Serialization::DiscriminatedUnionTypeInfoSelector<T>::Type>>
{
	static T Deserialize(BinaryDeserializer& deser)
	{
		using namespace TFC::Serialization;
		return GenericDeserializer<BinaryDeserializer, T>::Deserialize(deser);

	}
};

template<typename T>
struct BinaryDeserializer::DeserializerSelector<std::vector<T>, Core::Metaprogramming::Void_T<typename Serialization::TypeSerializationInfoSelector<T>::Type>>
{
	static std::vector<T> Deserialize(BinaryDeserializer& deser)
	{
		using namespace TFC::Serialization;

	}
};

}
}



#endif /* TFC_SERVICEMODEL_BINARYSERIALIZER_H_ */