#include "function.h"

struct function *process_function(char *function, const struct generals *generals){
    struct function *processed_function = NULL;
    struct queue *queue = NULL;
    struct stack *stack = NULL;
    char current_char, stack_out;
    int i, parse_number_result, parse_var_result, sign;
    struct queue_record operand_1, operand_2, result;
    operation operator = NULL;

    if (!control_function(function)){
        return NULL;
    }
    

    processed_function = function_alloc(generals->variables_count);
    if(!processed_function){
        return NULL;
    }

    stack = stack_alloc(sizeof(char));
    if(!stack){
        error = MEMORY_ERR;
        return NULL;
    }

    queue = queue_alloc(sizeof(struct queue_record));
    if(!stack){
        error = MEMORY_ERR;
        return NULL;
    }

    sign = 1;
    i = 0;

    while(function[i] != '\0'){

        current_char = function[i];

        if (isdigit(current_char)){
            parse_number_result = parse_number(stack, queue, &function[i], &sign);

            if (parse_number_result == -1){
                return NULL;
            }
            i += parse_number_result;
        }

        if (isspace(current_char)){
            ++i;
            continue;
        }

        if (is_left_bracket(current_char)){
            stack_push(stack, &current_char);

            if (!check_unary_operator(current_char, &sign)){
                return NULL;
            }
            
            ++i;
            continue;
        }

        if (is_right_bracket(current_char)){
            if(!parse_brackets(queue, stack, current_char)){
                return NULL;
            }

            ++i;
            continue; 
        }

        if (is_operator(current_char)){
            if(!parse_operator(queue, stack, current_char, &sign)){
                return NULL;
            }
            ++i;
            continue;
        }

        parse_var_result = parse_variable(queue, stack, &function[i], generals, &sign);
        if (parse_var_result == -1){
            return NULL;
        }

        i += parse_var_result;
    }
    
    while (!check_empty(stack)){

        if(is_empty(queue)){
            goto err;
        }

        queue_dequeue(queue, &operand_2);

        if(is_empty(queue)){
            goto err;
        }

        queue_dequeue(queue, &operand_1);

        stack_pop(stack, &stack_out);

        operator = get_operation(stack_out);

        if(!operator){
            goto err;
        }

        if(!evaluate(&operand_1, &operand_2, queue, operator)){
            return 0;
        }
    }

    if(is_empty(queue)){
        goto err;
    }

    queue_dequeue(queue, &result);

    if(!is_empty(queue)){
        goto err;
    }

    processed_function->coefs = result.coef_values;

    free(result.coef_values);

    queue_dealloc(&queue);
    stack_dealloc(&stack);

    return processed_function;

    err:
        error = SYNTAX_ERR;
        return NULL;
}

int check_multiply(const char next_char){
    if (!is_right_bracket(next_char) && !is_operator(next_char) && !isdigit(next_char)){
        return 1;
    }

    return 0;  
}

int parse_variable(struct queue *queue, struct stack *stack, const char *function, const struct generals *generals, int *sign){
    size_t i;
    char next_char;
    char *variable_name = NULL;
    int variable_id;
    struct queue_record new_record;
    double *coefs = NULL;


    i = 0;
    next_char = function[i + 1];

    while ((!is_operator(next_char)) || (!is_left_bracket(next_char)) || (!is_right_bracket(next_char))){
        ++i;
        next_char = function[i + 1];
    }

    variable_name = calloc(sizeof(char), i+1);
    if (!variable_name){
        error = MEMORY_ERR;
        return -1;
    }
    
    strncpy(variable_name, function, i+1);

    variable_id = get_variable(variable_name, generals);
    if(variable_id == -1){
        error = VARIABLE_ERR;
        return -1;
    }

    free(variable_name);

    coefs = calloc(sizeof(double), generals->variables_count);
    if (!coefs){
        error = MEMORY_ERR;
        return -1;
    }

    if(!queue_record_init(&new_record, coefs, generals->variables_count, 0, NULL)){
        return -1;
    }
    
    if(!queue_enqueue(queue, &new_record)){
        error = POINTER_ERR;
        return -1;
    }

    if (is_left_bracket(next_char)){
        if(!parse_operator(queue, stack, MULTIPLY, sign)){
            return -1;
        }
    }

    return i;
}

int parse_operator(struct queue *queue, struct stack *stack, const char current_char, int *sign){
    char stack_out, current_char_cpy;
    int stack_priority, current_priority;
    operation operator;
    struct queue_record operand_1, operand_2;

    if(check_empty(stack)){
        stack_push(stack, &current_char);
        return 1;
    }

    stack_pop(stack, &stack_out);

    stack_priority = get_priority(stack_out);
    current_priority = get_priority(current_char);

    if(stack_priority == -1 || current_priority == -1){
        error = SYNTAX_ERR;
        return 0;
    }

    while (stack_priority >= current_priority){
        if (is_left_bracket(stack_out)){
            break;
        }

        if(check_empty(stack)){
            break;
        }

        operator = get_operation(stack_out);

        queue_dequeue(queue, &operand_2);

        queue_dequeue(queue, &operand_1);

        if (!evaluate(&operand_1, &operand_2, queue, operator)){
            return 0;
        }

        stack_pop(stack, &stack_out);
        stack_priority = get_priority(stack_out);
    }

    if (is_empty(queue)){
        if(!check_unary_operator(current_char, sign)){
            return 0;
        }
    }
    
    if (current_char == MINUS){
        *sign = -1;
        current_char_cpy = PLUS;
    }

    else{
        *sign = 1;
        current_char_cpy = current_char;
    }

    stack_push(stack, &current_char_cpy);
    return 1;
}

