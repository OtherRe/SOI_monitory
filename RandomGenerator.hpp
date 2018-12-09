#include <random>
#include <cstddef>

class RandomGenerator{
    public:
        RandomGenerator(std::size_t low, std::size_t high): distribution(low, high){}
        std::size_t randomRange()
        {
            return distribution(generator);
        }

    private:
        std::default_random_engine generator;
        std::uniform_int_distribution<size_t> distribution;
};