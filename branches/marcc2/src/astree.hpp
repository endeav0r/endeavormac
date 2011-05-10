#ifndef astree_HEADER
#define astree_HEADER


#include <list>
#include <string>
#include <iostream>
#include <sstream>


enum { AST_ADD,
       AST_SUBTRACT,
       AST_MULTIPLY,
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
    protected :
        ASTreeExpr   * expr; // this is what follows the expr in []
                             // IE: expr[expr]
        int reference_count; // this is how we deal with pointers/references
                             // for parser: 
                             //   & should increase the reference count by 1
                             //   * should decrease the reference count by 1
    public :
        ASTreeExpr ();
        virtual void debug (int depth);
        void reference   ();
        void dereference ();
        int  g_reference_count ();
        void s_expr (ASTreeExpr * expr);
        ASTreeExpr * g_expr ();
        virtual std::string g_comment ();
};


class ASTreeExprConstant : public ASTreeExpr {
    private :
        int constant;
    public :
        ASTreeExprConstant (int constant);
        int g_constant ();
        void debug (int depth);
        std::string g_comment ();
};


class ASTreeExprString : public ASTreeExpr {
    private :
        std::string string;
    public :
        ASTreeExprString (std::string);
        std::string g_string ();
        void debug (int depth);
        std::string g_comment ();
};


class ASTreeSymbol : public ASTree {
    private :
        std::string symbol;
    public :
        ASTreeSymbol (std::string symbol);
        std::string g_symbol ();
        void debug (int depth);
        std::string g_comment ();
};


class ASTreeDecl : public ASTree {
    private :
        int type;
    public :
        ASTreeDecl (int type);
        int g_type ();
        void debug (int depth);
        std::string g_comment ();
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
        std::string g_comment();
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
        int g_operation ();
        void debug   (int depth);
        std::string g_comment ();
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
        ASTreeExprVar * g_left  ();
        ASTreeExpr    * g_right ();
        void debug (int depth);
        std::string g_comment ();
};


class ASTreeStatement : public ASTree {
    private :
        std::list <ASTree *> nodes;
    public :
        ~ASTreeStatement ();
        void push_node (ASTree * node);
        void push_node_front (ASTree * node);
        std::list <ASTree *> g_nodes();
        void debug (int depth);
};


class ASTreeCondition : public ASTree {
    private :
        int op;
        ASTreeExpr * left;
        ASTreeExpr * right;
    public :
        ASTreeCondition  (int op);
        ~ASTreeCondition ();
        void s_left  (ASTreeExpr * left);
        void s_right (ASTreeExpr * right);
        ASTreeExpr * g_left  ();
        ASTreeExpr * g_right ();
        int          g_op    ();
        void debug (int depth);
        std::string g_comment ();
};


class ASTreeIf : public ASTree {
    private :
        ASTreeStatement * statement;
        ASTreeCondition * condition;
    public :
        ~ASTreeIf ();
        void s_statement (ASTreeStatement * statement);
        void s_condition (ASTreeCondition * condition);
        ASTreeStatement * g_statement ();
        ASTreeCondition * g_condition ();
        void debug (int depth);
        std::string g_comment ();
};


class ASTreeWhile : public ASTree {
    private :
        ASTreeStatement * statement;
        ASTreeCondition * condition;
    public :
        ~ASTreeWhile ();
        void s_statement (ASTreeStatement * statement);
        void s_condition (ASTreeCondition * condition);
        ASTreeStatement * g_statement ();
        ASTreeCondition * g_condition ();
        void debug (int depth);
        std::string g_comment ();
};
/*
 * because we use ASTree as our parser stack, we *cheat* here and create some
 * tree nodes that won't exist in the final tree, but will exist temporarily
 * on the parser stack in order to save state
 */
 
class ASTreeTerminator   : public ASTree { public : void debug (int depth); };
class ASTreeParenOpen    : public ASTree { public : void debug (int depth); };
class ASTreeParenClose   : public ASTree { public : void debug (int depth); };
class ASTreeBlockOpen    : public ASTree { public : void debug (int depth); };
class ASTreeBlockClose   : public ASTree { public : void debug (int depth); };
class ASTreeBracketOpen  : public ASTree { public : void debug (int depth); };
class ASTreeBracketClose : public ASTree { public : void debug (int depth); };
class ASTreeStar         : public ASTree { public : void debug (int depth); };
class ASTreeAmpersand    : public ASTree { public : void debug (int depth); };


#endif
