#pragma once
#include <ui/core/node.hpp>
#include <engine/renderer.hpp>
#include <tree.hh>

struct Menu
{
    using NodeTree = tr::tree<std::unique_ptr<UINode>>;
    using NodeIterator = NodeTree::iterator;

    NodeTree elements {};

    void Draw(Renderer& renderer, const UICursorInfo& cursor_params);
    void DrawElement(Renderer& renderer, NodeIterator element, const UIDrawInfo& parent_info, const UICursorInfo& cursor_params);
};