#include <stdio.h>
#include <stdlib.h>

//全局变量
int token;              //当前的token src永远指向下一个指针位置
int token_val;          //对应Num的value
char * src = NULL;

//代表数值类型的枚举  
enum { Num = 128};      //区别于ASCII字符

/*适用于：token是一个字符的情况，如果是数字则无效，src指向下一个指针位置*/
void next(){
    token = *src;
    src++;
}
/*  输入：src永远指向未取用的位置
 *  此函数获得src指向的token(数字串或者字符), 调用一次返回一次token, src指向下一个位置
*/
int getToken(){
    while (*src != 0)   //只要本位置不是结束
    {
        next();         //取本位置这次，同是推进src ：： 结果就是获得token了，下面就是判断了
        //判断开始...
        if (token == ' '){
            while (token == ' ' || token == '\t'){
                next();
            }   //最终src指向目标
        }

        if (token >= '0' && token <= '9'){  //开始小循环
            token_val = token - '0';
            while (*src >= '0' && *src <= '9')
            {
                next();
                token_val = token_val * 10 + token - '0';
            }
            token = Num;
            return Num;
        }
        else{
            return token;
        }
    }
    return 0;
}
/* match 向前匹配function 输入：src tk 
    功能：判断前面那个src的内容是不是tk，如果是，那么就跳过src，指向src+1
                                      如果不是，打印错误
*/
void match(int tk){
    if(*src == tk)
        src++;
    else
        printf("different token! %c\n",tk);    
}

/*  BNF:
    <expr> ::= <term><expr_tail>
    <expr_tail> ::= +<term><expr_tail>
                  | -<term><expr_tail>
                  | <empty>
    <term>  ::= *<factor><term_tail>
              | /<factor><term_tail>
              | <empty>
    <factor> ::= Num
                | (<expr>)
*/

int expr();
int expr_tail(int lvalue);
int term();
int term_tail(int lvalue);
int factor();

/*这是第一个状态， 这个状态的返回结果就是，整个表达式的值*/
int expr(){
    int lvalue;         //进入expr状态后，就开始进行新的状态的创建（函数调用）和组织
    lvalue = term();    //创建一个新的状态，并获得其返回值
    return expr_tail(lvalue); //创建另外一个新的状态，返回其返回值
}
int expr_tail(int lvalue){
    int value;

    if(*src == '+'){            //如果向前看是+，此时token还不是+
        match('+');
        value = lvalue + term();
        return expr_tail(value);
    }
    if(*src == '-'){
        match('-');
        value = lvalue - term();
        return expr_tail(value);
    }
    return lvalue;
}
int term(){
    int lvalue;
    lvalue = factor();      //在此状态中创造一个新的状态，获得其返回值
    return term_tail(lvalue);   //创建另一个新的状态
}
int term_tail(int lvalue){
    int value;

    if(*src == '*'){
        match('*');
        value = lvalue * factor();
        return term_tail(value);
    }
    if(*src == '/'){
        match('/');
        value = lvalue / factor();
        return term_tail(value);
    }
    return lvalue;
}
int factor(){
    int value;
    getToken();     //获得src指向的token,src+1

    if(token == Num){
        return token_val;
    }
    if(token == '('){
        //match('(');     //推进一个src
        value = expr();         //根据这个新src进行求值
        match(')');
        return value;
    }
}

/* 获得命令行的一个字符串，以EOF为结尾, 还是不会字符串的处理*/
int getline(char * src){    
    char ch;                            //保存单个字符的变量 
    while ((ch = getchar()) && ch != 0 && ch != '\n'){
        *src++ = ch;
    }
    *src = 0;           //结尾放入EOF，字符串构造完毕
}


int main(int argc, char * argv[]){
    src = malloc(100);

    while (getline(src))
    {
        printf("confirm  string  : %s\n", src);
        printf("expr eval result : %d\n\n", expr());
        printf("please input expr: ");
    }
    
    return 0;
}