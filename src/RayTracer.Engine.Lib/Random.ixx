#include <cstdint>

export module RayTracer.Random;

namespace RayTracer
{
    export class Random
    {
    private:
        mutable uint32_t _state;

    public:
        Random(uint32_t seed = 92134)
            : _state{seed}
        {

        }

        inline uint32_t GetInteger() const
        {
            uint32_t x = _state;
            x ^= x << 13;
            x ^= x >> 17;
            x ^= x << 5;

            _state = x;

            return _state;
        }

        inline float GetNormalizedFloat() const
        {
            uint32_t x = GetInteger();
            return static_cast<float>(x) / static_cast<float>(4294967295);
        }
    };
}