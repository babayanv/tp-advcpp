#ifndef RNG_HPP
#define RNG_HPP

#include <random>
#include <ratio>


template<class IntType>
class RNG
{
public:
    static IntType roll(IntType bottom, IntType top = std::numeric_limits<IntType>::max())
    {
        std::uniform_int_distribution<IntType> m_dist(bottom, top);
        return m_dist(RNG<IntType>::m_engine);
    }

private:
    static std::random_device m_rand_device;
    static std::default_random_engine m_engine;
};

template<class IntType>
std::random_device RNG<IntType>::m_rand_device{};

template<class IntType>
std::default_random_engine RNG<IntType>::m_engine{m_rand_device()};


#endif // RNG_HPP
