#pragma once
#include <stack>
#include <memory>

class Application;
class MenuStack;

class MenuInterface
{
public:
    virtual ~MenuInterface() = default;
    virtual void UpdateMenu(Application& application) = 0;
};

class MenuStack
{
public:
    void UpdateTop(Application& application)
    {
        menu_stack.top()->UpdateMenu(application);
    }

    void Push(std::unique_ptr<MenuInterface>&& menu)
    {
        menu_stack.emplace(std::move(menu));
    }
    void Pop()
    {
        menu_stack.pop();
    }

    void Clear()
    {
        menu_stack = {};
    }

    bool Empty()
    {
        return menu_stack.empty();
    }

private:
    std::stack<std::unique_ptr<MenuInterface>> menu_stack {};
};