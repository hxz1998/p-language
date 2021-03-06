#ifndef PUBLIC_DVM_CODE_H_INCLUDED
#define PUBLIC_DVM_CODE_H_INCLUDED
#include <stdio.h>
#include <wchar.h>
#include "DVM.h"

typedef enum
{
    DVM_BOOLEAN_TYPE,
    DVM_INT_TYPE,
    DVM_DOUBLE_TYPE,
    DVM_STRING_TYPE
} DVM_BasicType;

typedef struct DVM_TypeSpecifier_tag DVM_TypeSpecifier;

/* 局部变量 */
typedef struct
{
    char *name;
    DVM_TypeSpecifier *type;
} DVM_LocalVariable;

typedef enum
{
    DVM_FUNCTION_DERIVE
} DVM_DeriveTag;

typedef struct
{
    int parameter_count;
    DVM_LocalVariable *parameter;
} DVM_FunctionDerive;

typedef struct DVM_TypeDerive_tag
{
    DVM_DeriveTag tag;
    union {
        DVM_FunctionDerive function_d;
    } u;
} DVM_TypeDerive;

struct DVM_TypeSpecifier_tag
{
    DVM_BasicType basic_type;
    int derive_count;
    DVM_TypeDerive *derive;
};

typedef wchar_t DVM_Char;
typedef unsigned char DVM_Byte;

/* 操作码 */
typedef enum
{
    DVM_PUSH_INT_1BYTE = 1,
    DVM_PUSH_INT_2BYTE,
    DVM_PUSH_INT,
    DVM_PUSH_DOUBLE_0,
    DVM_PUSH_DOUBLE_1,
    DVM_PUSH_DOUBLE,
    DVM_PUSH_STRING,
    /**********/
    DVM_PUSH_STACK_INT,
    DVM_PUSH_STACK_DOUBLE,
    DVM_PUSH_STACK_STRING,
    DVM_POP_STACK_INT,
    DVM_POP_STACK_DOUBLE,
    DVM_POP_STACK_STRING,
    /**********/
    DVM_PUSH_STATIC_INT,
    DVM_PUSH_STATIC_DOUBLE,
    DVM_PUSH_STATIC_STRING,
    DVM_POP_STATIC_INT,
    DVM_POP_STATIC_DOUBLE,
    DVM_POP_STATIC_STRING,
    /**********/
    DVM_ADD_INT,
    DVM_ADD_DOUBLE,
    DVM_ADD_STRING,
    DVM_SUB_INT,
    DVM_SUB_DOUBLE,
    DVM_MUL_INT,
    DVM_MUL_DOUBLE,
    DVM_DIV_INT,
    DVM_DIV_DOUBLE,
    DVM_MOD_INT,
    DVM_MOD_DOUBLE,
    DVM_MINUS_INT,
    DVM_MINUS_DOUBLE,
    DVM_INCREMENT,
    DVM_DECREMENT,
    DVM_CAST_INT_TO_DOUBLE,
    DVM_CAST_DOUBLE_TO_INT,
    DVM_CAST_BOOLEAN_TO_STRING,
    DVM_CAST_INT_TO_STRING,
    DVM_CAST_DOUBLE_TO_STRING,
    DVM_EQ_INT,
    DVM_EQ_DOUBLE,
    DVM_EQ_STRING,
    DVM_GT_INT,
    DVM_GT_DOUBLE,
    DVM_GT_STRING,
    DVM_GE_INT,
    DVM_GE_DOUBLE,
    DVM_GE_STRING,
    DVM_LT_INT,
    DVM_LT_DOUBLE,
    DVM_LT_STRING,
    DVM_LE_INT,
    DVM_LE_DOUBLE,
    DVM_LE_STRING,
    DVM_NE_INT,
    DVM_NE_DOUBLE,
    DVM_NE_STRING,
    DVM_LOGICAL_AND,
    DVM_LOGICAL_OR,
    DVM_LOGICAL_NOT,
    DVM_POP,
    DVM_DUPLICATE,
    DVM_JUMP,
    DVM_JUMP_IF_TRUE,
    DVM_JUMP_IF_FALSE,
    /**********/
    DVM_PUSH_FUNCTION,
    DVM_INVOKE,
    DVM_RETURN
} DVM_Opcode;

typedef enum
{
    DVM_CONSTANT_INT,
    DVM_CONSTANT_DOUBLE,
    DVM_CONSTANT_STRING
} DVM_ConstantPoolTag;

typedef struct
{
    DVM_ConstantPoolTag tag;
    union {
        int c_int;
        double c_double;
        DVM_Char *c_string;
    } u;
} DVM_ConstantPool;

typedef struct
{
    char *name;
    DVM_TypeSpecifier *type;
} DVM_Variable;

typedef struct
{
    int line_number;
    int start_pc;
    int pc_count;
} DVM_LineNumber;

/* 函数定义 */
typedef struct
{
    DVM_TypeSpecifier *type;        /* 函数的返回类型 */
    char *name;                     /* 函数名称 */
    int parameter_count;
    DVM_LocalVariable *parameter;
    DVM_Boolean is_implemented;     /* 是否在 DVM_Executable 函数中实现 */
    int local_variable_count;
    DVM_LocalVariable *local_variable;
    int code_size;
    DVM_Byte *code;
    int line_number_size;
    DVM_LineNumber *line_number;
    int need_stack_size;
} DVM_Function;

struct DVM_Excecutable_tag
{
    int constant_pool_count;
    DVM_ConstantPool *constant_pool;
    int global_variable_count;
    DVM_Variable *global_variable;
    int function_count;
    DVM_Function *function;
    int code_size;
    DVM_Byte *code;                 /* 指向顶层结构对应的字节码 */
    int line_number_size;
    DVM_LineNumber *line_number;    /* 保存了字节码上的位置对应的源文件行号 */
    int need_stack_size;            /* 保存了顶层结构所需要的栈空间的大小 */
};

#endif /* PUBLIC_DVM_CODE_H_INCLUDED */
