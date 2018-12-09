#include <random>
#include <cstddef>
#include <chrono>

class RandomGenerator
{
  public:
    RandomGenerator(std::size_t low, std::size_t high) : distribution(low, high)
    {
        generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    }
    std::size_t randomRange()
    {
        return distribution(generator);
    }

  private:
    std::default_random_engine generator;
    std::uniform_int_distribution<size_t> distribution;
};