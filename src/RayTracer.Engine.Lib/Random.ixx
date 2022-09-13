module;

#include <cstdint>
#include <random>

export module RayTracer.Random;

namespace RayTracer
{
    export class Random
    {
    private:
        mutable uint32_t _state{};

    public:
        inline constexpr Random()
        {
            if (std::is_constant_evaluated())
            {
                _state = 7910260;
            }
            else
            {
                std::random_device randomDevice{};
                _state = randomDevice();
            }
        }

        inline constexpr explicit Random(uint32_t seed)
            : _state{seed}
        {

        }

        inline constexpr uint32_t GetInteger() const
        {
            uint32_t x = _state;
            x ^= x << 13;
            x ^= x >> 17;
            x ^= x << 5;

            _state = x;

            return _state;
        }

        inline constexpr float GetNormalizedFloat() const
        {
            uint32_t x = GetInteger();
            return static_cast<float>(x) / static_cast<float>(4294967295);
        }
    };
}