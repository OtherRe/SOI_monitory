#include <iostream>
#include <string>
#include "RandomGenerator.hpp"
#include "buffor.hpp"

class Manager {
 public:
  Manager(int id, BufferMonitor &monitor, std::size_t low, std::size_t high, std::string log_name)
      : id(id),
        monitor(monitor),
        generator(low, high),
        log_name(log_name) {}

  virtual void useBuffer() = 0;

 protected:
  int id;
  BufferMonitor &monitor;
  RandomGenerator generator;
  std::string log_name;

  void log(const std::string &msg) {
    FILE *log;
    log = fopen(log_name.c_str(), "a+");
    fputs(msg.c_str(), log);
    fclose(log);
  }
};

class Producer : public Manager {
 public:
  Producer(int id, BufferMonitor &monitor, std::size_t low, std::size_t high)
      : Manager(id, monitor, low, high, "producer_log" + std::to_string(id))
      {}

  void useBuffer() {
    size_t r = generator.randomRange();
    log("Trying to add " + std::to_string(r) + " elements\n");
    size_t newSize = monitor.addElements(r);
    log("Added " + std::to_string(r) + " elements. Now buffer has " + std::to_string(newSize) + '\n');
    sleep(1);
  }
};

class Consumer : public Manager {
 public:
  Consumer(int id, BufferMonitor &monitor, std::size_t low, std::size_t high)
      : Manager(id, monitor, low, high, "consumer_log" + std::to_string(id))
      {}

  void useBuffer() {
    size_t r = generator.randomRange();
    log("Trying to remove " + std::to_string(r) + " elements\n");
    size_t newSize = monitor.removeElements(r);
    log("Removed " + std::to_string(r) + " elements. Now buffer has " + std::to_string(newSize) + '\n');
    sleep(1);
  }
};