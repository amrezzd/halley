/*****************************************************************\
           __
          / /
		 / /                     __  __
		/ /______    _______    / / / / ________   __       __
	   / ______  \  /_____  \  / / / / / _____  | / /      / /
	  / /      | / _______| / / / / / / /____/ / / /      / /
	 / /      / / / _____  / / / / / / _______/ / /      / /
	/ /      / / / /____/ / / / / / / |______  / |______/ /
   /_/      /_/ |________/ / / / /  \_______/  \_______  /
                          /_/ /_/                     / /
			                                         / /
		       High Level Game Framework            /_/

  ---------------------------------------------------------------

  Copyright (c) 2007-2014 - Rodrigo Braz Monteiro.
  This file is subject to the terms of halley_license.txt.

\*****************************************************************/

#include "../../include/halley/file_formats/string_serializer.h"
using namespace Halley;

String StringSerializer::encode(Vector2i v)
{
	return String::integerToString(v.x) + "," + String::integerToString(v.y);
}

String StringSerializer::encode(Vector2f v)
{
	return String::floatToString(v.x) + "," + String::floatToString(v.y);
}

String StringSerializer::encode(Vector3i v)
{
	return String::integerToString(v.x) + "," + String::integerToString(v.y) + "," + String::integerToString(v.z);
}

String StringSerializer::encode(Vector3f v)
{
	return String::floatToString(v.x) + "," + String::floatToString(v.y) + "," + String::floatToString(v.z);
}

String StringSerializer::encode(String v)
{
	return v;
}

String StringSerializer::encode(int v)
{
	return String::integerToString(v);
}

String StringSerializer::encode(float v)
{
	return String::floatToString(v);
}

String StringSerializer::encode(bool v)
{
	return v ? "true" : "false";
}

Halley::String Halley::StringSerializer::encode(Colour c)
{
	return c.toString();
}

StringDeserializer StringSerializer::decode(String v)
{
	return StringDeserializer(v);
}

StringDeserializer::StringDeserializer(String v)
	: value(v)
{
}

StringDeserializer::operator Vector2i()
{
	auto values = value.split(',');
	return Vector2i(values[0].toInteger(), values[1].toInteger());
}

StringDeserializer::operator Vector2f()
{
	auto values = value.split(',');
	return Vector2f(values[0].toFloat(), values[1].toFloat());
}

StringDeserializer::operator Vector3i()
{
	auto values = value.split(',');
	return Vector3i(values[0].toInteger(), values[1].toInteger(), values[2].toInteger());
}

StringDeserializer::operator Vector3f()
{
	auto values = value.split(',');
	return Vector3f(values[0].toFloat(), values[1].toFloat(), values[2].toFloat());
}

StringDeserializer::operator String()
{
	return value;
}

StringDeserializer::operator int()
{
	return value.toInteger();
}

StringDeserializer::operator float()
{
	return value.toFloat();
}

StringDeserializer::operator bool()
{
	return value.asciiLower() == "true";
}

StringDeserializer::operator Colour()
{
	return Colour::fromString(value);
}
