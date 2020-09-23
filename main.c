#include <stdio.h>
#include <stdlib.h>

//全局变量
int token;              //当前的token src永远指向下一个指针位置
int token_val;          //对应Num的value
char * src = NULL;

//代表数值类型的枚举  
enum { Num = 128};      //区别于ASCII字符

/*  输入：src永远指向未取用的位置
 *  
 *  批：一方面获得了token（主线） 另一方面也是向前推进 src指向下一位置
*/
void nextToken(){

    while (*src == ' ' || *src == '\t')  { //小循环：去除所有空格和tab 一次搞完
        src++;
    }
    token = *src++;

    if (token >= '0' && token <= '9')
    { //小循环: 识别数字串  一次搞完
        token_val = token - '0';
        token = Num;                //肯定是数字了

        while (*src >= '0' && *src <= '9') //向前是数字：
        {
            token_val = token_val * 10 + *src - '0';
            src++;
        }
        return;
    }
    else if (token == '(' || token == ')' || token == '+' || token == '-' || token == '*' || token == '/')
    {
        return;
    }
}
/* match 获得token，然后与字面值进行比较, 如果相等，不用处理
*/
void match(int tk){
    if(token != tk)
        printf("different token! %c\n",tk);
    else
        nextToken();    
}

/*  BNF:代表着一种解释：用右边的格式套左边的数据，就像联合和结构体的共享内存关系, 每一路计算都必须经过叶子结点
    <expr> ::= <term><expr_tail>
    <expr_tail> ::= +<term><expr_tail>
                  | -<term><expr_tail>
                  | <empty>
    <term> ::= <factor><term_tail>
    <term_tail>  ::= *<factor><term_tail>
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

    if(token == '+'){            //如果向前看是+，此时token还不是+
        match('+');
        value = lvalue + term();
        return expr_tail(value);
    }
    if(token == '-'){
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
    int value = 0;
    
    if( token == '*'){
        match('*');
        value = lvalue * factor();
        return term_tail(value);
    }
    if( token == '/'){
        match('/');
        value = lvalue / factor();
        return term_tail(value);
    }
    return lvalue;
}
int factor(){
    int value = 0;
    //nextToken();     //获得src指向的token,src+1

    if(token == Num){
        match(Num);                     //驱动向前走
        return token_val;
    }
    if(token == '('){
        match('(');           //推进一个src
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
        nextToken();
        printf("expr eval result : %d\n\n", expr());
        printf("please input expr: ");
    }
    
    return 0;
}
