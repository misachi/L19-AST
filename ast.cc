#include "include/ast.hh"

#include <iostream>
#include <variant>

static bool isnumber(Token *tok)
{
    return tok->type == NUMBER || tok->type == FLOAT || tok->type == INT || tok->type == DOUBLE;
}

void token_value(Token *t)
{
    std::visit([](auto &&arg) { std::cout << arg; }, t->val);
}
void WalkNode::walk(ASSIGNNode *node)
{
#if DEBUG
    std::cout << "Assignment: ";
    node->print();
#endif
}

void WalkNode::walk(ConditionNode *node)
{
#if DEBUG
    std::cout << "ConditionExpression: ";
    node->print();
#endif
}

void WalkNode::walk(FORNode *node)
{
#if DEBUG
    node->print();
#endif
}

void WalkNode::walk(WHILENode *node)
{
#if DEBUG
    node->print();
#endif
}

void WalkNode::walk(IFNode *node)
{
#if DEBUG
    node->print();
#endif
}

void WalkNode::walk(RETURNNode *node)
{
#if DEBUG
    std::cout << "Return: ";
    node->print();
#endif
}

void WalkNode::walk(EXPRNode *node)
{
#if DEBUG
    std::cout << "Expression: ";
    node->print();
    std::cout << "\n";
#endif
}

void WalkNode::walk(BodyNode *node)
{
#if DEBUG
    std::cout << "Body: ";
    node->print();
#endif
}

std::string get_string_type(int name)
{
    switch (name)
    {
    case WHILE:
        return "WHILE";
    case ELSE:
        return "ELSE";
    case IF:
        return "IF";
    case DO:
        return "DO";
    case RETURN:
        return "RETURN";
    case BREAK:
        return "BREAK";
    case DEFAULT:
        return "DEFAULT";
    case CONTINUE:
        return "CONTINUE";
    case TRY:
        return "TRY";
    case CATCH:
        return "CATCH";
    case TRUE:
        return "TRUE";
    case FALSE:
        return "FALSE";
    case LP:
        return "LP";
    case RP:
        return "RP";
    case LB:
        return "LB";
    case RB:
        return "RB";
    case ENDMARKER:
        return "ENDMARKER";
    case LT:
        return "LT";
    case EQ:
        return "EQ";
    case LE:
        return "LE";
    case GT:
        return "GT";
    case GE:
        return "GE";
    case NE:
        return "NE";
    case MINUS:
        return "MINUS";
    case PLUS:
        return "PLUS";
    case MULTIPLY:
        return "MULTIPLY";
    case MINUSEQUAL:
        return "MINUSEQUAL";
    case PLUSEQUAL:
        return "PLUSEQUAL";
    case MULTIPLYEQUAL:
        return "MULTIPLYEQUAL";
    case DIVIDEEQUAL:
        return "DIVIDEEQUAL";
    case DIVIDE:
        return "DIVIDE";
    case INC:
        return "INCR";
    case DEC:
        return "DECR";
    case LOGICAL_AND:
        return "LOGICAL_AND";
    case LOGICAL_OR:
        return "LOGICAL_OR";
    case LSHIFT:
        return "LSHIFT";
    case RSHIFT:
        return "RSHIFT";
    case NUMBER:
        return "NUMBER";
    case FLOAT:
        return "FLOAT";
    case FOR:
        return "FOR";
    case ID:
        return "ID";
    case ASSIGN:
        return "ASSIGN";
    case FUNC:
        return "FUNC";
    case RETURN_TYPE:
        return "RETURN_TYPE";
    default:
        return "ERRORTOKEN";
    }
}

ASTNode::ASTNode(const ASTNode &node)
{
    left = node.left;
    right = node.right;
    type = node.type;
}

ASTNode &ASTNode::operator=(const ASTNode &node)
{
    left = node.left;
    right = node.right;
    type = node.type;
    return *this;
}

ASTNode::~ASTNode()
{
    if (left != nullptr)
        delete left;
    if (right != nullptr)
        delete right;
}

