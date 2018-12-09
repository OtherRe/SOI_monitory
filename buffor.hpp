#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdexcept>
#include <cstddef>

class Buffer
{
    std::size_t elems;
    std::size_t maxCapacity;

  public:
    Buffer(std::size_t maxCapacity) : elems(0), maxCapacity(maxCapacity) {}
    std::size_t size()
    {
        return elems;
    }
    std::size_t maxSize()
    {
        return maxCapacity;
    }

    void addElems(std::size_t numberOfElements)
    {
        if (elems + numberOfElements > maxCapacity)
            throw std::runtime_error("Buffer overflow");
        elems += numberOfElements;
        std::cout << "Added " << numberOfElements << " size is " << elems << std::endl;
    }
    void removeElems(std::size_t numberOfElements)
    {
        if (elems < numberOfElements)
            throw std::runtime_error("Removing from empty buffer");
        elems -= numberOfElements;
        std::cout << "Removed " << numberOfElements << " size is " << elems << std::endl;

    }
};
class BufferMonitor
{
  public:
    BufferMonitor(std::size_t bufferSize) : buffer(bufferSize)
    {
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&producers, NULL);
        pthread_cond_init(&consumers, NULL);
    }

    void addElements(std::size_t numberOfElements)
    {
        pthread_mutex_lock(&mutex);

        while (buffer.size() + numberOfElements > buffer.maxSize())
        {
            producerWait();
        }

        buffer.addElems(numberOfElements);

        signal();
        pthread_mutex_unlock(&mutex);
    }
    void removeElements(std::size_t numberOfElements)
    {
        pthread_mutex_lock(&mutex);

        while (buffer.size() < numberOfElements)
        {
            consumerWait();
        }

        buffer.removeElems(numberOfElements);

        signal();
        pthread_mutex_unlock(&mutex);
    }

  private:
    Buffer buffer;

    pthread_mutex_t mutex;
    pthread_cond_t producers;
    pthread_cond_t consumers;
    std::size_t producersWaiting = 0;
    std::size_t consumersWaiting = 0;

    void producerWait()
    {
        producersWaiting++;
        pthread_cond_wait(&producers, &mutex);
    }
    void consumerWait()
    {
        consumersWaiting++;
        pthread_cond_wait(&consumers, &mutex);
    }

    void signal()
    {
        if (!consumersWaiting && !producersWaiting)
            return;
        if (consumersWaiting > producersWaiting)
        {
            --consumersWaiting;
            pthread_cond_signal(&consumers);
        }
        else
        {
            --producersWaiting;
            pthread_cond_signal(&producers);
        }
    }
};
