#ifndef DVM_PRI_H_INCLUDED
#define DVM_PRI_H_INCLUDED
#include "DVM_code.h"
#include "DVM_dev.h"
#include "share.h"

#define STACK_ALLOC_SIZE (4096)
#define HEAP_THRESHOLD_SIZE (1024 * 256)
#define TRUE_STRING (L"true")
#define FALSE_STRING (L"false")

#define NO_LINE_NUMBER_PC (-1)

#define MESSAGE_ARGUMENT_MAX (256)
#define LINE_BUF_SIZE (1024)

#define GET_2BYTE_INT(p) (((p)[0] << 8) + (p)[1])
#define SET_2BYTE_INT(p, value) \
    (((p)[0] = (value) >> 8), ((p)[1] = value & 0xff))

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
} ErrorDefinition;

typedef enum
{
    BAD_MULTIBYTE_CHARACTER_ERR = 1,
    FUNCTION_NOT_FOUND_ERR,
    FUNCTION_MULTIPLE_DEFINE_ERR,
    RUNTIME_ERROR_COUNT_PLUS_1
} RuntimeError;

typedef struct
{
    DVM_Char *string;
} VString;

typedef enum
{
    NATIVE_FUNCTION,
    DIKSAM_FUNCTION
} FunctionKind;

typedef struct
{
    DVM_NativeFunctionProc *proc;
    int arg_count;
} NativeFunction;

typedef struct
{
    DVM_Executable *executable;
    int index;
} DiksamFunction;

typedef struct
{
    char *name;
    FunctionKind kind;
    union {
        NativeFunction native_f;
        DiksamFunction diksam_f;
    } u;
} Function;

typedef struct
{
    Function *caller;   /* 当前函数的调用者 */
    int caller_address; /* 函数内字节码上的地址 */
    int base;           /* 参数或者局部变量的地址 */
} CallInfo;             /* 返回信息的实体 */

#define revalue_up_align(val) ((val) ? (((val)-1) / sizeof(DVM_Value) + 1) \
                                     : 0)
#define CALL_INFO_ALIGN_SIZE (revalue_up_align(sizeof(CallInfo)))

typedef union {
    CallInfo s;
    DVM_Value u[CALL_INFO_ALIGN_SIZE];
} CallInfoUnion;

typedef struct
{
    int alloc_size;
    int stack_pointer;          /* 保存着栈顶的索引值 */
    DVM_Value *stack;           /* 栈的实体内容 */
    DVM_Boolean *pointer_flags; /* 指示栈上的值是否是指针类型的数据，大小和stack一致 */
} Stack;

typedef enum
{
    STRING_OBJECT = 1,
    OBJECT_TYPE_COUNT_PLUS_1
} ObjectType;

struct DVM_String_tag
{
    DVM_Boolean is_literal;
    DVM_Char *string;
};

struct DVM_Object_tag
{
    ObjectType type;
    unsigned int marked : 1;
    union {
        DVM_String string;
    } u;
    struct DVM_Object_tag *prev;
    struct DVM_Object_tag *next;
};

typedef struct
{
    int current_heap_size;
    int current_threshold;
    DVM_Object *header; /* 堆上的对象使用链表形式存储 */
} Heap;

typedef struct
{
    int variable_count;
    DVM_Value *variable;    
} Static;

/* DVM 描述 */
struct DVM_VirtualMachine_tag
{
    Stack stack;        /* 栈 ，存放局部变量、函数的参数或者函数返回的信息等 */
    Heap heap;          /* 堆 ，通过引用进行访问的内存区域 */
    Static static_v;    /* 静态字段， 保存全局变量 */
    int pc;             /* 程序计数器，保存正在执行的指令地址的作用 */
    Function *function;
    int function_count;
    DVM_Executable *executable;
};

/* heap.c */
DVM_Object *
dvm_literal_to_dvm_string_i(DVM_VirtualMachine *inter, DVM_Char *str);
DVM_Object *
dvm_create_dvm_string_i(DVM_VirtualMachine *dvm, DVM_Char *str);
void dvm_garbage_collect(DVM_VirtualMachine *dvm);
/* native.c */
void dvm_add_native_functions(DVM_VirtualMachine *dvm);
/* wchar.c */
wchar_t *dvm_mbstowcs_alloc(DVM_Executable *exe, Function *func, int pc,
                            const char *src);
/* util.c */
void dvm_vstr_clear(VString *v);
void dvm_vstr_append_string(VString *v, DVM_Char *str);
void dvm_vstr_append_character(VString *v, DVM_Char ch);
/* error.c */
void dvm_error(DVM_Executable *exe, Function *func, int pc, RuntimeError id, ...);

#endif /* DVM_PRI_H_INCLUDED */
