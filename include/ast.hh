#ifndef _AST_H
#define _AST_H

#include <iostream>
#include <string>
#include <variant>

#define DEBUG true

struct Token
{
    Token(int typ) : type(typ){};
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
    EXPRESSION,
    BODY,
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
    ASTNode(int typ) : type(typ)
    {
        left = right = parent = nullptr;
    };
    ASTNode(const ASTNode &);
    virtual ~ASTNode();
    ASTNode &operator=(const ASTNode &);
    virtual void accept(Visitor *v){};
    virtual void print(){};
    ASTNode *get_left() { return left; };
    void set_left(ASTNode *n) { left = n; };
    ASTNode *get_right() { return right; };
    void set_right(ASTNode *n) { right = n; };
    ASTNode *get_parent() { return parent; };
    void set_parent(ASTNode *n) { parent = n; };
    int get_type() { return type; };
    void set_type(int typ) { type = typ; };

private:
    int type;
    ASTNode *left, *right, *parent;
};

std::string get_string_type(int name);
void token_value(Token *t);
void build_ast(ASTNode *root, ASTNode *node);
void walk_tree(ASTNode *tree);

class EXPRNode;
class ConditionNode;
class ASSIGNNode;
class RETURNNode;
class BodyNode;
class IFNode;
class WHILENode;
class FORNode;

void build_expr(EXPRNode *root, Token *tok);
void build_if(IFNode *root, ConditionNode *node, BodyNode *body);
void build_assign(ASSIGNNode *root, Token *name, EXPRNode *expr);
void build_condition(ConditionNode *root, Token *tok, EXPRNode *lhs, EXPRNode *rhs);
void build_return(RETURNNode *root, ASTNode *node);
void build_while(WHILENode *root, ConditionNode *node, BodyNode *body);
void build_for(FORNode *root, ASSIGNNode *assign, ConditionNode *node, BodyNode *body);
void build_body(BodyNode *root, ASTNode *node);

class Visitor
{
public:
    virtual ~Visitor(){};
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
    EXPRNode() : ASTNode(EXPRESSION)
    {
        left_node = right_node = parent_node = nullptr;
    };
    EXPRNode(Token *t) : ASTNode(EXPRESSION), tok(t)
    {
        left_node = right_node = parent_node = nullptr;
    };
    virtual ~EXPRNode();
    void accept(Visitor *visit) override { visit->walk(this); };
    void set_token(Token *t) { tok = t; };
    Token *get_token() { return tok; };
    EXPRNode *get_left_node() { return left_node; };
    void set_left_node(EXPRNode *n) { left_node = n; };
    EXPRNode *get_right_node() { return right_node; };
    void set_right_node(EXPRNode *n) { right_node = n; };
    EXPRNode *get_parent_node() { return parent_node; };
    void set_parent_node(EXPRNode *n) { parent_node = n; };
    void traverse(EXPRNode *root)
    {
        if (root != nullptr)
        {
            traverse(root->left_node);
            token_value(root->tok);
            std::cout << " ";
            traverse(root->right_node);
        }
    };
    void print() override
    {
        traverse(this);
    };

private:
    Token *tok;
    EXPRNode *left_node, *right_node, *parent_node;
};

class ConditionNode : public ASTNode
{
public:
    ConditionNode() : ASTNode(CONDITION)
    {
        left_var = right_var = nullptr;
    };
    ConditionNode(Token *t) : ASTNode(CONDITION), cond(t)
    {
        left_var = right_var = nullptr;
    };
    ConditionNode(Token *t, EXPRNode *lhs, EXPRNode *rhs) : ASTNode(CONDITION), cond(t), left_var(lhs), right_var(rhs) {}
    virtual ~ConditionNode(){};
    void set_cond(Token *c) { cond = c; };
    void set_lhs_expr(EXPRNode *expr) { left_var = expr; };
    void set_rhs_expr(EXPRNode *expr) { right_var = expr; };
    void accept(Visitor *visit) override { visit->walk(this); };
    void print() override
    {
        left_var->print();
        std::cout << " " << get_string_type(cond->type) << " ";
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
    ASSIGNNode() : ASTNode(ASSIGN)
    {
        expr = nullptr;
    };
    ASSIGNNode(Token *tok) : ASTNode(ASSIGN), name(tok)
    {
        expr = nullptr;
    };
    ASSIGNNode(Token *tok, EXPRNode *val) : ASTNode(ASSIGN), name(tok), expr(val){};
    virtual ~ASSIGNNode()
    {
        if (expr != nullptr)
            delete expr;
    };
    void set_name(Token *tok) { name = tok; };
    void set_expr(EXPRNode *node) { expr = node; };
    void accept(Visitor *visit) override { visit->walk(this); };
    void print() override
    {
        token_value(name);
        std::cout << " = ";
        expr->print();
        std::cout << "\n";
    };

private:
    Token *name;
    EXPRNode *expr;
};

class RETURNNode : public ASTNode
{
public:
    RETURNNode() : ASTNode(RETURN) { expr = nullptr; };
    RETURNNode(EXPRNode *val) : ASTNode(RETURN), expr(val){};
    virtual ~RETURNNode(){ if (expr != nullptr) delete expr; };
    void accept(Visitor *visit) override { visit->walk(this); };

    void print() override
    {
        expr->print();
        std::cout << "\n";
    };

private:
    EXPRNode *expr;
};

class BodyNode : public ASTNode
{
public:
    BodyNode() : ASTNode(BODY){ node = nullptr; };
    BodyNode(ASTNode *nde) : ASTNode(BODY), node(nde) {};
    void accept(Visitor *visit) override { visit->walk(this); };
    void traverse(ASTNode *n)
    {
        if (n != nullptr)
        {
            n->print();
            traverse(n->get_left());
            traverse(n->get_right());
        }
    };
    void print() override
    {
        traverse(this);
    };

private:
    ASTNode *node;
};

class IFNode : public ASTNode
{
public:
    IFNode() : ASTNode(IF){};
    IFNode(ConditionNode *condition, BodyNode *bdy) : ASTNode(IF), cond(condition), body(bdy){};
    void set_cond(ConditionNode *c) { cond = c; };
    void set_body(BodyNode *b) { body = b; };
    void accept(Visitor *visit) override { visit->walk(this); };
    void print() override
    {
        cond->print();
        body->print();
    };

private:
    ConditionNode *cond;
    BodyNode *body;
};

class WHILENode : public ASTNode
{
public:
    WHILENode() : ASTNode(IF){};
    WHILENode(ConditionNode *condition, BodyNode *bdy) : ASTNode(IF), cond(condition), body(bdy){};
    void set_cond(ConditionNode *c) { cond = c; };
    void set_body(BodyNode *b) { body = b; };
    void accept(Visitor *visit) override { visit->walk(this); };
    void print() override
    {
        cond->print();
        body->print();
    };

private:
    ConditionNode *cond;
    BodyNode *body;
};

class FORNode : public ASTNode
{
public:
    FORNode() : ASTNode(IF){
        assign = nullptr;
        cond = nullptr;
        body = nullptr;
    };
    FORNode(ASSIGNNode *ass, ConditionNode *condition, BodyNode *bdy) : ASTNode(IF), assign(ass), cond(condition), body(bdy){};
    void set_cond(ConditionNode *c) { cond = c; };
    void set_body(BodyNode *b) { body = b; };
    void set_assign(ASSIGNNode *ass) { assign = ass; };
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

class WalkNode : public Visitor
{
public:
    virtual ~WalkNode(){};
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
