#ifndef PRIVATE_CROWBAR_H_INCLUDED
#define PRIVATE_CROWBAR_H_INCLUDED
#include <stdio.h>
#include "MEM.h"
#include "CRB.h"
#include "CRB_dev.h"

#define smaller(a, b) ((a) < (b) ? (a) : (b))
#define larger(a, b) ((a) > (b) ? (a) : (b))

#define MESSAGE_ARGUMENT_MAX (256)
#define LINE_BUF_SIZE (1024)

/* 编译时错误，详细内容在 error_message_not_gcc.c 文件中描述 */
typedef enum
{
    PARSE_ERR = 1,
    CHARACTER_INVALID_ERR,
    FUNCTION_MULTIPLE_DEFINE_ERR,
    COMPILE_ERROR_COUNT_PLUS_1
} CompileError;

/* 运行时错误 */
typedef enum
{
    VARIABLE_NOT_FOUND_ERR = 1,
    FUNCTION_NOT_FOUND_ERR,
    ARGUMENT_TOO_MANY_ERR,
    ARGUMENT_TOO_FEW_ERR,
    NOT_BOOLEAN_TYPE_ERR,
    MINUS_OPERAND_TYPE_ERR,
    BAD_OPERAND_TYPE_ERR,
    NOT_BOOLEAN_OPERATOR_ERR,
    FOPEN_ARGUMENT_TYPE_ERR,
    FCLOSE_ARGUMENT_TYPE_ERR,
    FGETS_ARGUMENT_TYPE_ERR,
    FPUTS_ARGUMENT_TYPE_ERR,
    NOT_NULL_OPERATOR_ERR,
    DIVISION_BY_ZERO_ERR,
    GLOBAL_VARIABLE_NOT_FOUND_ERR,
    GLOBAL_STATEMENT_IN_TOPLEVEL_ERR,
    BAD_OPERATOR_FOR_STRING_ERR,
    RUNTIME_ERROR_COUNT_PLUS_1
} RuntimeError;

typedef enum
{
    INT_MESSAGE_ARGUMENT = 1,
    DOUBLE_MESSAGE_ARGUMENT,
    STRING_MESSAGE_ARGUMENT,
    CHARACTER_MESSAGE_ARGUMENT,
    POINTER_MESSAGE_ARGUMENT,
    MESSAGE_ARGUMENT_END
} MessageArgumentType;

typedef struct
{
    char *format;
} MessageFormat;

typedef struct Expression_tag Expression;

/* 表达式的类型 */
typedef enum
{
    BOOLEAN_EXPRESSION = 1, /* 布尔型常量 */
    INT_EXPRESSION,         /* 整数型常量 */
    DOUBLE_EXPRESSION,      /* 实数型常量 */
    STRING_EXPRESSION,      /* 字符串型常量 */
    IDENTIFIER_EXPRESSION,  /* 变量 */
    ASSIGN_EXPRESSION,      /* 赋值表达式 */
    ADD_EXPRESSION,         /* 加减乘除表达式  */
    SUB_EXPRESSION,
    MUL_EXPRESSION,
    DIV_EXPRESSION,
    MOD_EXPRESSION,         /* 求余表达式 */
    EQ_EXPRESSION,          /* == */
    NE_EXPRESSION,          /* ！= */
    GT_EXPRESSION,          /* > */
    GE_EXPRESSION,          /* >= */
    LT_EXPRESSION,          /* < */
    LE_EXPRESSION,          /* <= */ 
    LOGICAL_AND_EXPRESSION, /* && */
    LOGICAL_OR_EXPRESSION,  /* || */
    MINUS_EXPRESSION,       /* 单目取负 */
    FUNCTION_CALL_EXPRESSION,
    NULL_EXPRESSION, /* null 表达式  */
    EXPRESSION_TYPE_COUNT_PLUS_1
} ExpressionType;

#define dkc_is_math_operator(operator) \
    ((operator) == ADD_EXPRESSION || (operator) == SUB_EXPRESSION || (operator) == MUL_EXPRESSION || (operator) == DIV_EXPRESSION || (operator) == MOD_EXPRESSION)

