#pragma once
#include <future>

class WorkerContext;

namespace detail
{

class CallableInterface
{
public:
    CallableInterface() = default;
    virtual ~CallableInterface() = default;
    virtual void Call(WorkerContext& context) = 0;
};

template <typename Ret>
class BasicTaskImpl : public CallableInterface
{
public:
    BasicTaskImpl(std::packaged_task<Ret(WorkerContext&)>&& task)
        : task(std::move(task)) { };

    virtual ~BasicTaskImpl() = default;

    void Call(WorkerContext& context) override { task(context); };

private:
    std::packaged_task<Ret(WorkerContext&)> task;
};

}

class Task
{
public:
    Task() = default;
    Task(std::unique_ptr<detail::CallableInterface>&& callable)
        : _callable(std::move(callable))
    {
    }
    void Run(WorkerContext& context) { _callable->Call(context); }
    bool Valid() const { return _callable != nullptr; }

private:
    std::unique_ptr<detail::CallableInterface> _callable {};
};