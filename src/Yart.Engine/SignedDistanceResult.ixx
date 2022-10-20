export module SignedDistanceResult;

import "Common.h";

namespace Yart
{
    export class SignedDistanceResult
    {
    public:
        real Distance{};
        real MixAmount{};

        SignedDistanceResult(real distance, real mixAmount)
            : Distance{distance}, MixAmount{mixAmount}
        {

        }
    };
}