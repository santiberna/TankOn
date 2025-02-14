#pragma once
#include <thread/task.hpp>

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

class ThreadPool
{
public:
    ThreadPool(uint32_t threadCount);
    ~ThreadPool();

    template <typename Functor>
    auto QueueWork(Functor&& f)
    {
        using Ret = std::invoke_result_t<Functor, WorkerContext&>;
        auto packaged = std::packaged_task<Ret(WorkerContext&)>(std::forward<Functor>(f));
        auto future = packaged.get_future();
        {
            std::scoped_lock<std::mutex> lock { mutex };
            Task task = { std::make_unique<detail::BasicTaskImpl<Ret>>(std::move(packaged)) };
            tasks.emplace(std::move(task));
        }

        producer_notify.notify_one();
        consumer_notify.notify_all();
        return future;
    }

    // Starts the thread pool, making workers continuously consume work until FinishWork() or Cancel() is called
    void Start();

    // Stops the threadpool from running any additional jobs and clears the queue beyond the ones that are already running
    // Start() must be called again to queue and run any jobs added afterwards
    void CancelAll();

    // Blocks the calling thread until all work in the queue is complete
    // WARN: any futures that are cancelled will throw if accessed
    void Flush();

private:
    friend WorkerContext;

    Task GetTask()
    {
        auto my_task = std::move(tasks.front());
        tasks.pop();
        return my_task;
    }

    static void WorkerMain(ThreadPool* pool, uint32_t ID);

    std::mutex mutex;
    std::condition_variable producer_notify;
    std::condition_variable consumer_notify;
    std::vector<std::thread> threads {};
    std::queue<Task> tasks;
    bool running = false;
    bool kill = false;
};

class WorkerContext
{
public:
    static constexpr std::chrono::milliseconds NO_WAIT { 0 };
    static constexpr uint32_t MAX_RECURSE { 16 };

    template <typename Functor>
    auto QueueWork(Functor&& f)
    {
        return pool->QueueWork(std::forward<Functor>(f));
    }

    // Makes the worker work on other tasks until the future is read
    template <typename T>
    T WaitForFuture(std::future<T>& future)
    {
        while (true)
        {
            Task next_task {};
            {
                std::unique_lock<std::mutex> lock(pool->mutex);
                pool->producer_notify.wait(lock, [this, &future]()
                    { return future.wait_for(NO_WAIT) == std::future_status::ready || (pool->tasks.size() > 0 && recurse_count < MAX_RECURSE); });

                if (future.wait_for(NO_WAIT) == std::future_status::ready)
                {
                    return future.get();
                }

                next_task = pool->GetTask();
            }

            if (next_task.Valid())
            {
                recurse_count++;
                next_task.Run(*this);
                recurse_count--;
            }
        }
    }

private:
    friend ThreadPool;

    ThreadPool* pool {};
    uint32_t id {};
    uint32_t recurse_count = 0;
};