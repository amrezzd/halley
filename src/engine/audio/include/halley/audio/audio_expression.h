#pragma once
#include "halley/data_structures/vector.h"
#include "halley/text/string_converter.h"

namespace Halley {
	class ConfigNode;
	class AudioEmitter;
    class Serializer;
    class Deserializer;

    enum class AudioExpressionTermType {
	    Switch,
        Variable
    };

	template <>
	struct EnumNames<AudioExpressionTermType> {
		constexpr std::array<const char*, 2> operator()() const {
			return{{
				"switch",
                "variable"
			}};
		}
	};

    class AudioExpressionTerm {
    public:
        AudioExpressionTermType type;
        String id;
        String value;

        AudioExpressionTerm() = default;
        AudioExpressionTerm(const ConfigNode& node);

        float evaluate(const AudioEmitter& emitter) const;

        void serialize(Serializer& s) const;
        void deserialize(Deserializer& s);
    };

	class AudioExpression {
    public:
        void load(const ConfigNode& node);

        float evaluate(const AudioEmitter& emitter) const;

        void serialize(Serializer& s) const;
        void deserialize(Deserializer& s);

	private:
        Vector<AudioExpressionTerm> terms;
    };
}
