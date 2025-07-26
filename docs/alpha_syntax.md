## III. The Alpha Language

This section describes the syntax of the Alpha programming language. Alpha is a dynamically-typed language that uses a block-structured approach.

### A. Lexical Structure

*   **Character Set:**  Alpha uses the UTF-8 character set.
*   **Whitespace:**  Whitespace characters (spaces, tabs, newlines) are generally ignored, except to separate tokens.
*   **Comments:**
    *   Single-line comments: Start with `//` and continue to the end of the line.  Example: `// This is a comment.`
    *   Multi-line comments: Start with `/*` and end with `*/`.  Example: `/* This is a multi-line comment. */`
*   **Identifiers:**
    *   An identifier is a sequence of letters, digits, and underscores, starting with a letter or underscore.
    *   Example: `myVariable`, `_count`, `result123`
    *   Identifiers can be of any length, but only the first 256 characters are significant. Keywords are reserved and cannot be used as identifiers.
*   **Keywords:** The following words are reserved keywords in Alpha and cannot be used as identifiers:
    `and`, `break`, `do`, `else`, `elseif`, `end`, `false`, `for`, `function`, `global`, `if`, `in`, `local`, `not`, `nil`, `or`, `repeat`, `return`, `then`, `true`, `until`, `while`
*   **Operators:**
    *   Arithmetic: `+`, `-`, `*`, `/`, `%`, `^` (exponentiation)
    *   Relational: `==`, `!=`, `<`, `>`, `<=`, `>=`
    *   Logical: `and`, `or`, `not`
    *   Assignment: `=`, `+=`, `-=`, `*=`, `/=`, `%=`
    *   Concatenation: `..` (for strings)
*   **Operator Precedence and Associativity:**

    | Operator          | Precedence | Associativity |
    |-------------------|------------|---------------|
    | `^`               | 8          | Right         |
    | `*`, `/`, `%`     | 7          | Left          |
    | `+`, `-`          | 6          | Left          |
    | `..`              | 5          | Right         |
    | `<`, `<=`, `>`, `>=` | 4          | Left          |
    | `==`, `!=`        | 3          | Left          |
    | `and`             | 2          | Left          |
    | `or`              | 1          | Left          |
    | `=` (assignment) | 0          | Right         |

*   **Literals:**
    *   **Number Literals:** A sequence of digits, possibly with a decimal point.  Example: `123`, `0`, `-42`, `3.14`, `-0.5`, `1.0`
    *   **String Literals:** A sequence of characters enclosed in double quotes.  Example: `"Hello, world!"`, `"This is a string."`
        *   Escape sequences supported within string literals: `\n` (newline), `\t` (tab), `\\` (backslash), `\"` (double quote).
    *   **Table Literals:** Tables are created using curly braces `{}` and key-value pairs separated by commas. Example: `{ "name": "John", "age": 30 }`
    *   **Boolean Literals:** `true`, `false`
    *   **Nil Literal:** `nil`

### B. Data Types

Alpha supports the following data types. Note that types are associated with *values* at runtime, not with variables in the code.

*   Number (Integers and Floating-Point): Represents numeric values.
*   String: Represents sequences of characters.
*   Table: Associative arrays (dictionaries).
    *   Tables are indexed using strings or numbers.
    *   Tables can hold values of different types (heterogeneous).
*   Boolean: Represents truth values (`true` or `false`).
*   Nil: Represents the absence of a value.

### C. Program Structure

An Alpha program is a sequence of statements.  There is no required `program { ... }` block. The statements are executed sequentially from the beginning of the file.

```alpha
// Alpha program
variable1 = 10;
print(variable1);
