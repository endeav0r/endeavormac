#include "parser.hpp"


Parser :: Parser (std::list <Token> tokens) {
    this->tokens = tokens;
    this->tokens_it = this->tokens.begin();
    this->tree = NULL;
}


Parser :: ~Parser () {
    std::list <ASTree *> :: iterator stack_it;
    
    for (stack_it = this->stack.begin();
         stack_it != this->stack.end();
         stack_it++) {
        delete *stack_it;
    }
    if (this->tree != NULL)
        delete this->tree;
}


ASTree * Parser :: g_tree () { return this->tree; }


Token Parser :: g_next_token () {
    Token result;
    this->tokens_it++;
    result = *(this->tokens_it);
    this->tokens_it--;
    return result;
}

#include <cstdio>
void Parser :: reduce () {
    int size;
    bool reduce = true;
    ASTreeAssign         * assign;
    ASTreeSymbol         * symbol;
    ASTreeDecl           * decl;
    ASTreeExprVar        * var;
    ASTreeExpr           * expr;
    ASTreeExpr           * expr2;
    ASTreeExprArithmetic * arith;
    ASTreeCondition      * condition;
    ASTreeStatement      * statement;
    ASTreeIf             * treeIf;
    ASTreeWhile          * treeWhile;

    std::list <ASTree *> :: iterator stack_it;
    
    while (reduce == true) {
        reduce = false;
        stack_it = this->stack.begin();
        size = this->stack.size();
        if (size == 0)
            break;
            
        // expr [ ( \[ ]
        if (((expr = dynamic_cast<ASTreeExpr *>(*stack_it)))
            && (this->g_next_token().g_type() != TOKEN_PAREN_OPEN)
            && (this->g_next_token().g_type() != TOKEN_BRACKET_OPEN)) {
            std::cout << "PARSER_REDUCE_EXPR\n";
            if (size == 1)
                break;
            stack_it++;
            
            // assign expr [ + - ]
            if (((assign = dynamic_cast<ASTreeAssign *>(*stack_it)))
                && (this->g_next_token().g_type() != TOKEN_PLUS)
                && (this->g_next_token().g_type() != TOKEN_MINUS)) {
                std::cout << "PARSER_REDUCE_ASSIGN_EXPR\n";
                if (size == 2)
                    break;
                stack_it++;
                
                // var assign expr
                if ((var = dynamic_cast<ASTreeExprVar *>(*stack_it))) {
                    std::cout << "PARSER_REDUCE_VAR_ASSIGN_EXPR\n";
                    assign->s_right(expr);
                    assign->s_left(var);
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.push_front(assign);
                    reduce = true;
                } // symbol assign expr
            } // assign expr [ + ]
            
            // arith expr
            else if ((arith = dynamic_cast<ASTreeExprArithmetic *>(*stack_it))) {
                std::cout << "PARSER_REDUCE_ARITH_EXPR\n";
                if (size == 2)
                    break;
                stack_it++;
                
                // expr arith expr
                if ((dynamic_cast<ASTreeExpr *>(*stack_it))) {
                    std::cout << "PARSER_REDUCE_EXPR_ARITH_EXPR\n";
                    arith->s_right(expr);
                    arith->s_left(dynamic_cast<ASTreeExpr *>(*stack_it));
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.push_front(arith);
                    reduce = true;
                } // expr arith expr
            } // arith expr
            // star expr
            else if (dynamic_cast<ASTreeStar *>(*stack_it)) {
                if (size < 3) {
                    std::cout << "PARSER_REDUCE_STAR_EXPR DEREF_1\n";
                    expr->dereference();
                    this->stack.pop_front();
                    delete this->stack.front();
                    this->stack.pop_front();
                    this->stack.push_front(expr);
                }
                // expr star expr
                else if ((expr2 = dynamic_cast<ASTreeExpr *>(*stack_it))) {
                    this->stack.pop_front();
                    delete this->stack.front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    arith = new ASTreeExprArithmetic(AST_MULTIPLY);
                    arith->s_left(expr2);
                    arith->s_right(expr);
                    this->stack.push_front(arith);
                }
                else {
                    std::cout << "PARSER_REDUCE_STAR_EXPR DEREF_2\n";
                    expr->dereference();
                    this->stack.pop_front();
                    delete this->stack.front();
                    this->stack.pop_front();
                    this->stack.push_front(expr);
                }
                reduce = true;
            }// star expr
            // ampersand expr
            else if (dynamic_cast<ASTreeStar *>(*stack_it)) {
                expr->reference();
                this->stack.pop_front();
                this->stack.pop_front();
                this->stack.push_front(expr);
            }
        } // expr [ ( \] ]
        
        // )
        else if ((dynamic_cast<ASTreeParenClose *>(*stack_it))) {
            if (size < 3)
                break;
            stack_it++;
            // expr )
            if ((expr = dynamic_cast<ASTreeExpr *>(*stack_it))) {
                stack_it++;
                // ( expr )
                if ((dynamic_cast<ASTreeParenOpen *>(*stack_it))) {
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.push_front(expr);
                    reduce = true;
                }
                // condition expr )
                else if ((condition = dynamic_cast<ASTreeCondition *>(*stack_it))) {
                    if (size < 5)
                        break;
                    stack_it++;
                    // expr condition expr )
                    if ((expr2 = dynamic_cast<ASTreeExpr *>(*stack_it))) {
                        stack_it++;
                        // ( expr condition expr )
                        if ((dynamic_cast<ASTreeParenOpen *>(*stack_it))) {
                            std::cout << "PARSER_REDUCE_(_EXPR_CONDITION_EXPR_)\n";
                            condition->s_left(expr2);
                            condition->s_right(expr);
                            delete this->stack.front();
                            this->stack.pop_front();
                            this->stack.pop_front();
                            this->stack.pop_front();
                            this->stack.pop_front();
                            delete this->stack.front();
                            this->stack.pop_front();
                            this->stack.push_front(condition);
                            reduce = true;
                        } // ( expr condition expr )
                    } // expr condition expr )
                } // condition expr )
            } // expr )
        } // )
        
        // }
        else if ((dynamic_cast<ASTreeBlockClose *>(*stack_it))) {
            stack_it++;
            while (stack_it != this->stack.end()) {
                // { statements }
                if ((dynamic_cast<ASTreeBlockOpen *>(*stack_it))) {
                    statement = new ASTreeStatement();
                    this->stack.pop_front();
                    while ((dynamic_cast<ASTreeStatement *>(this->stack.front()))) {
                        statement->push_node_front(dynamic_cast<ASTree *>(this->stack.front()));
                        this->stack.pop_front();
                    }
                    this->stack.pop_front();
                    this->stack.push_front(statement);
                    reduce = true;
                    break;
                }
                else if ((dynamic_cast<ASTreeStatement *>(*stack_it)) == NULL)
                    break;
                stack_it++;
            }
        }
        
        // ]
        else if ((dynamic_cast<ASTreeBracketClose *>(*stack_it))) {
            stack_it++;
            // expr ]
            if ((expr = dynamic_cast<ASTreeExpr *>(*stack_it))) {
                stack_it++;
                // [ expr ]
                if ((dynamic_cast<ASTreeBracketOpen *>(*stack_it))) {
                    stack_it++;
                    // expr [ expr ]
                    if ((expr2 = dynamic_cast<ASTreeExpr *>(*stack_it))) {
                        delete this->stack.front();
                        this->stack.pop_front();
                        this->stack.pop_front();
                        delete this->stack.front();
                        this->stack.pop_front();
                        expr2->s_expr(expr);
                        reduce = true;
                    } // expr [ expr ]
                } // [ expr ]
            } // expr ]
        } // ]
        
        // statement
        else if ((statement = dynamic_cast<ASTreeStatement *>(*stack_it))) {
            std::cout << "PARSE_REDUCE_STATEMENT\n";
            if (size < 3)
                continue;
            stack_it++;
            // condition statement
            if ((condition = dynamic_cast<ASTreeCondition *>(*stack_it))) {
                std::cout << "PARSE_REDUCE_CONDITION_STATEMENT\n";
                stack_it++;
                // if condition statement
                if ((treeIf = dynamic_cast<ASTreeIf *>(*stack_it))) {
                    std::cout << "PARSE_REDUCE_IF_CONDITION_STATEMENT\n";
                    treeIf->s_condition(condition);
                    treeIf->s_statement(statement);
                    this->stack.pop_front();
                    this->stack.pop_front();
                    statement = new ASTreeStatement();
                    statement->push_node(treeIf);
                    this->stack.pop_front();
                    this->stack.push_front(statement);
                    reduce = true;
                }// if condition statement
                // while condition statement
                if ((treeWhile = dynamic_cast<ASTreeWhile *>(*stack_it))) {
                    std::cout << "PARSE_REDUCE_WHILE_CONDITION_STATEMENT\n";
                    condition->debug(0);
                    treeWhile->s_condition(condition);
                    treeWhile->s_statement(statement);
                    this->stack.pop_front();
                    this->stack.pop_front();
                    statement = new ASTreeStatement();
                    statement->push_node(treeWhile);
                    this->stack.pop_front();
                    this->stack.push_front(statement);
                    reduce = true;
                } // while condition statement
            } // condition statement
        } // statement
        
        // symbol
        else if ((symbol = dynamic_cast<ASTreeSymbol *>(*stack_it))) {
            std::cout << "PARSE_REDUCE_SYMBOL\n";
            var = new ASTreeExprVar();
            var->s_symbol(symbol);
            this->stack.pop_front();
            this->stack.push_front(var);
            reduce = true;
        }
        
        // var
        if ((var = dynamic_cast<ASTreeExprVar *>(*stack_it))) {
            if (size >= 2) {
                stack_it++;
                // decl var
                if ((decl = dynamic_cast<ASTreeDecl *>(*stack_it))) {
                    std::cout << "PARSE_REDUCE_DECL_VAR\n";
                    var = new ASTreeExprVar();
                    var->s_symbol(symbol);
                    var->s_decl(decl);
                    this->stack.pop_front();
                    this->stack.pop_front();
                    this->stack.push_front(var);
                    reduce = true;
                }
            }
        } // var
        
        // terminator
        else if ((dynamic_cast<ASTreeTerminator *>(*stack_it))) {
            std::cout << "PARSER_REDUCE_TERMINATOR\n";
            if (size < 2)
                break;
            stack_it++;
            // assign terminator
            if ((dynamic_cast<ASTreeAssign *>(*stack_it))) {
                statement = new ASTreeStatement();
                this->stack.pop_front();
                statement->push_node_front(this->stack.front());
                this->stack.pop_front();
                this->stack.push_front(statement);
                reduce = true;
            }// assign terminator
        } // terminator
    } // while (reduce == true)
}


