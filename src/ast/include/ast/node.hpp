#pragma once

#include "fwd.hpp"
#include "inode.hpp"
#include "node_base.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace language
{

class Number : public NodeBase<Number>
{
private:
    int value_;

public:
    Number(int value) : value_(value)
    {
    }
    void evaluate(Interpreter& interp) override;
    int get_value() const
    {
        return value_;
    }
};

class Variable : public NodeBase<Variable>
{
private:
    std::string name_;

public:
    Variable(std::string_view name) : name_(name)
    {
    }
    void evaluate(Interpreter& interp) override;
    const std::string& get_name() const
    {
        return name_;
    }
};

class BinaryOp : public NodeBase<BinaryOp>
{
public:
    enum class Op
    {
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        OR,
        AND,
        EQ,
        NE,
        L,
        G,
        LE,
        GE
    };

private:
    Op op_;
    INode* left_;
    INode* right_;

public:
    BinaryOp(Op op, INode* left, INode* right)
        : op_(op), left_(left), right_(right)
    {
    }
    void evaluate(Interpreter& interp) override;
    Op get_op() const
    {
        return op_;
    }
    INode* get_left() const
    {
        return left_;
    }
    INode* get_right() const
    {
        return right_;
    }
};

class UnaryOp : public NodeBase<UnaryOp>
{
public:
    enum class Op
    {
        UMINUS,
        NOT
    };

private:
    Op op_;
    INode* expr_;

public:
    UnaryOp(Op op, INode* expr) : op_(op), expr_(expr)
    {
    }
    void evaluate(Interpreter& interp) override;
    Op get_op() const
    {
        return op_;
    }
    INode* get_expr() const
    {
        return expr_;
    }
};

class Assignment : public NodeBase<Assignment>
{
private:
    std::string var_name_;
    INode* expr_;

public:
    Assignment(std::string_view var_name, INode* expr)
        : var_name_(var_name), expr_(expr)
    {
    }
    void evaluate(Interpreter& interp) override;
    const std::string& get_var_name() const
    {
        return var_name_;
    }
    INode* get_expr() const
    {
        return expr_;
    }
};

class ExpressionStmt : public NodeBase<ExpressionStmt>
{
private:
    INode* expr_;

public:
    ExpressionStmt(INode* expr) : expr_(expr)
    {
    }

    void evaluate(Interpreter& interp) override;

    INode* get_expr() const
    {
        return expr_;
    }
};

class PrintStmt : public NodeBase<PrintStmt>
{
private:
    INode* expr_;

public:
    PrintStmt(INode* expr) : expr_(expr)
    {
    }
    void evaluate(Interpreter& interp) override;
    INode* get_expr() const
    {
        return expr_;
    }
};

class ScanfExpr : public NodeBase<ScanfExpr>
{
public:
    ScanfExpr() = default;
    void evaluate(Interpreter& interp) override;
};

class IfStmt : public NodeBase<IfStmt>
{
private:
    INode* condition_;
    INode* body_if_;
    INode* body_else_;

public:
    IfStmt(INode* condition, INode* body_if, INode* body_else = nullptr)
        : condition_(condition), body_if_(body_if), body_else_(body_else)
    {
    }
    void evaluate(Interpreter& interp) override;
    INode* get_condition() const
    {
        return condition_;
    }
    INode* get_body_if() const
    {
        return body_if_;
    }
    INode* get_body_else() const
    {
        return body_else_;
    }
};

class WhileStmt : public NodeBase<WhileStmt>
{
private:
    INode* condition_;
    INode* body_;

public:
    WhileStmt(INode* condition, INode* body)
        : condition_(condition), body_(body)
    {
    }
    void evaluate(Interpreter& interp) override;
    INode* get_condition() const
    {
        return condition_;
    }
    INode* get_body() const
    {
        return body_;
    }
};

class BlockStmt : public NodeBase<BlockStmt>
{
private:
    std::vector<INode*> statements_;

public:
    BlockStmt() = default;
    void add_statement(INode* stmt)
    {
        statements_.push_back(stmt);
    }
    void evaluate(Interpreter& interp) override;
    size_t get_statement_count() const
    {
        return statements_.size();
    }
    INode* get_statement(size_t i) const
    {
        return statements_[i];
    }
};

} // namespace language
