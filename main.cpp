#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstddef>
#include <string>
#include <vector>
#include "buffor.hpp"
#include "RandomGenerator.hpp"
#include "ProducerCustomer.hpp"
using namespace std;

size_t low;
size_t high;

void readInput(size_t& buffSize, size_t& producerNumber, size_t& consumerNumber);
void initilizeThreads(vector<pthread_t>& producers, vector<pthread_t>& consumers, BufferMonitor& monitor);
void joinAll(vector<pthread_t>& producers, vector<pthread_t>& consumers);


void *producent(void *prod)
{   
    Producer* producer = (Producer*) prod;
    while(true)
        producer->useBuffer();

    delete producer;
}
void *consument(void *cons)
{   
    Consumer* consumer = (Consumer*) cons;
    while(true)
        consumer->useBuffer();

    delete consumer;
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
    int id = 0;
    for(pthread_t& p : producers)
    {
        Producer* prod = new Producer(id, monitor, low, high); 
        pthread_create(&p, NULL, producent, (void *)(prod));
        ++id;
    }
    id = 0;
    for(pthread_t& c : consumers)
    {
        Consumer* prod = new Consumer(id, monitor, low, high); 
        pthread_create(&c, NULL, consument, (void *)(prod));
        ++id;
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

