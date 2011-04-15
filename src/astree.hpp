#ifndef astree_HEADER
#define astree_HEADER


#include <list>
#include <string>
#include <iostream>


enum { AST_ADD,
       AST_SUBTRACT,
       AST_EQUALS,
       AST_LESS_THAN };


class ASTree {
    public :
        virtual void debug (int depth);
};

       
class ASTreeExpr : public ASTree {
    public :
        virtual void debug (int depth);
};


class ASTreeExprConstant : public ASTreeExpr {
    private :
        unsigned int constant;
    public :
        ASTreeExprConstant (unsigned int constant);
        void debug (int depth);
};


class ASTreeExprSymbol : public ASTreeExpr {
    private :
        std::string symbol;
    public :
        ASTreeExprSymbol (std::string symbol);
        void debug (int depth);
};


class ASTreeExprArithmetic : public ASTreeExpr {
    private :
        int operation;
        ASTreeExpr * left;
        ASTreeExpr * right;
        
    public :
        ASTreeExprArithmetic(int operation);
        void s_left  (ASTreeExpr * left);
        void s_right (ASTreeExpr * right);
        void debug   (int depth);
};


class ASTreeAssign : public ASTree {
    private :
        ASTreeExprSymbol * left;
        ASTreeExpr * right;
    public :
        ASTreeAssign ();
        ASTreeAssign (ASTreeExprSymbol * left, ASTreeExpr * right);
        ~ASTreeAssign ();
        void s_left  (ASTreeExprSymbol * left);
        void s_right (ASTreeExpr * right);
        void debug (int depth);
};


class ASTreeStatement : public ASTree {
    private :
        std::list <ASTree *> nodes;
    public :
        ~ASTreeStatement ();
        void push_node (ASTree * node);
        void push_node_front (ASTree * node);
        void debug (int depth);
};


class ASTreeCondition : public ASTree {
    private :
        int op;
        ASTreeExpr * left;
        ASTreeExpr * right;
    public :
        ASTreeCondition (int op);
        ~ASTreeCondition ();
        void s_left (ASTreeExpr * left);
        void s_right (ASTreeExpr * right);
        void debug (int depth);
};


class ASTreeBlock : public ASTree {
    private :
        static int next_id;
        int id; // blocks get their own id. isn't that nice
        std::list <ASTreeStatement *> statements;
    public :
        ASTreeBlock ();
        ~ASTreeBlock ();
        void push_statement (ASTreeStatement * statement);
        void push_statement_front (ASTreeStatement * statement);
        void debug (int depth);
};


class ASTreeIf : public ASTree {
    private :
        ASTreeCondition * condition;
        ASTreeBlock * block;
    public :
        ~ASTreeIf ();
        void s_condition (ASTreeCondition * condition);
        void s_block     (ASTreeBlock     * block);
        void debug(int depth);
};


/*
 * because we use ASTree as our parser stack, we *cheat* here and create some
 * tree nodes that won't exist in the final tree, but will exist temporarily
 * on the parser stack in order to save state
 */
 
class ASTreeTerminator : public ASTree { public : void debug (int depth); };
class ASTreeParenOpen  : public ASTree { public : void debug (int depth); };
class ASTreeParenClose : public ASTree { public : void debug (int depth); };
class ASTreeBlockOpen  : public ASTree { public : void debug (int depth); };
class ASTreeBlockClose : public ASTree { public : void debug (int depth); };


#endif
