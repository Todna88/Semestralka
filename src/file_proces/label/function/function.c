#include "function.h"

struct function *process_function(char *function, char *function_type, const struct generals *generals){
    struct function *processed_function = NULL;
    char splitter[2] = "=";
    char *token;
    size_t i;

    if(!function || !generals){
        error = POINTER_ERR;
        goto err;
    }

    if (!control_function(function)){
        goto err;
    }

    processed_function = function_alloc(generals->variables_count);
    if(!processed_function){
        goto err;
    }

    token = strtok(function, splitter);
    i = 0;

    while(token != NULL){
        if (search_variables(token, generals)){
            if(!parse_artithmetic_expression(function, processed_function->coefs, generals)){
                function_dealloc(&processed_function);
                goto err;
            }
            token = strtok(NULL, splitter);
        }

        else{
            token = strtok(NULL, splitter);
        }

        ++i;
    }

    if ((i == 0) || (i > 2)){
        function_dealloc(&processed_function);
        error = SYNTAX_ERR;
        goto err;
    }

    if(!check_type_of_task(processed_function, function_type, generals->variables_count)){
        function_dealloc(&processed_function);
        goto err;
    }

    return processed_function;

    err:
        return NULL;
}

int check_multiply(const char next_char){
    if (!is_right_bracket(next_char) && !is_operator(next_char) && !isdigit(next_char) && !(next_char == '\0')){
        return 1;
    }

    return 0;  
}

int parse_variable(struct stack *output_stack, struct stack *input_stack, const char *function, const struct generals *generals){
    size_t i;
    char next_char;
    char *variable_name = NULL;
    int variable_id;
    struct output_record new_record;
    double *coefs = NULL;

    if (!output_stack || !input_stack || !function || !generals){
        error = POINTER_ERR;
        goto err;
    }
    
    i = 0;
    next_char = function[i + 1];

    while ((!is_operator(next_char)) && (!is_left_bracket(next_char)) && (!is_right_bracket(next_char) && next_char)){
        ++i;
        next_char = function[i + 1];
    }

    variable_name = calloc(sizeof(char), i + 2);
    if (!variable_name){
        error = MEMORY_ERR;
        goto err;
    }
    
    strncpy(variable_name, function, i + 1);

    variable_id = get_variable(variable_name, generals);
    if(variable_id == -1){
        printf("Unknown variable \'%s\'!\n", variable_name);
        free(variable_name);
        error = VARIABLE_ERR;
        goto err;
    }

    free(variable_name);

    coefs = calloc(sizeof(double), generals->variables_count);
    if (!coefs){
        error = MEMORY_ERR;
        goto err;
    }

    coefs[variable_id] = 1.0;

    if(!output_record_init(&new_record, coefs, generals->variables_count, 0)){
        free(coefs);
        goto err;
    }
    
    if(!stack_push(output_stack, &new_record)){
        free(coefs);
        goto err;
    }

    if (is_left_bracket(next_char)){
        if(!parse_operator(output_stack, input_stack, MULTIPLY)){
            goto err;
        }
    }

    return i + 1;

    err:
        return -1;
}

int parse_operator(struct stack *output_stack, struct stack *input_stack, const char current_char){
    char stack_out, current_char_cpy, multiply;
    int stack_priority, current_priority;
    operation operator;
    struct output_record operand_1, operand_2, new_record;

    if (!output_stack || !input_stack){
        error = POINTER_ERR;
        return 0;
    }

    if(check_empty(input_stack)){
        goto push;
    }

    if(!stack_head(input_stack, &stack_out)){
        goto err;
    }

    if (is_left_bracket(stack_out)){
        goto push;
    }

    stack_priority = get_priority(stack_out);
    current_priority = get_priority(current_char);

    if(stack_priority == -1 || current_priority == -1){
        error = SYNTAX_ERR;
        return 0;
    }

    while (stack_priority >= current_priority){

        if(!stack_pop(input_stack, &stack_out)){
            goto err;
        }

        operator = get_operation(stack_out);

        if(check_empty(output_stack)){
            error = SYNTAX_ERR;
            goto err;
        }

        if(!stack_pop(output_stack, &operand_2)){
            goto err;
        }

        if(check_empty(output_stack)){
            dealloc_record(&operand_2);
            error = SYNTAX_ERR;
            goto err;
        }

        if(!stack_pop(output_stack, &operand_1)){
            dealloc_record(&operand_2);  
            goto err;
        }

        if (!evaluate(&operand_1, &operand_2, output_stack, operator)){
            goto err;
        }

        if(check_empty(input_stack)){
            break;
        }

        if(!stack_head(input_stack, &stack_out)){
            return 0;
        }

        if (is_left_bracket(stack_out)){
            break;
        }

        stack_priority = get_priority(stack_out);
    }

    push:
    if (check_empty(output_stack)){
        if(check_unary_operator(current_char, output_stack, input_stack) == -1){
            goto err;
        }
        return 1;
    }
    
    if (current_char == MINUS){
        current_char_cpy = PLUS;

        if(!stack_push(input_stack, &current_char_cpy)){
            goto err;
        }

        output_record_init(&new_record, NULL, 0, -1);
        if(!stack_push(output_stack, &new_record)){
            goto err;
        }

        multiply = MULTIPLY;
        if(!stack_push(input_stack, &multiply)){
            goto err;
        }
    }

    else{
        current_char_cpy = current_char;

        if(!stack_push(input_stack, &current_char_cpy)){
            goto err;
        }
    }

    return 1;

    err:
        return 0;
}

