# GLaDOS Part 2 - Extended Backus-Naur Form (EBNF)

## Grammar Definition

```ebnf
(* Program Structure *)
program          ::= statement*

statement        ::= function_def
                   | assignment
                   | return_stmt
                   | if_stmt
                   | expr_stmt

(* Function Definition *)
function_def     ::= "fn" identifier "(" parameter_list? ")" "{" statement* "}"
parameter_list   ::= identifier ("," identifier)*

(* Statements *)
assignment       ::= identifier "=" expression ";"?
return_stmt      ::= "return" expression ";"?
if_stmt          ::= "if" "(" expression ")" "{" statement* "}"
                     ("else" "{" statement* "}")?
expr_stmt        ::= expression ";"?

(* Expressions - with precedence levels *)
expression       ::= comparison

comparison       ::= term (comparison_op term)?
comparison_op    ::= "==" | "!=" | "<=" | ">=" | "<" | ">"

term             ::= factor (additive_op factor)*
additive_op      ::= "+" | "-"

factor           ::= primary (multiplicative_op primary)*
multiplicative_op ::= "*" | "/"

primary          ::= function_call
                   | int_literal
                   | bool_literal
                   | identifier
                   | "(" expression ")"

(* Function Call *)
function_call    ::= identifier "(" argument_list? ")"
argument_list    ::= expression ("," expression)*

(* Literals *)
int_literal      ::= "-"? digit+
bool_literal     ::= "true" | "false"

(* Identifiers *)
identifier       ::= (letter | "_") (letter | digit | "_")*

(* Lexical Elements *)
letter           ::= "a" | "b" | "c" | ... | "z" | "A" | "B" | ... | "Z"
digit            ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"

(* Whitespace (ignored) *)
whitespace       ::= " " | "\t" | "\n" | "\r"
```

## Operator Precedence (High to Low)

1. **Primary**: Literals, identifiers, function calls, parenthesized expressions
2. **Multiplicative**: `*`, `/`
3. **Additive**: `+`, `-`
4. **Comparison**: `==`, `!=`, `<`, `>`, `<=`, `>=`

## Language Features

### Keywords
- `fn` - Function definition
- `return` - Return statement
- `if` - Conditional statement
- `else` - Alternative branch
- `true`, `false` - Boolean literals

### Operators
- **Arithmetic**: `+`, `-`, `*`, `/`
- **Comparison**: `==`, `!=`, `<`, `>`, `<=`, `>=`
- **Assignment**: `=`

### Data Types
- **Integer**: Signed integers (e.g., `42`, `-10`)
- **Boolean**: `true` or `false`

## Example Programs

### Hello World (via function)
```glados
fn main() {
    return 42;
}
```

### Factorial Function
```glados
fn factorial(n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

fn main() {
    return factorial(5);
}
```

### Variable Assignment
```glados
fn main() {
    x = 10;
    y = 20;
    result = x + y;
    return result;
}
```

## Notes

1. **Semicolons are optional** after statements
2. **Whitespace is ignored** except for separating tokens
3. **Entry point**: Program must have a `main()` function with no parameters
4. **No top-level code**: All code must be inside functions
5. **Type system**: Dynamically typed (Int and Bool)
