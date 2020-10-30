#include "include/ast.hh"

#include <vector>
#include <iostream>

int main()
{
    // dummy tree
    Token tok1{INT};
    tok1.val = 20;
    Token tok2{MINUS};
    tok2.val = '-';
    Token tok3{INT};
    tok3.val = 10;
    Token tok4{PLUS};
    tok4.val = '+';
    Token tok5{INT};
    tok5.val = 60;
    std::vector<Token> list{tok2, tok3, tok4, tok5};

    EXPRNode *root{new EXPRNode(&tok1)};
    ASTNode *Troot{root};
    for (size_t i = 0; i < list.size(); i++)
    {
        build_expr(root, &list.at(i));
    }
    EXPRNode *tmp{new EXPRNode{&tok5}};
    RETURNNode *node{new RETURNNode{tmp}};
    build_ast(Troot, node);

    Token tok10{ID};
    tok10.val = "temp";
    Token tok11{INT};
    tok11.val = 400;
    Token tok13{MINUS};
    tok13.val = '-';
    Token tok12{INT};
    tok12.val = 20;
    EXPRNode *expr{new EXPRNode{&tok11}};
    build_expr(expr, &tok13);
    build_expr(expr, &tok12);
    ASSIGNNode *ass{new ASSIGNNode{&tok10, expr}};
    build_ast(Troot, ass);
    walk_tree(Troot);

    delete Troot;
}
