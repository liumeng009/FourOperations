#include <stdio.h>

//全局变量
int token;              //当前的token
int token_val;          //对应Num的value
char * src = NULL;

//缓冲区代表状态
char exprInput[100];

enum { Num = 128};

/*  next token 分两级判断  
*/
void next(){

    while (*src == ' ' || *src == '\n'){
        src++;
    }
    
    token = *src++;         //向前走一个，要根据token进行下一步判断
        
    if (token >= '0' && token <= '9')
    {
        token_val = token - '0';
        while (*src >= '0' && *src <= '9')
        {
            token = *src++;
            token_val = token_val * 10 + token - '0';
        }
        token = Num;
        return;     //主要是分支作用
    }

    //如果是其他符号 直接使用token

    //如果是结束 token == 0
}

/* match(token) 消耗掉本token,next指向下一个token，如果结束token = 0
*/
void match(int tk){
    if(token == tk)
        next();
    else
        printf("different token! %c\n",tk);    
}

int expr();
int expr_tail(int lvalue);
int term();
int term_tail(int lvalue);
int factor();

/*这是第一个状态， 这个状态的返回结果就是，整个表达式的值*/
int expr(){
    int lvalue;     //进入expr状态后，就开始进行新的状态的创建（函数调用）和组织
    lvalue = term();    //创建一个新的状态，并获得其返回值
    return expr_tail(lvalue); //创建另外一个新的状态，返回其返回值
}
int expr_tail(int lvalue){
    int value;
    int rvalue;

    if(token == '+'){            //如果向前看是+，此时token还不是+
        match('+');
        rvalue = term();
        value = lvalue + rvalue;
        printf("%d + %d = %d\n",lvalue,rvalue,value);
        return expr_tail(value);
    }
    if(token == '-'){
        match('-');
        rvalue = term();
        value = lvalue - rvalue;
        printf("%d + %d = %d\n",lvalue,rvalue,value);
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
    int rvalue;

    if(token == '*'){
        match('*');
        rvalue = factor();
        value = lvalue * rvalue;
        printf("%d + %d = %d\n",lvalue,rvalue,value);
        return term_tail(value);
    }
    if(token == '/'){
        match('/');
        rvalue = factor();
        value = lvalue / rvalue;
        printf("%d + %d = %d\n",lvalue,rvalue,value);
        return term_tail(value);
    }
    return lvalue;
}
int factor(){
    int value;

    if(token == Num){
        match(Num);
        return token_val;
    }
    if(token == '('){
        match('(');     //推进一个src
        value = expr();         //根据这个新src进行求值
        match(')');
        return value;
    }
}

int main(int argc, char * argv[]){

    src = exprInput;
    
    while(1){
        printf("input expr string: ");
        scanf("%s",src);
        fflush(stdin);
        printf("confirm: %s\n",src);
        next();
        printf("the result is : %d \n",expr());
    }

    getchar();
    return 0;
}
