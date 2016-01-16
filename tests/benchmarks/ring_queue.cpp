//
// Created by Ivan Shynkarenka on 16.01.2016.
//

#include "cppbenchmark.h"

#include <functional>
#include <thread>

#include "threads/ring_queue.h"

const int items_to_produce = 100000000;

template<typename T, size_t N>
void produce_consume(CppBenchmark::Context& context, const std::function<void()>& wait_strategy)
{
    CppCommon::RingQueue<T, N> queue;

    // Start consumer thread
    auto consumer = std::thread([&queue, &wait_strategy]()
    {
        for (int i = 0; i < items_to_produce; ++i)
        {
            // Dequeue with the given wait strategy
            T item;
			while (!queue.Dequeue(item))
                wait_strategy();
        }
    });

    // Start producer thread
    auto producer = std::thread([&queue, &wait_strategy, &consumer]()
    {
        for (int i = 0; i < items_to_produce; ++i)
        {
            // Enqueue with the given wait strategy
            T item = (T)i;
			while (!queue.Enqueue(item))
                wait_strategy();
        }

        // Wait for the consumer thread
        consumer.join();
    });

    // Wait for the producer thread
    producer.join();

    // Update benchmark metrics
    context.metrics().AddItems(items_to_produce);
    context.metrics().AddBytes(items_to_produce * sizeof(T));
}

BENCHMARK("RingQueue-SpinWait")
{
    produce_consume<int, 1048576>(context, []{});
}

BENCHMARK("RingQueue-YieldWait")
{
    produce_consume<int, 1048576>(context, []{ std::this_thread::yield(); });
}

BENCHMARK_MAIN()
