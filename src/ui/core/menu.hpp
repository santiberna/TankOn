#pragma once
#include <ui/core/node.hpp>
#include <engine/renderer.hpp>
#include <tree.hh>

class Menu
{
public:
    using NodeTree = tr::tree<std::unique_ptr<UINode>>;
    using NodeIterator = NodeTree::iterator;

    void Draw(Renderer& renderer, const UICursorInfo& cursor_params);

    NodeIterator AddChildNode(NodeIterator parent, std::unique_ptr<UINode>&& node);
    NodeIterator AddRootNode(std::unique_ptr<UINode>&& node);

private:
    void DrawElement(Renderer& renderer, NodeIterator element, const UIDrawInfo& parent_info, const UICursorInfo& cursor_params);
    NodeTree elements {};
};