void Parser :: parse () {
    ASTreeStatement * statement;
    Token token;
    
    for (this->tokens_it  = this->tokens.begin();
         this->tokens_it != this->tokens.end();
         this->tokens_it++) {

        token = *(this->tokens_it);

        switch (token.g_type()) {
            case TOKEN_SYMBOL :
                std::cout << "PARSER_TOKEN_SYMBOL\n";
                this->stack.push_front(new ASTreeSymbol(token.g_text()));
                break;
            case TOKEN_INT :
                std::cout << "PARSER_TOKEN_INT\n";
                this->stack.push_front(new ASTreeDecl(AST_INT));
                break;
            case TOKEN_NUMBER :
                std::cout << "PARSER_TOKEN_NUMBER\n";
                this->stack.push_front(new ASTreeExprConstant(atoi(token.g_text().c_str())));
                break;
            case TOKEN_STRING :
                std::cout << "PARSER_STRING\n";
                this->stack.push_front(new ASTreeExprString(token.g_text()));
                break;
            case TOKEN_EQUAL :
                if (this->g_next_token().g_type() == TOKEN_EQUAL) {
                    std::cout << "PARSER_TOKEN_EQUAL\n";
                    this->tokens_it++;
                    this->stack.push_front(new ASTreeCondition(AST_EQUALS));
                    break;
                }
                else {
                    std::cout << "PARSER_TOKEN_ASSIGN\n";
                    this->stack.push_front(new ASTreeAssign());
                    break;
                }
            case TOKEN_LESS_THAN :
                std::cout << "PARSER_TOKEN_LESS_THAN\n";
                this->stack.push_front(new ASTreeCondition(AST_LESS_THAN));
                break;
            case TOKEN_GREATER_THAN :
                std::cout << "PARSER_TOKEN_LESS_THAN\n";
                this->stack.push_front(new ASTreeCondition(AST_GREATER_THAN));
                break;
            case TOKEN_PLUS :
                std::cout << "PARSER_TOKEN_PLUS\n";
                this->stack.push_front(new ASTreeExprArithmetic(AST_ADD));
                break;
            case TOKEN_MINUS :
                std::cout << "PARSER_TOKEN_MINUS\n";
                this->stack.push_front(new ASTreeExprArithmetic(AST_SUBTRACT));
                break;
            case TOKEN_STAR :
                std::cout << "PARSER_TOKEN_STAR\n";
                this->stack.push_front(new ASTreeStar());
                break;
            case TOKEN_AMPERSAND :
                std::cout << "PARSER_TOKEN_AMPERSAND\n";
                this->stack.push_front(new ASTreeAmpersand());
                break;
            case TOKEN_IF :
                std::cout << "PARSER_IF\n";
                this->stack.push_front(new ASTreeIf());
                break;
            case TOKEN_WHILE :
                std::cout << "PARSER_WHILE\n";
                this->stack.push_front(new ASTreeWhile());
                break;
            case TOKEN_PAREN_OPEN :
                std::cout << "PARSER_PAREN_OPEN\n";
                this->stack.push_front(new ASTreeParenOpen());
                break;
            case TOKEN_PAREN_CLOSE :
                std::cout << "PARSER_PAREN_CLOSE\n";
                this->stack.push_front(new ASTreeParenClose());
                break;
            case TOKEN_BLOCK_OPEN :
                std::cout << "PARSER_BLOCK_OPEN\n";
                this->stack.push_front(new ASTreeBlockOpen());
                break;
            case TOKEN_BLOCK_CLOSE :
                std::cout << "PARSER_BLOCK_CLOSE\n";
                this->stack.push_front(new ASTreeBlockClose());
                break;
            case TOKEN_BRACKET_OPEN :
                std::cout << "PARSER_BRACKET_OPEN\n";
                this->stack.push_front(new ASTreeBracketOpen());
                break;
            case TOKEN_BRACKET_CLOSE :
                std::cout << "PARSER_BRACKET_CLOSE\n";
                this->stack.push_front(new ASTreeBracketClose());
                break;
            case TOKEN_TERMINATOR :
                std::cout << "PARSER_TERMINATOR\n";
                this->stack.push_front(new ASTreeTerminator());
                break;
            default :
                throw Exception("Invalid Token");
                break;
        }
        
        this->reduce();
        
    }
    
    if (this->tree != NULL)
        delete this->tree;
    
    if (this->stack.size() > 1) {
        statement = new ASTreeStatement();
        while (this->stack.size() > 0) {
            statement->push_node_front(this->stack.front());
            this->stack.pop_front();
        }
        this->tree = statement;
    }
    else if (this->stack.size() == 1) {
        this->tree = this->stack.front();
        this->stack.pop_front();
    }
    
}