int check_unary_operator(const char operator, struct stack *output_stack, struct stack *input_stack){
    struct output_record new_record;
    char multiply = MULTIPLY;

    switch (operator){
        case PLUS:
            return 1;
            break;

        case MINUS:
            output_record_init(&new_record, NULL, 0, -1);
            if(!stack_push(output_stack, &new_record)){
                return -1;
            }

            if(!stack_push(input_stack, &multiply)){
                return -1;
            }

            return 1;
            break;

        case MULTIPLY:
            error = SYNTAX_ERR;
            return -1;
            break;
        default:
            return 0;
            break;            
    }
}

int parse_brackets(struct stack *output_stack, struct stack *input_stack, const char current_char){
    char stack_out;
    struct output_record operand_1, operand_2;
    operation operate = NULL;

    if (!output_stack || !input_stack){
        error = POINTER_ERR;
        goto err;
    }

    if (check_empty(input_stack)){
        error = SYNTAX_ERR;
        goto err;
    }
    
    if(!stack_pop(input_stack, &stack_out)){
        goto err;
    }

    while (!is_left_bracket(stack_out)){

        operate = get_operation(stack_out);

        if(!operate){
            error = SYNTAX_ERR;
            goto err;
        }

        if(!stack_pop(output_stack, &operand_2)){
            goto err;
        }

        if (check_empty(output_stack)){
            dealloc_record(&operand_2);
            error = SYNTAX_ERR;
            goto err;
        }

        if(!stack_pop(output_stack, &operand_1)){
            dealloc_record(&operand_2);
            goto err;
        }

        if (!evaluate(&operand_1, &operand_2, output_stack, operate)){
            goto err;
        }
        
        if(!stack_pop(input_stack, &stack_out)){
            goto err;
        }

    }

    if (!is_equal_bracket(stack_out, current_char)){
        error = SYNTAX_ERR;
        goto err;
    }
    return 1;

    err:
        return 0;
}

int evaluate(struct output_record *operand_1, struct output_record *operand_2, struct stack *output_stack, operation operator){
    if (!operand_1 || !operand_2 || !output_stack || !operator){
        error = POINTER_ERR;
        goto err;
    }
    
    if (operator == mul){
        if (operand_1->coef_values && operand_2->coef_values){
            dealloc_record(operand_1);
            dealloc_record(operand_2);
            error = SYNTAX_ERR;
            goto err;
        }

        if (operand_1->coef_values){
            if(!array_mul(operand_1->coef_values, operand_2->value, operand_1->arr_len)){
                dealloc_record(operand_1);
                goto err;
            }
            if(!stack_push(output_stack, operand_1)){
                dealloc_record(operand_1);
                goto err;
            }
            return 1;
        }

        if (operand_2->coef_values){
            if(!array_mul(operand_2->coef_values, operand_1->value, operand_2->arr_len)){
                dealloc_record(operand_2);
                goto err;
            }
            if(!stack_push(output_stack, operand_2)){
                dealloc_record(operand_2);
                goto err;
            }
            return 1;
        }

        operand_1->value = operator(operand_1->value, operand_2->value);
        if(!stack_push(output_stack, operand_1)){
            goto err;
        }
        return 1;
    }

    else{
        if (operand_1->coef_values && operand_2->coef_values){
            if(!array_sum(operand_1->coef_values, operand_2->coef_values, operand_1->arr_len)){
                dealloc_record(operand_1);
                dealloc_record(operand_2);
                goto err;
            }

            dealloc_record(operand_2);

            if(!stack_push(output_stack, operand_1)){
                dealloc_record(operand_1);
                goto err;
            }
            return 1;
        }

        if (operand_1->coef_values){
            if(!stack_push(output_stack, operand_1)){
                dealloc_record(operand_1);
                goto err;
            }
            return 1;
        }

        if (operand_2->coef_values){
            if(!stack_push(output_stack, operand_2)){
                dealloc_record(operand_2);
                goto err;
            }
            return 1;
        }

        operand_1->value = operator(operand_1->value, operand_2->value);
        if(!stack_push(output_stack, operand_1)){
            goto err;
        }
        return 1;
    }

    err:
        return 0;   
}