#define dkc_is_compare_operator(operator) \
    ((operator) == EQ_EXPRESSION || (operator) == NE_EXPRESSION || (operator) == GT_EXPRESSION || (operator) == GE_EXPRESSION || (operator) == LT_EXPRESSION || (operator) == LE_EXPRESSION)

#define dkc_is_logical_operator(operator) \
    ((operator) == LOGICAL_AND_EXPRESSION || (operator) == LOGICAL_OR_EXPRESSION)

typedef struct ArgumentList_tag
{
    Expression *expression;
    struct ArgumentList_tag *next;
} ArgumentList;

typedef struct
{
    char *variable;
    Expression *operand;
} AssignExpression;

typedef struct
{
    Expression *left;
    Expression *right;
} BinaryExpression;

typedef struct
{
    char *identifier;
    ArgumentList *argument;
} FunctionCallExpression;

struct Expression_tag
{
    ExpressionType type;
    int line_number;
    union {
        CRB_Boolean boolean_value;
        int int_value;
        double double_value;
        char *string_value;
        char *identifier;
        AssignExpression assign_expression;
        BinaryExpression binary_expression;
        Expression *minus_expression;
        FunctionCallExpression function_call_expression;
    } u;
};

typedef struct Statement_tag Statement;

/* 构建语句（statement）的思路 */
typedef struct StatementList_tag
{
    Statement *statement;
    struct StatementList_tag *next;
} StatementList;

typedef struct
{
    StatementList *statement_list;
} Block;

typedef struct IdentifierList_tag
{
    char *name;
    struct IdentifierList_tag *next;
} IdentifierList;

typedef struct
{
    IdentifierList *identifier_list;
} GlobalStatement;

typedef struct Elsif_tag
{
    Expression *condition;
    Block *block;
    struct Elsif_tag *next;
} Elsif;

typedef struct
{
    Expression *condition;
    Block *then_block;
    Elsif *elsif_list;
    Block *else_block;
} IfStatement;

typedef struct
{
    Expression *condition;  /* 条件表达式 */
    Block *block;           /* 可执行块 */
} WhileStatement;

typedef struct
{
    Expression *init;
    Expression *condition;
    Expression *post;
    Block *block;
} ForStatement;

typedef struct
{
    Expression *return_value;
} ReturnStatement;

typedef enum
{
    EXPRESSION_STATEMENT = 1,
    GLOBAL_STATEMENT,
    IF_STATEMENT,
    WHILE_STATEMENT,
    FOR_STATEMENT,
    RETURN_STATEMENT,
    BREAK_STATEMENT,
    CONTINUE_STATEMENT,
    STATEMENT_TYPE_COUNT_PLUS_1
} StatementType;

/* 构建语句的思路 */
struct Statement_tag
{
    StatementType type;
    int line_number;
    union {
        Expression *expression_s;   /* 表达式语句 */
        GlobalStatement global_s;   /* global 语句 */
        IfStatement if_s;           /* if 语句 */
        WhileStatement while_s;     /* while 语句 */
        ForStatement for_s;         /* for 语句 */
        ReturnStatement return_s;   /* return 语句 */
    } u;
};

typedef struct ParameterList_tag
{
    char *name;
    struct ParameterList_tag *next;
} ParameterList;

typedef enum
{
    CROWBAR_FUNCTION_DEFINITION = 1,
    NATIVE_FUNCTION_DEFINITION
} FunctionDefinitionType;

typedef struct FunctionDefinition_tag
{
    char *name;
    FunctionDefinitionType type;
    union {
        struct
        {
            ParameterList *parameter;
            Block *block;
        } crowbar_f;
        struct
        {
            CRB_NativeFunctionProc *proc;
        } native_f;
    } u;
    struct FunctionDefinition_tag *next;
} FunctionDefinition;

typedef struct Variable_tag
{
    char *name;
    CRB_Value value;
    struct Variable_tag *next;
} Variable;

typedef enum
{
    NORMAL_STATEMENT_RESULT = 1,
    RETURN_STATEMENT_RESULT,
    BREAK_STATEMENT_RESULT,
    CONTINUE_STATEMENT_RESULT,
    STATEMENT_RESULT_TYPE_COUNT_PLUS_1
} StatementResultType;

