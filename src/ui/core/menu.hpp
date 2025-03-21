#pragma once
#include <ui/core/node.hpp>
#include <engine/renderer.hpp>
#include <tree.hh>

struct Menu
{
    using ElementTree = tr::tree<std::unique_ptr<UINode>>;
    using ElementIterator = ElementTree::iterator;

    ElementTree elements {};

    void RenderCanvas(Renderer& renderer, const UICursorInfo& cursor_params);
    void DrawElement(Renderer& renderer, ElementIterator element, const UIDrawInfo& parent_info, const UICursorInfo& cursor_params);
};