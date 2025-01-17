#pragma once

#include "halley/text/string_converter.h"

namespace Halley {
	class Deserializer;
	class Serializer;
	class ConfigNode;

	enum class AudioFadeCurve : uint8_t {
		None,
		Linear,
		Sinusoidal
	};

	template <>
	struct EnumNames<AudioFadeCurve> {
		constexpr std::array<const char*, 3> operator()() const {
			return{{
				"none",
				"linear",
				"sinusoidal"
			}};
		}
	};

	class AudioFade {
	public:
		AudioFade() = default;
		AudioFade(float length, AudioFadeCurve curve);
		AudioFade(const ConfigNode& node);

		float evaluate(float time) const;

		float getLength() const;
		void setLength(float len);
		AudioFadeCurve getCurve() const;
		void setCurve(AudioFadeCurve curve);
		bool hasFade() const;

		void serialize(Serializer& s) const;
		void deserialize(Deserializer& s);

		ConfigNode toConfigNode() const;

	private:
		float length = 0;
		AudioFadeCurve curve = AudioFadeCurve::None;
	};

	class AudioFader {
	public:
		void startFade(float from, float to, const AudioFade& fade);
		void stopAndSetValue(float value);

		/// Returns true if the fade just ended
		bool update(float time);

		bool isFading() const;
		float getCurrentValue() const;
		float getTargetValue() const;

	private:
		AudioFade fade;
		bool fading = false;
		float time = 0;
		float timeScale = 1;
		float startVal = 0;
		float endVal = 0;
	};
}