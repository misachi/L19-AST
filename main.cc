#include "include/ast.hh"

#include <vector>
#include <iostream>

int main()
{
    // dummy expression node
    Token *tok1, *tok2, *tok3, *tok4, *tok5;
    tok1 = new Token(INT);
    tok1->val = 20;
    tok2 = new Token(MINUS);
    tok2->val = '-';
    tok3 = new Token(INT);
    tok3->val = 10;
    tok4 = new Token(PLUS);
    tok4->val = '+';
    tok5 = new Token(INT);
    tok5->val = 60;
    std::vector<Token *> list{tok2, tok3, tok4, tok5};

    EXPRNode *root = new EXPRNode(tok1);
    for (size_t i = 0; i < list.size(); i++)
    {
        build_expr(root, list.at(i));
    }

    root->print();
    std::cout << "\n";
}