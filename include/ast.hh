#ifndef _AST_H
#define _AST_H

#include <iostream>
#include <string>
#include <variant>

#define DEBUG true

struct Token
{
    Token(int typ) : type(typ) {};
    int type;
    std::variant<int, char, std::string, float, double> val;
};

enum
{
    WHILE = 100,
    ELSE,
    IF,
    DO,
    FOR,
    RETURN,
    SWITCH,
    CASE,
    BREAK,
    DEFAULT,
    CONTINUE,
    TRY,
    CATCH,
    TRUE,
    FALSE,
    ID,
    RELOP,
    FLOATNUMBER,
    ASSIGN,
    LT,
    EQ,
    LE,
    GT,
    GE,
    NE,
    ENDTOKEN,
    LP, // left parenthesis
    RP, // right parenthesis
    LB, // left bracket
    RB, // right bracket
    PUNCT,
    MINUS,
    PLUS,
    MULTIPLY,
    DIVIDE,
    MINUSEQUAL,
    PLUSEQUAL,
    MULTIPLYEQUAL,
    DIVIDEEQUAL,
    ERRORTOKEN,
    ENDMARKER,
    ASSIGN_DELIM, // :
    DOUBLEQUOTE,
    SINGLEQUOTE,
    OPERATOR,
    INC,
    DEC,
    LOGICAL_AND,
    LOGICAL_OR,
    LSHIFT,
    RSHIFT,
    CONDITION,
    // Types
    NUMBER,
    FLOAT,
    FUNC,
    RETURN_TYPE,
    VOID,
    BOOL,
    CHAR,
    SHORT,
    INT,
    LONG,
    LLONG,
    DOUBLE,
    LDOUBLE,
    ARRAY,
    ENUM,
    PTR,
    STRUCT,
};

class Visitor;

class ASTNode
{
public:
    ASTNode(Token *t) : tok(t) {
        left = right = parent = nullptr;
    };
    virtual void accept(Visitor *v) = 0;
    virtual void print() = 0;
    ASTNode *get_left() { return left; };
    void set_left(ASTNode *n) { left = n; };
    ASTNode *get_right() { return right; };
    void set_right(ASTNode *n) { right = n; };
    ASTNode *get_parent() { return parent; };
    void set_parent(ASTNode *n) { parent = n; };
    Token *retrieve_token() { return tok; };
    void set_token(Token *t) { tok = t; };

private:
    Token *tok;
    ASTNode *left, *right, *parent;
};

std::string get_type(int name);
void token_value(Token *t);
void build_tree(ASTNode *root, ASTNode *node);

class EXPRNode;
class ConditionNode;
class ASSIGNNode;
class RETURNNode;
class BodyNode;
class IFNode;
class WHILENode;
class FORNode;

void build_expr(ASTNode *root, Token *tok);
IFNode *build_if(ASTNode *root, ASTNode *node);
ASSIGNNode *build_assign(ASTNode *root, ASTNode *node);
ConditionNode *build_condition(ASTNode *root, ASTNode *node);
RETURNNode *build_return(ASTNode *root, ASTNode *node);
WHILENode *build_while(ASTNode *root, ASTNode *node);
FORNode *build_for(ASTNode *root, ASTNode *node);

class Visitor
{
public:
    virtual void walk(EXPRNode *node) = 0;
    virtual void walk(ConditionNode *node) = 0;
    virtual void walk(ASSIGNNode *node) = 0;
    virtual void walk(RETURNNode *node) = 0;
    virtual void walk(BodyNode *node) = 0;
    virtual void walk(IFNode *node) = 0;
    virtual void walk(WHILENode *node) = 0;
    virtual void walk(FORNode *node) = 0;
};

