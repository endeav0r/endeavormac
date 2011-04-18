#include "astree.hpp"

int ASTreeBlock :: next_id = 0;

std::string ASTree :: debug_space (int depth) {
    std::string space = "";
    while (depth--)
        space += " ";
    return space;
}
        

void ASTree     :: debug (int depth) { std::cout << depth << debug_space(depth)
                                                 << "ASTree\n"; }
void ASTreeExpr :: debug (int depth) { std::cout << depth << debug_space(depth)
                                                 << "ASTreeExpr\n"; }
void ASTreeSymbol :: debug (int depth) { 
    std::cout << depth << debug_space(depth) 
              << "ASTreeSymbol " << this->symbol << "\n"; }
void ASTreeExprConstant :: debug (int depth) {
    std::cout << depth << debug_space(depth)
              << "ASTreeExprConstant " << this->constant << "\n"; }
void ASTreeAssign :: debug (int depth) {
    std::cout << depth << debug_space(depth) << "ASTreeExprAssign\n";
    std::cout << depth << debug_space(depth) << "LHS:\n";
    this->left->debug(depth + 1);
    std::cout << depth << debug_space(depth) << "RHS:\n";
    this->right->debug(depth + 1);
}
void ASTreeDecl :: debug (int depth) {
    std::cout << depth << debug_space(depth) 
              << "ASTreeDecl " << this->type << "\n";
}
void ASTreeExprVar :: debug (int depth) {
    std::cout << depth << debug_space(depth) << "ASTreeExprVar\n";
    std::cout << depth << debug_space(depth) << "symbol:\n";
    this->symbol->debug(depth + 1);
    if (this->decl != NULL) {
        std::cout << depth << debug_space(depth) << "decl:\n";
        this->decl->debug(depth + 1);
    }
}
void ASTreeExprArithmetic :: debug (int depth) {
    std::cout << depth << debug_space(depth) << "ASTreeArithmetic ";
    switch (this->operation) {
        case AST_ADD :
            std::cout << "ADD\n";
            break;
    }
    std::cout << depth << debug_space(depth) << "left\n";
    this->left->debug(depth + 1);
    std::cout << depth << debug_space(depth) << "right\n";
    this->right->debug(depth + 1);
}
void ASTreeCondition :: debug (int depth) {
    std::cout << depth << debug_space(depth) << "ASTreeCondition\n";
    switch (this->op) {
        case AST_EQUALS :
            std::cout << "EQUALS\n";
            break;
    }
    std::cout << depth << debug_space(depth) << "left\n";
    this->left->debug(depth + 1);
    std::cout << depth << debug_space(depth) << "right\n";
    this->right->debug(depth + 1);
}
void ASTreeStatement :: debug (int depth) {
    std::cout << depth << debug_space(depth) << "ASTreeStatement\n";
    for (std::list <ASTree *> :: iterator it = this->nodes.begin();
         it != this->nodes.end(); it++) {
        (*it)->debug(depth + 1);
    }
}
void ASTreeBlock :: debug (int depth) {
    std::cout << depth << debug_space(depth) 
              << "ASTreeBlock " << this->id << "\n";
    for (std::list <ASTreeStatement *> :: iterator it = this->statements.begin();
         it != this->statements.end(); it++) {
        (*it)->debug(depth + 1);
    }
}
void ASTreeIf :: debug (int depth) {
    std::cout << depth << debug_space(depth) << "ASTreeIf\n";
    std::cout << depth << debug_space(depth) << "condition:\n";
    this->condition->debug(depth + 1);
    std::cout << depth << debug_space(depth) << "block:\n";
    this->block->debug(depth + 1);
}
void ASTreeWhile :: debug (int depth) {
    std::cout << depth << debug_space(depth) << "ASTreeWhile\n";
    std::cout << depth << debug_space(depth) << "condition:\n";
    this->condition->debug(depth + 1);
    std::cout << depth << debug_space(depth) << "block:\n";
    this->block->debug(depth + 1);
}
void ASTreeTerminator :: debug (int depth) { 
    std :: cout << depth << debug_space(depth) << "ASTreeTerminator\n"; }
void ASTreeParenOpen :: debug (int depth) { 
    std :: cout << depth << debug_space(depth) << "ASTreeParenOpen\n"; }
void ASTreeParenClose :: debug (int depth) {
    std :: cout << depth << debug_space(depth) << "ASTreeParenClose\n"; }
void ASTreeBlockOpen :: debug (int depth) { 
    std :: cout << depth << debug_space(depth) << "ASTreeBlockOpen\n"; }
