#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstddef>
#include <string>
#include <vector>
#include "buffor.hpp"
#include "RandomGenerator.hpp"
using namespace std;

size_t low;
size_t high;

void readInput(size_t& buffSize, size_t& producerNumber, size_t& consumerNumber);
void initilizeThreads(vector<pthread_t>& producers, vector<pthread_t>& consumers, BufferMonitor& monitor);
void joinAll(vector<pthread_t>& producers, vector<pthread_t>& consumers);


void *producent(void *mon)
{   
    BufferMonitor* monitor = (BufferMonitor*)mon;
    RandomGenerator generator(low, high);

    while(true){
        monitor->addElements(generator.randomRange());
    }
}
void *consument(void *mon)
{   
    BufferMonitor* monitor = (BufferMonitor*)mon;
    RandomGenerator generator(low, high);

    while(true){
        monitor->removeElements(generator.randomRange());
    }
}

int main(int argc, char* argv[])
{
    size_t buffSize;
    size_t producerNumber;
    size_t consumerNumber;
    readInput(buffSize, producerNumber, consumerNumber);

    BufferMonitor monitor(buffSize);
    vector<pthread_t> producers(producerNumber);
    vector<pthread_t> consumers(consumerNumber);

    initilizeThreads(producers, consumers, monitor);
    joinAll(producers, consumers);

    return 0;
}


void readInput(size_t& buffSize, size_t& producerNumber, size_t& consumerNumber)
{
    cout << "Please give max buff size: ";
    cin >> buffSize;
    cout << "Please give how much producers you want: ";
    cin >> producerNumber;
    cout << "Please give how much consumers you want: ";
    cin >> consumerNumber;
    cout << "Please give lower bound: ";
    cin >> low;
    cout << "Please give upper bound: ";
    cin >> high;
}
void initilizeThreads(vector<pthread_t>& producers, vector<pthread_t>& consumers, BufferMonitor& monitor)
{
    for(pthread_t& p : producers)
    {
        pthread_create(&p, NULL, producent, (void *)(&monitor));
    }
    for(pthread_t& c : consumers)
    {
        pthread_create(&c, NULL, consument, (void *)(&monitor));
    }
}
void joinAll(vector<pthread_t>& producers, vector<pthread_t>& consumers)
{
    for(pthread_t& p : producers)
    {
        pthread_join(p, NULL);
    }
    for(pthread_t& c : consumers)
    {
        pthread_join(c, NULL);
    }
}

