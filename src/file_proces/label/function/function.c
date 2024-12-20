#include "function.h"
#include "../../../stack/stack.h"
#include "../../../queue/queue.h"
#include <ctype.h>


struct function *process_function(char *function){
    struct function *processed_function = NULL;
    struct stack *stack = NULL;
    struct queue *queue = NULL;
    char current_char, next_char, dot_flag, stack_out;
    char *number = NULL;
    char **ptr;
    size_t i, number_len;
    double num_value, operand_1, operand_2, result;
    double (*operate)(float, float);

    if (!control_function(function)){
        return NULL;
    }
    

    processed_function = function_alloc();
    if(!processed_function){
        return NULL;
    }

    stack = stack_alloc(sizeof(char));
    if(!stack){
        error = MEMORY_ERR;
        return NULL;
    }

    queue = queue_alloc(sizeof(double));
    if(!queue){
        error = MEMORY_ERR;
        return NULL;
    }

    number_len = 0;
    dot_flag = 0;
    i = 0;
    while(function[i] != '\0'){

        current_char = function[i];
        next_char = function[i+1];

        if (isdigit(current_char) || current_char =='.'){

            if (current_char == '.' && dot_flag == 1){
                error = SYNTAX_ERR;
                return NULL;
            }

            else if (current_char == '.'){
                dot_flag = 1;
            }

            if (number_len == 0 && dot_flag == 1){
                error = SYNTAX_ERR;
                return NULL;
            }

            if (!isdigit(next_char) && next_char != '.'){
                number = malloc(sizeof(char) * number_len);
                if (!number){
                    error = MEMORY_ERR;
                    return NULL;
                }

                strncpy(number, function[i-number_len], number_len);
                num_value = strtod(number, ptr);
                free(number);

                queue_enqueue(queue, &num_value);

                number = NULL;
                number_len = 0;
                dot_flag = 0;
            }

            ++number_len;
        }

        if (isspace(current_char)){
            continue;
        }

        if (is_left_bracket(current_char)){
            stack_push(stack, &current_char);
        }

        if (is_right_bracket(current_char)){
            if(!stack_pop(stack, &stack_out)){
                error = SYNTAX_ERR;
                return NULL;
            }

            while (1){
                if (is_left_bracket(stack_out)){
                    if (!is_equal_bracket(stack_out, current_char)){
                        error = SYNTAX_ERR;
                        return NULL;
                    }
                    break; 
                }

                if(!queue_dequeue(queue, &operand_1)){
                    error = SYNTAX_ERR;
                    return NULL;
                }

                if(!queue_dequeue(queue, &operand_2)){
                    error = SYNTAX_ERR;
                    return NULL;
                }

                switch (stack_out){
                case '+': operate = sum; break;
                case '-': operate = sub; break;
                case '*': operate = mul; break;
                default: operate = NULL; break;
                }

                result = operate(operand_1, operand_2);
                queue_enqueue(queue, &result);
                

                if(!stack_pop(stack, &stack_out)){
                    error = SYNTAX_ERR;
                    return NULL;
                }
            }          
            
        }

        if (is_operator(current_char)){
            /* code */
        }
        
        ++i;
    }
   
}


int control_function(char *function){
    char *banned_chars = "^></:,";

    if(strpbrk(function, banned_chars)){
        error = SYNTAX_ERR;
        return 0;
    }

    return 1;
}

struct function *function_alloc(){
    struct function *new_function = NULL;

    new_function = malloc(sizeof(*new_function));

    if (!new_function){
        error = MEMORY_ERR;
        return NULL;
    }

    if(function_init(new_function) == 0){
        free(new_function);
        return NULL;
    }

    return new_function;
}

int function_init(struct function *function){
    if(!function){
        error = POINTER_ERR;
        return 0;
    }

    function->coefs = NULL;
    function->vars = NULL;
    function->coef_count = 0;

    return 1;
}

void function_deinit(struct function *function){
    if(!function){
        return;
    }

    free(function->coefs);
    function->coefs = NULL;

    /*variables_dealloc(&(function->vars), function->coef_count);*/
    function->coef_count = 0;
    return;
}

void function_dealloc(struct function **function){
    if(!function || !(*function)){
        return;
    }

    function_deinit(*function);
    free(*function);
    *function = NULL;
    return;
}

int is_left_bracket(const char symbol){
    if (symbol == '(' || symbol == '{' || symbol == '['){
        return 1;
    }
    
    return 0;
}

int is_right_bracket(const char symbol){
    if (symbol == ')' || symbol == '}' || symbol == ']'){
        return 1;
    }
    
    return 0;
}

int is_equal_bracket(const char left_bracket, const char right_bracket){
    
    switch (right_bracket){

    case ')':
        if (left_bracket == '('){
            return 1;
        }

        return 0;
        
        break;

    case '}':
        if (left_bracket == '{'){
            return 1;
        }

        return 0;
        
        break;

    case ']':
        if (left_bracket == ']'){
            return 1;
        }

        return 0;
        
        break;
    }

    return 0; 
}

int is_operator(char symbol){
    if (symbol == '+' || symbol == '-' || symbol == '*'){
        return 1;
    }
    
    return 0;
}

double sum(double a, double b){
    return a + b;
}

double sub(double a, double b){
    return a - b;
}

double mul(double a, double b){
    return a * b;
}