#include <string>
#include <thread/thread_pool.hpp>

ThreadPool::ThreadPool(uint32_t thread_count)
{
    for (uint32_t i = 0; i < thread_count; i++)
    {
        threads.emplace_back(std::thread(WorkerMain, this, i));
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::scoped_lock<std::mutex> lock { mutex };
        kill = true;
    }

    producer_notify.notify_all();

    for (auto& thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

void ThreadPool::Start()
{
    {
        std::scoped_lock<std::mutex> lock { mutex };
        running = true;
    }
    producer_notify.notify_all();
}

void ThreadPool::CancelAll()
{
    {
        std::scoped_lock<std::mutex> lock { mutex };
        tasks = {};
        running = false;
    }
    producer_notify.notify_all();
    consumer_notify.notify_all();
}

void ThreadPool::Flush()
{
    if (!running)
        return;

    std::unique_lock<std::mutex> lock { mutex };
    consumer_notify.wait(lock, [this]()
        { return tasks.empty(); });
}

void ThreadPool::WorkerMain(ThreadPool* pool, uint32_t ID)
{
    WorkerContext context {};
    context.id = ID;
    context.pool = pool;

    while (true)
    {
        Task my_task {};

        {
            // wait for a notify to start or kill the thread

            std::unique_lock<std::mutex> lock(pool->mutex);
            pool->producer_notify.wait(lock, [pool]()
                { return pool->kill == true || (pool->tasks.size() > 0 && pool->running == true); });

            // After a wait, the lock is acquired

            if (pool->kill == true)
            {
                return;
            }

            my_task = pool->GetTask();
        }

        if (my_task.Valid())
            my_task.Run(context);

        pool->consumer_notify.notify_all();
    }
}