EXPRNode::~EXPRNode()
{
    if (left_node != nullptr)
        delete left_node;
    if (right_node != nullptr)
        delete right_node;
}

void build_ast(ASTNode *Troot, ASTNode *node)
{
    ASTNode *root = Troot;
    ASTNode *temp = nullptr;

    while (root != nullptr)
    {
        temp = root;
        if (root->get_left() == nullptr)
        {
            root = root->get_left();
        }
        else
        {
            root = root->get_right();
        }
    }

    node->set_parent(temp);
    if (temp == nullptr)
    {
        Troot = node;
    }
    else if (temp->get_left() == nullptr)
    {
        temp->set_left(node);
    }
    else
    {
        temp->set_right(node);
    }
}

void build(ASTNode *Troot, ASTNode *temp, ASTNode *expr)
{
    if (temp == nullptr)
    {
        Troot = expr;
    }
    else if (temp->get_left() == nullptr)
    {
        temp->set_left(expr);
    }
    else
    {
        temp->set_right(expr);
    }
}

void build_expr(EXPRNode *Troot, Token *tok)
{
    EXPRNode *expr = new EXPRNode(tok);
    auto *root = Troot;
    EXPRNode *temp = nullptr;

    while (root != nullptr)
    {
        temp = root;
        if (root->get_left_node() == nullptr)
        {
            root = root->get_left_node();
        }
        else
        {
            root = root->get_right_node();
        }
    }

    if (temp == nullptr)
    {
        root = expr;
    }
    else if (temp->get_left_node() == nullptr)
    {
        if (isnumber(temp->get_token()))
        {
            auto val = temp->get_token();
            temp->set_token(expr->get_token());
            expr->set_token(val);
        }
        expr->set_parent_node(temp);
        temp->set_left_node(expr);
    }
    else
    {
        if (isnumber(temp->get_token()))
        {
            auto val = temp->get_token();
            temp->set_token(expr->get_token());
            expr->set_token(val);
        }
        expr->set_parent_node(temp);
        temp->set_right_node(expr);
    }
}

void build_condition(ConditionNode *Troot, Token *tok, EXPRNode *lhs, EXPRNode *rhs)
{
    Troot->set_cond(tok);
    Troot->set_lhs_expr(lhs);
    Troot->set_rhs_expr(rhs);
}

void build_body(BodyNode *root, ASTNode *node)
{
    build_ast(root, node);
}

void build_if(IFNode *root, ConditionNode *node, BodyNode *body)
{
    root->set_cond(node);
    root->set_body(body);
}

void build_while(WHILENode *root, ConditionNode *node, BodyNode *body)
{
    root->set_cond(node);
    root->set_body(body);
}

void build_for(FORNode *root, ASSIGNNode *assign, ConditionNode *node, BodyNode *body)
{
    root->set_assign(assign);
    root->set_cond(node);
    root->set_body(body);
}

void build_assign(ASSIGNNode *root, Token *name, EXPRNode *expr)
{
    root->set_name(name);
    root->set_expr(expr);
}

void walk_tree(ASTNode *tree)
{
    if (tree != nullptr)
    {
        WalkNode *walknode = new WalkNode{};
        switch (tree->get_type())
        {
        case EXPRESSION:
            dynamic_cast<EXPRNode *>(tree)->accept(walknode);
            break;
        case CONDITION:
            dynamic_cast<ConditionNode *>(tree)->accept(walknode);
            break;
        case ASSIGN:
            dynamic_cast<ASSIGNNode *>(tree)->accept(walknode);
            break;
        case RETURN:
            dynamic_cast<RETURNNode *>(tree)->accept(walknode);
            break;
        case IF:
            dynamic_cast<IFNode *>(tree)->accept(walknode);
            break;
        case WHILE:
            dynamic_cast<WHILENode *>(tree)->accept(walknode);
            break;
        case FOR:
            dynamic_cast<FORNode *>(tree)->accept(walknode);
            break;
        default:
            break;
        }
        delete walknode;
        walk_tree(tree->get_left());
        walk_tree(tree->get_right());
    }
}
