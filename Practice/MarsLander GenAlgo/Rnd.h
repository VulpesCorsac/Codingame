#pragma once

#define RNDCPP

#ifdef RNDCPP
    #include <random>
#endif // RNDCPP

class Rnd {
public:
    static float rndFloat() noexcept {
        #ifdef RNDCPP
            static std::random_device rd;
            static thread_local std::default_random_engine generator(rd());
            static std::uniform_real_distribution < float > distribution(0.0, 1.0);
            return distribution(generator);
        #else
            return rand() / RAND_MAX;
        #endif
    }
};
