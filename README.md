# FourOperations
# 利用BNF范式求解四则运算表达式的代码
# 使用：输入字符串如：2+(3*4) 输出：14

BNF如下:
<expr> ::= <term><expr_tail>
<expr_tail> ::= +<term><expr_tail>
              | -<term><expr_tail>
              | <empty>
<term>  ::= *<factor><term_tail>
              | /<factor><term_tail>
              | <empty>
<factor> ::= Num
              | (<expr>)
              