int parse_number(struct stack *input_stack, struct stack *output_stack, const char *function){
    char current_char, previous_char, dot_flag;
    struct output_record new_record;
    size_t i;
    char *number = NULL;
    double num_value;
    char *ptr;

    if (!input_stack || !output_stack || !function){
        error = POINTER_ERR;
        goto err;
    }

    i = 0;
    dot_flag = 0;
    current_char = function[i];
    while (isdigit(current_char) || current_char == DOT){

        if (current_char == DOT && dot_flag == 1){
            error = SYNTAX_ERR;
            goto err;
        }

        else if (current_char == DOT){
            dot_flag = 1;
        }

        if (dot_flag == 1 && i == 0){
            error = SYNTAX_ERR;
            goto err;
        }

        ++i;
        current_char = function[i];
        previous_char = function[i - 1];

        if (previous_char == DOT && !isdigit(current_char)){
            error = SYNTAX_ERR;
            goto err;
        }
    }

    number = calloc(sizeof(char), i + 1);
    if (!number){
        error = MEMORY_ERR;
        goto err;
    }

    strncpy(number, function, i);
    num_value = strtod(number, &ptr);
    free(number);

    if(!output_record_init(&new_record, NULL, 0, num_value)){
        goto err;
    }
            
    if(!stack_push(output_stack, &new_record)){
        goto err;
    }

    if(check_multiply(current_char)){
        if(!parse_operator(output_stack, input_stack, MULTIPLY)){
            goto err;
        }
    }
    return i;

    err:
        return -1;
}

int output_record_init(struct output_record *output_record, double *coef_values, size_t arr_len, double value){
    if (!output_record){
        error = POINTER_ERR;
        return 0;
    }
    
    output_record->coef_values = coef_values;
    output_record->arr_len = arr_len;
    output_record->value = value;

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
        if (left_bracket == '['){
            return 1;
        }

        return 0;
        
        break;
    }

    return 0; 
}

void delete_spaces(char *line){
    size_t count, i;

    count = i = 0;
    if (!line){
        return;
    }
    
    for (i = 0; line[i]; ++i){
        if (!isspace(line[i])){
            line[count] = line[i];
            ++count;
        }
    }

    line[count] = '\0';
    return;
}

void func_print_coefs(struct function *function, struct generals *generals){
    size_t i;

    if (!function || !generals){
        return;
    }

    for (i = 0; i < generals->variables_count; ++i){
        printf("Function coeficient of variable %s is %f\n", generals->variables[i], function->coefs[i]);
    }

    return;   
}

