#include <string.h>
#include "crowbar.h"

/* 编译时错误 */
MessageFormat crb_compile_error_message_format[] = {
    {"dummy"},
    {"$(token)附近有语法错误"},
    {"错误的字符($(bad_char))"},
    {"函数名重复($(name))"},
    {"dummy"},
};

/* 运行时错误 */
MessageFormat crb_runtime_error_message_format[] = {
    {"dummy"},
    {"找不到变量($(name))。"},
    {"找不到函数($(name))。"},
    {"传递的参数多于函数所要求的参数"},
    {"传递的参数小于函数所要求的参数"},
    {"条件语句类型必须为boolean型"},
    {"����������Ĳ�������������ֵ�͡�"},
    {"˫Ŀ�����$(operator)�Ĳ���������ȷ��"},
    {"$(operator)�������������boolean�͡�"},
    {"��Ϊfopen()���������ļ���·���ʹ򿪷�ʽ�����߶����ַ������͵ģ���"},
    {"��Ϊfclose()���������ļ�ָ�롣"},
    {"��Ϊfgets()���������ļ�ָ�롣"},
    {"��Ϊfputs()���������ļ�ָ����ַ�����"},
    {"nullֻ��ʹ������� == �� != (���ܽ���$(operator)����)��"},
    {"���ܱ�0����"},
    {"ȫ�ֱ���$(name)�����ڡ�"},
    {"�����ں�����ʹ��global��䡣"},
    {"�����$(operator)���������ַ�����"},
    {"dummy"},
};