int check_unary_operator(const char operator, int *sign){
    switch (operator){
        case PLUS:
            *sign = 1;
            return 1;
            break;

        case MINUS:
            *sign = -1;
            return 1;
            break;

        case MULTIPLY:
            error = SYNTAX_ERR;
            return 0;
            break;

        default:
            error = SYNTAX_ERR;
            return 0;
            break;
    }
}

int parse_brackets(struct queue *queue, struct stack *stack, const char current_char){
    char stack_out;
    struct queue_record operand_1, operand_2;
    operation operate;

    if(!stack_pop(stack, &stack_out)){
        error = SYNTAX_ERR;
        return 0;
    }

    while (!is_left_bracket(stack_out)){

        operate = get_operation(stack_out);

        if(!operate){
            error = SYNTAX_ERR;
            return 0;
        }

        queue_dequeue(queue, &operand_2);

        queue_dequeue(queue, &operand_1);

        if (!evaluate(&operand_1, &operand_2, queue, operate)){
            return 0;
        }
        
        if(!stack_pop(stack, &stack_out)){
            error = SYNTAX_ERR;
            return 0;
        }

    }

    if (!is_equal_bracket(stack_out, current_char)){
        error = SYNTAX_ERR;
        return 0;
    }
    return 1; 
}

int evaluate(struct queue_record *operand_1, struct queue_record *operand_2, struct queue *queue, operation operator){
    if (operator == mul){
        if (operand_1 && operand_2){
            error = SYNTAX_ERR;
            return 0;
        }

        if (operand_1){
            if(!array_mul(operand_1->coef_values, operand_2->value, operand_1->arr_len)){
                return 0;
            }
            queue_enqueue(queue, operand_1);
        }

        if (operand_2){
            if(!array_mul(operand_2->coef_values, operand_1->value, operand_2->arr_len)){
                return 0;
            }
            queue_enqueue(queue, operand_2);
        }

        operand_1->value = operator(operand_1->value, operand_2->value);
        queue_enqueue(queue, operand_1);
        return 1;
    }

    else{
        if (operand_1 && operand_2){
            if(!array_sum(operand_1->coef_values, operand_2->coef_values, operand_1->arr_len)){
                return 0;
            }

            free(operand_2->coef_values);
            operand_2->coef_values = NULL;

            queue_enqueue(queue, operand_1);
            return 1;
        }

        if (operand_1){
            queue_enqueue(queue, operand_1);
            return 1;
        }

        if (operand_2){
            queue_enqueue(queue, operand_2);
            return 1;
        }

        operand_1->value = operator(operand_1->value, operand_2->value);
        queue_enqueue(queue, operand_1);
        return 1;
    }
    
}

int parse_number(struct stack *stack, struct queue *queue, const char *function, int *sign){
    char current_char, next_char, dot_flag;
    struct queue_record new_record;
    size_t i;
    char *number = NULL;
    double num_value;
    char **ptr = NULL;

    i = 0;
    dot_flag = 0;
    current_char = function[i];
    next_char = function[i+1];
    while (isdigit(next_char) || next_char == DOT){

        if (next_char == DOT && dot_flag == 1){
            error = SYNTAX_ERR;
            return -1;
        }

        else if (next_char == DOT){
            dot_flag = 1;
        }

        ++i;
        next_char = function[i+1];
        current_char = function[i];
    }

    if(current_char == DOT){
        error = SYNTAX_ERR;
        return -1;
    }

    number = malloc(sizeof(char) * i+1);
    if (!number){
        error = MEMORY_ERR;
        return -1;
    }

    strncpy(number, function, i+1);
    num_value = strtod(number, ptr);
    free(number);

    num_value *= *sign;

    if(!queue_record_init(&new_record, NULL, 0, num_value, NULL)){
        return -1;
    }
            
    queue_enqueue(queue, &new_record);

    if(check_multiply(next_char)){
        if(!parse_operator(queue, stack, MULTIPLY, sign)){
            return 0;
        }
    }
    return i;
}

int queue_record_init(struct queue_record *queue_record, double *coef_values, size_t arr_len, double value, operation operator){
    if (!queue_record){
        error  = POINTER_ERR;
        return 0;
    }
    
    queue_record->coef_values = coef_values;
    queue_record->operator = operator;
    queue_record->arr_len = arr_len;
    queue_record->value = value;

    return 1;
}

int control_function(char *function){
    char *banned_chars = "^></:,";

    if(strpbrk(function, banned_chars)){
        error = SYNTAX_ERR;
        return 0;
    }

    return 1;
}

struct function *function_alloc(size_t coef_count){
    struct function *new_function = NULL;

    new_function = malloc(sizeof(*new_function));

    if (!new_function){
        error = MEMORY_ERR;
        return NULL;
    }

    if(function_init(new_function, coef_count) == 0){
        free(new_function);
        return NULL;
    }

    return new_function;
}

int function_init(struct function *function, size_t coef_count){
    if(!function){
        error = POINTER_ERR;
        return 0;
    }

    function->coefs = calloc(sizeof(double), coef_count);
    if (!function->coefs){
        error = MEMORY_ERR;
        return 0;
    }
    
    return 1;
}

void function_deinit(struct function *function){
    if(!function){
        return;
    }

    free(function->coefs);
    function->coefs = NULL;

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