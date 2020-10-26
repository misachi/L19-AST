#include "include/ast.hh"

#include <iostream>

static int isnumber(Token *tok)
{
    return tok->type == NUMBER || tok->type == FLOAT || tok->type == INT || tok->type == DOUBLE;
}

void WalkNode::walk(ASSIGNNode *node)
{
#if DEBUG
    node->print();
#endif
}

void WalkNode::walk(ConditionNode *node)
{
#if DEBUG
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
    node->print();
#endif
}

void WalkNode::walk(EXPRNode *node)
{
#if DEBUG
    node->print();
#endif
}

void WalkNode::walk(BodyNode *node)
{
#if DEBUG
    node->print();
#endif
}

std::string get_type(int name)
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

void build_tree(ASTNode *Troot, ASTNode *node)
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

void build(ASTNode *Troot, ASTNode *temp, ASTNode *expr) {
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

void *build_expr(ASTNode *Troot, Token *tok)
{
    EXPRNode *expr = new EXPRNode(tok);
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

    auto parent = temp->get_parent();
    if (temp != nullptr) {
        root = expr;
    } else if (temp->get_left() == nullptr) {
        if (isnumber(temp->get_parent()->retrieve_token())) {
            if (parent != nullptr)
                parent->set_left(expr);
            temp->set_parent(expr);
            expr->set_left(temp);
            expr->set_right(temp->get_right());
        } else {
            expr->set_parent(temp);
            temp->set_left(expr);
        }
    } else {
        if (isnumber(temp->get_parent()->retrieve_token())) {
            if (parent != nullptr)
                parent->set_right(expr);
            temp->set_parent(expr);
            expr->set_right(temp);
            expr->set_left(temp->get_left());
        } else {
            expr->set_parent(temp);
            temp->set_right(expr);
        }
    }
}


int main()
{
    // Token *tok1, *tok2, *tok3, *tok4, *tok5;
    // tok1 = new Token()
}
