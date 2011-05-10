#include "astree.hpp"

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
void ASTreeExprString :: debug (int depth) {
    std::cout << depth << debug_space(depth)
              << "ASTreeExprString " << this->string << "\n"; }
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
    if (this->expr != NULL) {
        std::cout << depth << debug_space(depth) << "expr:\n";
        this->expr->debug(depth + 1);
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
    std::cout << depth << debug_space(depth) << "ASTreeCondition ";
    switch (this->op) {
        case AST_EQUALS :
            std::cout << "EQUALS\n";
            break;
        case AST_LESS_THAN :
            std::cout << "LESS_THAN\n";
            break;
        case AST_GREATER_THAN :
            std::cout << "GREATER_THAN\n";
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
void ASTreeIf :: debug (int depth) {
    std::cout << depth << debug_space(depth) << "ASTreeIf\n";
    std::cout << depth << debug_space(depth) << "condition:\n";
    this->condition->debug(depth + 1);
    std::cout << depth << debug_space(depth) << "statement:\n";
    this->statement->debug(depth + 1);
}
void ASTreeWhile :: debug (int depth) {
    std::cout << depth << debug_space(depth) << "ASTreeWhile\n";
    std::cout << depth << debug_space(depth) << "condition:\n";
    this->condition->debug(depth + 1);
    std::cout << depth << debug_space(depth) << "statement:\n";
    this->statement->debug(depth + 1);
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
void ASTreeBracketOpen :: debug (int depth) { 
    std :: cout << depth << debug_space(depth) << "ASTreeBracketOpen\n"; }
void ASTreeBracketClose :: debug (int depth) {
    std :: cout << depth << debug_space(depth) << "ASTreeBracketClose\n"; }
void ASTreeStar :: debug (int depth) {
    std :: cout << depth << debug_space(depth) << "ASTreeStar\n"; }
void ASTreeAmpersand :: debug (int depth) {
    std :: cout << depth << debug_space(depth) << "ASTreeAmpersand\n"; }
         

std::string ASTreeSymbol :: g_comment () { return this->symbol; }
std::string ASTreeExprConstant :: g_comment () {
    std::stringstream stream;
    stream << this->constant;
    return stream.str();
}
std::string ASTreeExpr :: g_comment () { return "expression"; }
std::string ASTreeDecl :: g_comment () {
    switch (type) {
        case AST_INT :
            return "int";
    }
    return "decl";
}
std::string ASTreeExprVar :: g_comment () {
    std::string comment;
    if (this->decl != NULL)
        comment = this->decl->g_comment() + " ";
    comment += this->symbol->g_comment();
    if (this->expr != NULL)
        comment += "[" + this->expr->g_comment() + "]";
    return comment;
}
std::string ASTreeExprString :: g_comment () {
    return std::string("\"") + this->string + "\"";
}
std::string ASTreeExprArithmetic :: g_comment () {
    std::string comment;
    comment = this->left->g_comment();
    switch (this->operation) {
        case AST_ADD :
            comment += " + ";
            break;
        case AST_SUBTRACT :
            comment += " - ";
            break;
    }
    comment += this->right->g_comment();
    return comment;
}
std::string ASTreeAssign :: g_comment () {
    return this->left->g_comment() + " = " + this->right->g_comment();
}
std::string ASTreeCondition :: g_comment () {
    std::string comment;
    comment = this->left->g_comment();
    switch (this->op) {
        case AST_EQUALS :
            comment += " = ";
            break;
        case AST_LESS_THAN :
            comment += " < ";
            break;
        case AST_GREATER_THAN :
            comment += " > ";
            break;
    }
    comment += this->right->g_comment();
    return comment;
}
std::string ASTreeIf :: g_comment () {
    return "if (" + this->condition->g_comment() + ")";
}
std::string ASTreeWhile :: g_comment () {
    return "while (" + this->condition->g_comment() + ")";
}



ASTreeExprConstant :: ASTreeExprConstant (int constant) {
    this->constant = constant;
}
int ASTreeExprConstant :: g_constant () { return this->constant; }



ASTreeExprString :: ASTreeExprString (std::string string) {
    this->string = string; }
std::string ASTreeExprString :: g_string() { return this->string; }


ASTreeSymbol :: ASTreeSymbol (std::string symbol) { this->symbol = symbol; }
std::string ASTreeSymbol :: g_symbol  () { return this->symbol; }

ASTreeDecl :: ASTreeDecl (int type) { this->type = type; }
int ASTreeDecl :: g_type () { return this->type; }


ASTreeExpr :: ASTreeExpr () { this->expr = NULL; this->reference_count = 0; }
void         ASTreeExpr :: s_expr (ASTreeExpr   * expr) { this->expr = expr; }
ASTreeExpr * ASTreeExpr :: g_expr () { return this->expr; }
void ASTreeExpr :: reference () { this->reference_count++; }
void ASTreeExpr :: dereference () { this->reference_count--; }
int  ASTreeExpr :: g_reference_count () { return this->reference_count; }


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
ASTreeExpr * ASTreeExprArithmetic :: g_left      () { return this->left; }
ASTreeExpr * ASTreeExprArithmetic :: g_right     () { return this->right; }
int          ASTreeExprArithmetic :: g_operation () { return this->operation; }


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
ASTreeExprVar * ASTreeAssign :: g_left  () { return this->left;  }
ASTreeExpr    * ASTreeAssign :: g_right () { return this->right; }


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

std::list <ASTree *> ASTreeStatement :: g_nodes () { return this->nodes; }



ASTreeIf :: ~ASTreeIf () {
    delete this->condition;
    delete this->statement;
}

void ASTreeIf :: s_condition (ASTreeCondition * condition) { this->condition = condition; }
void ASTreeIf :: s_statement (ASTreeStatement * statement) { this->statement = statement; }
ASTreeStatement * ASTreeIf :: g_statement () { return this->statement; }
ASTreeCondition * ASTreeIf :: g_condition () { return this->condition; }

ASTreeWhile :: ~ASTreeWhile () {
    delete this->condition;
    delete this->statement;
}

void ASTreeWhile :: s_condition (ASTreeCondition * condition) { this->condition = condition; }
void ASTreeWhile :: s_statement (ASTreeStatement * statement) { this->statement = statement; }
ASTreeStatement * ASTreeWhile :: g_statement () { return this->statement; }
ASTreeCondition * ASTreeWhile :: g_condition () { return this->condition; }

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
ASTreeExpr * ASTreeCondition :: g_left  () { return this->left; }
ASTreeExpr * ASTreeCondition :: g_right () { return this->right; }
int          ASTreeCondition :: g_op    () { return this->op; }