void ASTreeBlockClose :: debug (int depth) {
    std :: cout << depth << debug_space(depth) << "ASTreeBlockClose\n"; }
         
         

ASTreeExprConstant :: ASTreeExprConstant (unsigned int constant) {
    this->constant = constant;
}
unsigned int ASTreeExprConstant :: g_constant () { return this->constant; }

ASTreeSymbol :: ASTreeSymbol (std::string symbol) { this->symbol = symbol; }
std::string ASTreeSymbol :: g_symbol () { return this->symbol; }

ASTreeDecl :: ASTreeDecl (int type) { this->type = type; }
int ASTreeDecl :: g_type () { return this->type; }

ASTreeExprVar :: ASTreeExprVar () {
    this->symbol = NULL;
    this->decl = NULL;
}

void ASTreeExprVar :: s_symbol (ASTreeSymbol * symbol) { this->symbol = symbol; }
void ASTreeExprVar :: s_decl   (ASTreeDecl   * decl  ) { this->decl   = decl;   }
ASTreeSymbol * ASTreeExprVar :: g_symbol () { return this->symbol; }
ASTreeDecl   * ASTreeExprVar :: g_decl   () { return this->decl;   }

ASTreeExprArithmetic :: ASTreeExprArithmetic (int operation) {
    this->operation = operation;
    this->left = NULL;
    this->right = NULL;
}

ASTreeExprArithmetic :: ~ASTreeExprArithmetic () {
    delete this->left;
    delete this->right;
}

void ASTreeExprArithmetic :: s_left  (ASTreeExpr * left)  { this->left = left; }
void ASTreeExprArithmetic :: s_right (ASTreeExpr * right) { this->right = right; }
ASTreeExpr * ASTreeExprArithmetic :: g_left  () { return this->left; }
ASTreeExpr * ASTreeExprArithmetic :: g_right () { return this->right; }

ASTreeAssign :: ASTreeAssign () {
    this->left = NULL;
    this->right = NULL;
}

ASTreeAssign :: ASTreeAssign (ASTreeExprVar * left,
                              ASTreeExpr * right) {
    this->left = left;
    this->right = right;
}

ASTreeAssign :: ~ ASTreeAssign () {
    delete this->left;
    delete this->right;
}

void ASTreeAssign :: s_left  (ASTreeExprVar * left)  { this->left  = left; }
void ASTreeAssign :: s_right (ASTreeExpr    * right) { this->right = right; }

ASTreeStatement :: ~ASTreeStatement () {
    std::list <ASTree *> :: iterator it;
    for (it = this->nodes.begin(); it != this->nodes.end(); it++) {
        delete (*it);
    }
}

void ASTreeStatement :: push_node (ASTree * node) {
    this->nodes.push_back(node);
}

void ASTreeStatement :: push_node_front (ASTree * node) {
    this->nodes.push_front(node);
}

ASTreeBlock :: ASTreeBlock () {
    this->id = ASTreeBlock :: next_id;
    ASTreeBlock :: next_id++;
}

ASTreeBlock :: ~ASTreeBlock () {
    std::list <ASTreeStatement *> :: iterator it;
    for (it = this->statements.begin(); it != this->statements.end(); it++) {
        delete (*it);
    }
}

void ASTreeBlock :: push_statement (ASTreeStatement * statement) {
    this->statements.push_back(statement);
}

ASTreeIf :: ~ASTreeIf () {
    delete this->condition;
    delete this->block;
}

void ASTreeIf :: s_condition (ASTreeCondition * condition) { this->condition = condition; }
void ASTreeIf :: s_block     (ASTreeBlock * block)         { this->block = block; }

ASTreeWhile :: ~ASTreeWhile () {
    delete this->condition;
    delete this->block;
}

void ASTreeWhile :: s_condition (ASTreeCondition * condition) { this->condition = condition; }
void ASTreeWhile :: s_block     (ASTreeBlock * block)         { this->block = block; }

void ASTreeBlock :: push_statement_front (ASTreeStatement * statement) {
    this->statements.push_front(statement);
}

ASTreeCondition :: ASTreeCondition (int op) {
    this->op = op;
    this->left = NULL;
    this->right = NULL;
}

ASTreeCondition :: ~ASTreeCondition () {
    delete this->left;
    delete this->right;
}

void ASTreeCondition :: s_left  (ASTreeExpr * left)  { this->left = left; }
void ASTreeCondition :: s_right (ASTreeExpr * right) { this->right = right; }
