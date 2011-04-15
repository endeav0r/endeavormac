#include "astree.hpp"

int ASTreeBlock :: next_id = 0;

void ASTree     :: debug (int depth) { std::cout << depth << "\tASTree\n"; }
void ASTreeExpr :: debug (int depth) { std::cout << depth << "\tASTreeExpr\n"; }
void ASTreeExprSymbol :: debug (int depth) { 
    std::cout << depth << "\tASTreeExprSymbol " << this->symbol << "\n"; }
void ASTreeExprConstant :: debug (int depth) {
    std::cout << depth << "\tASTreeExprConstant " << this->constant << "\n"; }
void ASTreeAssign :: debug (int depth) {
    std::cout << depth << "\tASTreeExprAssign\n";
    std::cout << depth << "\tLHS:\n";
    this->left->debug(depth + 1);
    std::cout << depth << "\tRHS:\n";
    this->right->debug(depth + 1);
}
void ASTreeExprArithmetic :: debug (int depth) {
    std::cout << depth << "\tASTreeArithmetic ";
    switch (this->operation) {
        case AST_ADD :
            std::cout << "ADD\n";
            break;
    }
    std::cout << depth << "\tleft\n";
    this->left->debug(depth + 1);
    std::cout << depth << "\tright\n";
    this->right->debug(depth + 1);
}
void ASTreeCondition :: debug (int depth) {
    std::cout << depth << "\tASTreeCondition ";
    switch (this->op) {
        case AST_EQUALS :
            std::cout << "EQUALS\n";
            break;
    }
    std::cout << depth << "\tleft\n";
    this->left->debug(depth + 1);
    std::cout << depth << "\tright\n";
    this->right->debug(depth + 1);
}
void ASTreeStatement :: debug (int depth) {
    std::cout << depth << "\tASTreeStatement\n";
    for (std::list <ASTree *> :: iterator it = this->nodes.begin();
         it != this->nodes.end(); it++) {
        (*it)->debug(depth + 1);
    }
}
void ASTreeBlock :: debug (int depth) {
    std::cout << depth << "\tASTreeBlock " << this->id << "\n";
    for (std::list <ASTreeStatement *> :: iterator it = this->statements.begin();
         it != this->statements.end(); it++) {
        (*it)->debug(depth + 1);
    }
}
void ASTreeIf :: debug (int depth) {
    std::cout << depth << "\tASTreeIf\n";
    std::cout << depth << "\tcondition:\n";
    this->condition->debug(depth + 1);
    std::cout << depth << "\tblock:\n";
    this->block->debug(depth + 1);
}
void ASTreeTerminator :: debug (int depth) { 
    std :: cout << depth << "\tASTreeTerminator\n"; }
void ASTreeParenOpen :: debug (int depth) { 
    std :: cout << depth << "\tASTreeParenOpen\n"; }
void ASTreeParenClose :: debug (int depth) {
    std :: cout << depth << "\tASTreeParenClose\n"; }
void ASTreeBlockOpen :: debug (int depth) { 
    std :: cout << depth << "\tASTreeBlockOpen\n"; }
void ASTreeBlockClose :: debug (int depth) {
    std :: cout << depth << "\tASTreeBlockClose\n"; }
         
         

ASTreeExprConstant :: ASTreeExprConstant (unsigned int constant) {
    this->constant = constant;
}

ASTreeExprSymbol :: ASTreeExprSymbol (std::string symbol) {
    this->symbol = symbol;
}

ASTreeExprArithmetic :: ASTreeExprArithmetic (int operation) {
    this->operation = operation;
    this->left = NULL;
    this->right = NULL;
}

void ASTreeExprArithmetic :: s_left  (ASTreeExpr * left)  { this->left = left; }
void ASTreeExprArithmetic :: s_right (ASTreeExpr * right) { this->right = right; }

ASTreeAssign :: ASTreeAssign () {
    this->left = NULL;
    this->right = NULL;
}

ASTreeAssign :: ASTreeAssign (ASTreeExprSymbol * left,
                              ASTreeExpr * right) {
    this->left = left;
    this->right = right;
}

ASTreeAssign :: ~ ASTreeAssign () {
    delete this->left;
    delete this->right;
}

void ASTreeAssign :: s_left  (ASTreeExprSymbol * left)  { this->left  = left; }
void ASTreeAssign :: s_right (ASTreeExpr       * right) { this->right = right; }

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
