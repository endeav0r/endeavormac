#ifndef astree_HEADER
#define astree_HEADER


#include <list>
#include <string>
#include <iostream>


enum { AST_ADD,
       AST_SUBTRACT,
       AST_EQUALS,
       AST_LESS_THAN,
       AST_GREATER_THAN,
       AST_INT };


class ASTree {
    protected :
        std::string debug_space (int depth);
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
        unsigned int g_constant ();
        void debug (int depth);
};


class ASTreeSymbol : public ASTree {
    private :
        std::string symbol;
    public :
        ASTreeSymbol (std::string symbol);
        std::string g_symbol ();
        void debug (int depth);
};


class ASTreeDecl : public ASTree {
    private :
        int type;
    public :
        ASTreeDecl (int type);
        int g_type ();
        void debug (int depth);
};


class ASTreeExprVar : public ASTreeExpr {
    private :
        ASTreeSymbol * symbol;
        ASTreeDecl   * decl;
    public :
        ASTreeExprVar ();
        void s_symbol (ASTreeSymbol * symbol);
        void s_decl   (ASTreeDecl   * decl);
        ASTreeSymbol * g_symbol ();
        ASTreeDecl   * g_decl   ();
        void debug (int depth);
};


class ASTreeExprArithmetic : public ASTreeExpr {
    private :
        int operation;
        ASTreeExpr * left;
        ASTreeExpr * right;
        
    public :
        ASTreeExprArithmetic(int operation);
        ~ASTreeExprArithmetic();
        void s_left  (ASTreeExpr * left);
        void s_right (ASTreeExpr * right);
        ASTreeExpr * g_left  ();
        ASTreeExpr * g_right ();
        void debug   (int depth);
};


class ASTreeAssign : public ASTree {
    private :
        ASTreeExprVar * left;
        ASTreeExpr * right;
    public :
        ASTreeAssign ();
        ASTreeAssign (ASTreeExprVar * left, ASTreeExpr * right);
        ~ASTreeAssign ();
        void s_left  (ASTreeExprVar * left);
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
        void debug (int depth);
};


class ASTreeWhile : public ASTree {
    private :
        ASTreeCondition * condition;
        ASTreeBlock * block;
    public :
        ~ASTreeWhile ();
        void s_condition (ASTreeCondition * condition);
        void s_block     (ASTreeBlock     * block);
        void debug (int depth);
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