typedef struct
{
    StatementResultType type;
    union {
        CRB_Value return_value;
    } u;
} StatementResult;

typedef struct GlobalVariableRef_tag
{
    Variable *variable;
    struct GlobalVariableRef_tag *next;
} GlobalVariableRef;

typedef struct
{
    Variable *variable;                 /* 保存局部变量的链表 */
    GlobalVariableRef *global_variable; /* 根据global语句生成的引用全局变量的链表 */
} LocalEnvironment;

struct CRB_String_tag
{
    int ref_count;
    char *string;
    CRB_Boolean is_literal;
};

typedef struct
{
    CRB_String *strings;
} StringPool;

/* 解释器的定义 */
struct CRB_Interpreter_tag
{
    MEM_Storage interpreter_storage;
    MEM_Storage execute_storage;
    Variable *variable;
    FunctionDefinition *function_list;
    StatementList *statement_list;
    int current_line_number;
};
/* execute.c */
StatementResult
crb_execute_statement_list(CRB_Interpreter *inter,
                           LocalEnvironment *env, StatementList *list);
/* create.c */
void crb_function_define(char *identifier, ParameterList *parameter_list,
                         Block *block);
ParameterList *crb_create_parameter(char *identifier);
ParameterList *crb_chain_parameter(ParameterList *list,
                                   char *identifier);
ArgumentList *crb_create_argument_list(Expression *expression);
ArgumentList *crb_chain_argument_list(ArgumentList *list, Expression *expr);
StatementList *crb_create_statement_list(Statement *statement);
StatementList *crb_chain_statement_list(StatementList *list,
                                        Statement *statement);
Expression *crb_alloc_expression(ExpressionType type);

CRB_Value crb_eval_binary_expression(CRB_Interpreter *inter,
                                     LocalEnvironment *env,
                                     ExpressionType operator,
                                     Expression *left, Expression *right);
CRB_Value crb_eval_minus_expression(CRB_Interpreter *inter,
                                    LocalEnvironment *env, Expression *operand);
CRB_Value crb_eval_expression(CRB_Interpreter *inter,
                              LocalEnvironment *env, Expression *expr);

/* string_pool.c */
CRB_String *crb_literal_to_crb_string(CRB_Interpreter *inter, char *str);
void crb_refer_string(CRB_String *str);
void crb_release_string(CRB_String *str);
CRB_String *crb_search_crb_string(CRB_Interpreter *inter, char *str);
CRB_String *crb_create_crowbar_string(CRB_Interpreter *inter, char *str);

/* util.c */
CRB_Interpreter *crb_get_current_interpreter(void);
void crb_set_current_interpreter(CRB_Interpreter *inter);
void *crb_malloc(size_t size);
void *crb_execute_malloc(CRB_Interpreter *inter, size_t size);
Variable *crb_search_local_variable(LocalEnvironment *env,
                                    char *identifier);
Variable *
crb_search_global_variable(CRB_Interpreter *inter, char *identifier);
void crb_add_local_variable(LocalEnvironment *env,
                            char *identifier, CRB_Value *value);
CRB_NativeFunctionProc *
crb_search_native_function(CRB_Interpreter *inter, char *name);
FunctionDefinition *crb_search_function(char *name);
char *crb_get_operator_string(ExpressionType type);

/* error.c */
void crb_compile_error(CompileError id, ...);
void crb_runtime_error(int line_number, RuntimeError id, ...);

/* native.c */
CRB_Value crb_nv_print_proc(CRB_Interpreter *interpreter,
                            int arg_count, CRB_Value *args);
CRB_Value crb_nv_fopen_proc(CRB_Interpreter *interpreter,
                            int arg_count, CRB_Value *args);
CRB_Value crb_nv_fclose_proc(CRB_Interpreter *interpreter,
                             int arg_count, CRB_Value *args);
CRB_Value crb_nv_fgets_proc(CRB_Interpreter *interpreter,
                            int arg_count, CRB_Value *args);
CRB_Value crb_nv_fputs_proc(CRB_Interpreter *interpreter,
                            int arg_count, CRB_Value *args);
void crb_add_std_fp(CRB_Interpreter *inter);

#endif /* PRIVATE_CROWBAR_H_INCLUDED */
