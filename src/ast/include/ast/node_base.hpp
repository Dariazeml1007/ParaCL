#pragma once

#include "ast/inode.hpp"
#include "visitor/ast_visitor.hpp"

namespace language
{

template <typename Derived> class NodeBase : public INode
{
public:
    void accept(ASTVisitor* visitor) override
    {
        visitor->visit(static_cast<Derived*>(this));
    }
};

} // namespace language
