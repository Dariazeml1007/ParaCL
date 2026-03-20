#pragma once

#include "ast/fwd.hpp"
#include "error/error.hpp"
#include "visitor/visitor_fwd.hpp"

namespace language
{
class Interpreter;

class INode
{
private:
    SourceRange range_;

public:
    INode() = default;
    virtual ~INode() = default;

    INode(const INode&) = delete;
    INode& operator=(const INode&) = delete;
    INode(INode&&) = delete;
    INode& operator=(INode&&) = delete;

    virtual void evaluate(Interpreter& interp) = 0;
    virtual void accept(ASTVisitor* visitor) = 0;

    void set_range(SourceRange r)
    {
        range_ = std::move(r);
    }

    const SourceRange& range() const
    {
        return range_;
    }
};

} // namespace language