class EXPRNode : public ASTNode
{
public:
    EXPRNode(Token *tok) : ASTNode(tok) {};
    void accept(Visitor *visit) override { visit->walk(this); };
    void traverse(ASTNode *root) {
        if (root != nullptr) {
            traverse(root->get_left());
            token_value(root->retrieve_token());
            traverse(root->get_right());
        }
    };
    void print() override
    {
        std::cout << "Expression: ";
        traverse(this);
    };
};

class ConditionNode : public ASTNode
{
public:
    ConditionNode(Token *tok) : ASTNode(tok) {
        left_var = right_var = nullptr;
    };
    void accept(Visitor *visit) override { visit->walk(this); };
    void print() override
    {
        std::cout << "ConditionExpression: ";
        left_var->print();
        std::cout << " " << get_type(cond->type) << " ";
        right_var->print();
        std::cout << "\n";
    };

private:
    Token *cond;
    EXPRNode *left_var, *right_var;
};

class ASSIGNNode : public ASTNode
{
public:
    // ASSIGNNode(Token *tok, Token *val_) : name(tok), val(val_) {};
    void accept(Visitor *visit) override { visit->walk(this); };
    void print() override
    {
        std::cout << "Assignment: ";
        token_value(token_typ);
        std::cout << " ";
        token_value(name);
        std::cout << " ";
        val->print();
        std::cout << "\n";
    };

private:
    Token *token_typ, *name;
    EXPRNode *val;
};

class RETURNNode : public ASTNode
{
public:
    // RETURNNode(Token *tok) : val(tok) {};
    void accept(Visitor *visit) override { visit->walk(this); };
    void print() override
    {
        std::cout << "Return: ";
        val->print();
        std::cout << "\n";
    };

private:
    EXPRNode *val;
};

class BodyNode : public ASTNode
{
public:
    void accept(Visitor *visit) override { visit->walk(this); };
    void traverse(ASTNode *n) {
        if (n!=nullptr) {
            n->print();
            traverse(n->get_left());
            traverse(n->get_right());
        }
    };
    void print() override{
        std::cout << "Body: ";
        traverse(this);
    };
private:
    ASTNode *node;
};

class IFNode : public ASTNode
{
public:
    // IFNode(ConditionNode *condition, BodyNode *bdy) : cond(condition), body(bdy) {};
    void accept(Visitor *visit) override { visit->walk(this); };
    void print() override { cond->print(); body->print(); };
private:
    ConditionNode *cond;
    BodyNode *body;
};

class WHILENode : public ASTNode
{
public:
    // WHILENode(ConditionNode *condition, BodyNode *bdy) : cond(condition), body(bdy) {};
    void accept(Visitor *visit) override { visit->walk(this); };
    void print() override { cond->print(); body->print(); };
private:
    ConditionNode *cond;
    BodyNode *body;
};

class FORNode : public ASTNode
{
public:
    // FORNode(ASSIGNNode *ass, ConditionNode *condition, BodyNode *bdy) : assign(ass), cond(condition), body(bdy) {};
    void accept(Visitor *visit) override { visit->walk(this); };
    void print() override
    {
        assign->print();
        cond->print();
        body->print();
    };

private:
    ASSIGNNode *assign;
    ConditionNode *cond;
    BodyNode *body;
};

class BuildNode : public Visitor
{
public:
    void walk(ASSIGNNode *node) override;
    void walk(BodyNode *node) override;
    void walk(ConditionNode *node) override;
    void walk(FORNode *node) override;
    void walk(WHILENode *node) override;
    void walk(IFNode *node) override;
    void walk(RETURNNode *node) override;
    void walk(EXPRNode *node) override;
};

class WalkNode : public Visitor
{
public:
    void walk(ASSIGNNode *node) override;
    void walk(BodyNode *node) override;
    void walk(ConditionNode *node) override;
    void walk(FORNode *node) override;
    void walk(WHILENode *node) override;
    void walk(IFNode *node) override;
    void walk(RETURNNode *node) override;
    void walk(EXPRNode *node) override;
};
#endif
