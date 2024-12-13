#pragma once
#include <string>
#include <vector>
#include <optional>

struct CommandArgs
{
    std::vector<std::string> arguments {};

    CommandArgs(int argc, char* args[])
    {
        for (int i = 0; i < argc; i++)
        {
            arguments.emplace_back(args[i]);
        }
    }

    std::optional<std::string> GetPositionalArg(size_t index)
    {
        if (index >= arguments.size())
            return std::nullopt;
        return arguments.at(index);
    }
};