int check_stacks(double *coef_array, struct stack *input_stack, struct stack *output_stack, size_t var_count){
    char stack_out;
    struct output_record operand_1, operand_2, result;
    operation operator = NULL;

    if(!coef_array || !input_stack || !output_stack){
        error = POINTER_ERR;
        goto err;
    }

    while (!check_empty(input_stack)){

        if(check_empty(output_stack)){
            error = SYNTAX_ERR;
            goto err;
        }

        if(!stack_pop(output_stack, &operand_2)){
            goto err;
        }

        if(check_empty(output_stack)){
            dealloc_record(&operand_2);
            error = SYNTAX_ERR;
            goto err;
        }

        if(!stack_pop(output_stack, &operand_1)){
            dealloc_record(&operand_2);
            goto err;
        }

        if(!stack_pop(input_stack, &stack_out)){
            dealloc_record(&operand_1);
            dealloc_record(&operand_2);
            goto err;
        }

        operator = get_operation(stack_out);

        if(!operator){
            dealloc_record(&operand_1);
            dealloc_record(&operand_2);
            goto err;
        }

        if(!evaluate(&operand_1, &operand_2, output_stack, operator)){
            goto err;
        }
    }

    if(check_empty(output_stack)){
        error = SYNTAX_ERR;
        goto err;
    }

    if(!stack_pop(output_stack, &result)){
        goto err;
    }

    if(!check_empty(output_stack)){
        error = SYNTAX_ERR;
        free(result.coef_values);
        goto err;
    }

    if (!result.coef_values){
        return 1;
    }

    memcpy(coef_array, result.coef_values, var_count * sizeof(double));

    free(result.coef_values);

    return 1;

    err:
    return 0;
}

int parse_artithmetic_expression(char *expression, double *coef_array, const struct generals *generals){
    struct stack *input_stack = NULL;
    struct stack *output_stack = NULL;
    char current_char, next_char;
    int i, parse_number_result, parse_var_result, unary_operator_result;

    if(!expression || !coef_array || !generals){
        error = POINTER_ERR;
        return 0;
    }

    input_stack = stack_alloc(sizeof(char));
    if(!input_stack){
        error = MEMORY_ERR;
        return 0;
    }

    output_stack = stack_alloc(sizeof(struct output_record));
    if(!input_stack){
        error = MEMORY_ERR;
        stack_dealloc(&input_stack);
        return 0;
    }

    i = 0;

    delete_spaces(expression);

    while(expression[i] != '\0'){

        current_char = expression[i];
        next_char = expression[i + 1];

        if (isdigit(current_char) || current_char == DOT){
            parse_number_result = parse_number(input_stack, output_stack, &expression[i]);

            if (parse_number_result == -1){
                goto err;
            }
            i += parse_number_result;
            continue;
        }

        if (is_left_bracket(current_char)){
            if(!stack_push(input_stack, &current_char)){
                goto err;
            }

            unary_operator_result = check_unary_operator(next_char, output_stack, input_stack);

            if (unary_operator_result == -1){
                goto err;
            }

            else if(unary_operator_result == 0){
                ++i;
                continue;
            }

            else{
                i += 2;
                continue;
            }
        }

        if (is_right_bracket(current_char)){
            if(!parse_brackets(output_stack, input_stack, current_char)){
                goto err;
            }

            if (is_operator(next_char) || is_right_bracket(next_char) || (next_char == '\0')){
                ++i;
                continue;
            }

            else{
                if(!parse_operator(output_stack, input_stack, MULTIPLY)){
                    goto err;
                }
                ++i;
                continue;
            }
        }

        if (is_operator(current_char)){
            if(!parse_operator(output_stack, input_stack, current_char)){
                goto err;
            }
            ++i;
            continue;
        }

        parse_var_result = parse_variable(output_stack, input_stack, &expression[i], generals);
        if (parse_var_result == -1){
            goto err;
        }

        i += parse_var_result;
    }

    if (!check_stacks(coef_array, input_stack, output_stack, generals->variables_count)){
        goto err;
    }

    stack_dealloc(&output_stack);
    stack_dealloc(&input_stack);
    return 1;

    err:
    record_arrays_dealloc(output_stack);
    stack_dealloc(&output_stack);
    stack_dealloc(&input_stack);
    return 0;
}

void record_arrays_dealloc(struct stack *output_stack){
    struct output_record record;
    if (!output_stack){
        return;
    }

    while (!check_empty(output_stack)){
        stack_pop(output_stack, &record);

        dealloc_record(&record);    
    }

    return;   
}

void dealloc_record(struct output_record *record){
    if (!record){
        return;
    }

    if (record->coef_values){
        free(record->coef_values);
        record->coef_values = NULL;
        return;
    }
    
    return; 
}

int check_type_of_task(struct function *function, char *function_type, const size_t var_count){
    size_t i;

    if(!function || !function_type){
        error = POINTER_ERR;
        return 0;
    }

    if (!strcmp(function_type, MIN_TYPE)){
        for (i = 0; i < var_count; ++i){
            function->coefs[i] = function->coefs[i] * -1.0;
        }
    }

    return 1;
}