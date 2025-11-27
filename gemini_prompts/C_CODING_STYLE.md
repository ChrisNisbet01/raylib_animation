# C Coding Style Guide

## 1. Formatting

*   **Indentation:** Use 4 spaces per indentation level.
*   **Tabs vs. Spaces:** Always use spaces. Do not use hard tabs.
*   **Whitespace After Declarations:** Place a blank line after a variable declaration or a block of related declarations to visually separate the setup from the subsequent logic.
*   **Line Length and Wrapping:**
    *   **Limit:** Strive to keep lines under 100 characters. A line may extend up to 120 characters only if it contains a long, unbreakable element like a string literal.
    *   **Simple Break:** For a long assignment statement, the preferred way to break the line is after the assignment operator (`=`). The new line should be indented by one level (4 spaces).

        *Example:*
        ```c
        SomeVeryLongType const my_variable =
            get_value_from_a_function_with_a_very_long_name();
        ```

    *   **Complex Break (Function Arguments):** If the line is still too long after a simple break, and the right-hand side is a function call, place each function argument on its own line. The arguments should be indented one level (4 spaces) relative to the function call. The closing parenthesis and semicolon should follow on a new line, aligned with the start of the assignment.

        *Example:*
        ```c
        SomeVeryLongType const my_variable =
            some_function_with_a_long_name(
                first_argument_with_a_very_long_name,
                second_argument_with_a_very_long_name,
                third_argument
            );
        ```
    *   **Chained Conditionals:** When a long line is a series of chained logical conditions (e.g., using `&&` or `||`), the line should be broken *before* the operator. The operator on the new line should be aligned with the first operand on the line above.

        *Good:*
        ```c
        bool const is_valid = this_is_the_first_condition
                              && this_is_the_second_condition
                              && another_condition;
        ```

        *Bad:*
        ```c
        bool const is_valid = this_is_the_first_condition &&
                              this_is_the_second_condition && another_condition;
        ```

## 2. Braces and Control Structures

*   **Brace Placement:** The opening curly brace of a block is always placed on its own line.

    *Good:*
    ```c
    if (condition)
    {
        do_something();
    }
    ```
    *Bad:*
    ```c
    if (condition) {
        do_something();
    }
    ```

*   **Always Use Braces:** Always use curly braces for the bodies of `if`, `else`, `for`, `while`, and `do` statements, even if the body contains only a single line.

*   **Explicit Comparisons:** Do not treat integers or pointers as implicit booleans in conditional statements. Always make the comparison explicit. This makes the code's intent clearer and prevents subtle bugs.

    *Good (integer):*
    ```c
    int count = get_count();
    if (count != 0)
    {
        /* ... */
    }
    ```

    *Good (pointer):*
    ```c
    char * p = get_string();
    if (p != NULL)
    {
        /* ... */
    }
    ```

## 3. Variables and Types

*   **Declaration Scope:** Declare variables in the narrowest possible scope, typically where they are first needed. Do not declare all variables at the top of a function block.

*   **Explanatory Variables:** For long or complex expressions, break down the components into well-named intermediate variables. This dramatically improves readability and simplifies debugging.

    *Bad:*
    ```c
    if (!(this_error || that_error || another_error) && some_new_operation())
    {
        // ...
    }
    ```

    *Good:*
    ```c
    bool const had_error = this_error || that_error || another_error;
    bool const op_success = some_new_operation();

    if (!had_error && op_success)
    {
        // ...
    }
    ```

*   **Parentheses in Expressions:** Only use parentheses in expressions when required to enforce a specific order of operations. For simple boolean assignments, parentheses are unnecessary. If an expression becomes so complex that it requires multiple levels of parentheses for clarity, prefer breaking it down using the `Explanatory Variables` rule instead.

    *Good (Omitted where not needed):*
    ```c
    bool const is_adult = age >= 18;
    ```

    *Good (Required for correctness):*
    ```c
    bool const is_valid = (user_is_active && has_permissions) || is_admin;
    ```

    *Bad (Unnecessary parentheses):*
    ```c
    bool const is_adult = (age >= 18);
    ```

*   **Enums Over #define:** Prefer using `enum` to group related constants (like error codes or states) over multiple `#define` directives. This provides better type safety and is more easily understood by debuggers.

    *Bad:*
    ```c
    #define ERR_NONE 0
    #define ERR_TIMEOUT 1
    #define ERR_CLOSED 2
    ```

    *Good:*
    ```c
    typedef enum
    {
        ERR_NONE = 0,
        ERR_TIMEOUT,
        ERR_CLOSED,
    } ErrorCode;
    ```

## 4. Pointers and Memory

*   **Pointer Declaration:** When declaring pointer variables, place a space on both sides of the asterisk (`*`). For multiple levels of indirection, treat each asterisk as a separate token.

    *Good:*
    ```c
    char * buffer = NULL;
    int * * matrix = NULL;
    ```

*   **Pointer Dereferencing:** When dereferencing a pointer, place the asterisk directly next to the variable name, with no space.

    *Good:*
    ```c
    char c = *buffer;
    int i = **matrix;
    ```

*   **Array Notation:** Always use array subscript notation (`[]`) instead of pointer arithmetic to access array elements.

    *Bad:*
    ```c
    int x = *(arr + i);
    ```

    *Good:*
    ```c
    int x = arr[i];
    ```

## 5. Const Correctness

*   **Use East Const:** Always place the `const` keyword to the right of the type it modifies (known as "east const").

*   **Use Const Aggressively:** Use the `const` keyword for any variable or parameter that will not be modified after initialization. This helps the compiler optimize and prevents accidental modification of data.

    *Examples:*
    ```c
    // Constant variable
    int const MAX_USERS = 100;

    // Pointer to a constant integer
    int const * limit;

    // A constant pointer to a mutable integer
    int * const id;

    // A constant pointer to a constant integer
    int const * const version;

    // A function that does not modify its parameter
    void print_value(int const value)
    {
        // ...
    }
    ```

## 6. Functions

*   **Extract Conditional Blocks:** To keep functions small and focused, logic inside a conditional or loop block should be moved into a separate function if it is more than a few lines long.

    *Bad:*
    ```c
    void process_data(Data const * const data)
    {
        if (data->type == SPECIAL_TYPE)
        {
            // 10 lines of complex logic for SPECIAL_TYPE
            // ...
        }
    }
    ```

    *Good:*
    ```c
    void
    handle_special_type(Data const * const data)
    {
        // 10 lines of complex logic for SPECIAL_TYPE
        // ...
    }

    void
    process_data(Data const * const data)
    {
        if (data->type == SPECIAL_TYPE)
        {
            handle_special_type(data);
        }
    }
    ```

*   **Return Type Position:** The return type of a function must be on a separate line directly above the function name.

*   **Parameter Formatting:** Follow these rules for laying out function parameters, assuming a 100-character line limit.
    1.  If the function name and all parameters fit on a single line less than 100 characters, do so.
    2.  If rule 1 fails, move all parameters to the line(s) below the function name, indented by 4 spaces.
    3.  If the parameters from rule 2 still exceed 100 characters, place each parameter on its own line, indented by 4 spaces.
    4.  If parameters are not on the same line as the function name, the closing parenthesis `)` must be on its own line.

    *Examples:*
    ```c
    // Fits on one line
    void
    short_fn(int const x)
    {
        // ...
    }

    // Parameters moved to next line
    int
    longer_fn(
        int const x, char const * const message, int const count
    )
    {
        // ...
    }

    // Each parameter on its own line
    int
    really_long_fn(
        int const first_parameter,
        char const * const second_parameter,
        float const third_parameter
    )
    {
        // ...
    }
    ```

*   **Const in Prototypes:** For function *declarations* (prototypes), top-level `const` qualifiers on parameters passed by value should be omitted as they are an implementation detail.

    *Definition:*
    ```c
    int
    some_fn(int const x, char const * const s)
    {
        // ...
    }
    ```

    *Declaration (in .h file):*
    ```c
    int
    some_fn(int x, char const * s);
    ```
*   **Returning Multiple Values:**
    *   **Preference:** When a function needs to return multiple distinct data values, prefer returning a single `struct` by value over using multiple "out-parameters". This makes the function's purpose clearer and simplifies the calling code.

        *Good (Preferred for multiple data values):*
        ```c
        typedef struct
        {
            int user_id;
            char const * user_name;
        } UserInfo;

        UserInfo get_user_info(void);

        // Usage is clean and direct:
        UserInfo info = get_user_info();
        printf("User: %s (ID: %d)\n", info.user_name, info.user_id);
        ```

    *   **Exception for Failable Functions:** A common and acceptable exception is for functions that might fail. In this case, returning an `ErrorCode` enum and providing the primary result via a single out-parameter is a conventional and clear pattern.

        *Acceptable (for failable operations):*
        ```c
        ErrorCode get_primary_user_id(int * out_id);

        // Usage clearly handles the failure case:
        int user_id = 0;

        if (get_primary_user_id(&user_id) == ERR_NONE)
        {
            // ... use user_id
        }
        ```

    *   **To Avoid:** Avoid using multiple out-parameters to return multiple data values.

        *Bad (multiple out-parameters):*
        ```c
        void get_user_info_via_params(int * out_id, char const * * out_name);
        ```

## 7. Header Files

*   **Header Guards:** Use `#pragma once` instead of traditional include guards for all header files. It is more concise and less error-prone.

    *Good:*
    ```c
    #pragma once

    // ... header content ...
    ```

    *Bad:*
    ```c
    #ifndef MY_HEADER_H
    #define MY_HEADER_H

    // ... header content ...

    #endif // MY_HEADER_H
    ```

*   **Include Order:** Order `#include` directives to make dependencies clear. Headers must be grouped in the following order, with each group separated by a blank line. Within each group, headers must be sorted alphanumerically.
    1.  **Corresponding Header:** The header file that shares the same base name as the `.c` file (e.g., `my_source.c` includes `my_source.h`). This file must come first and is the only exception to the alphanumeric sorting rule.
    2.  **Project Headers:** Other headers from the same project.
    3.  **External Headers:** Headers from third-party libraries.
    4.  **System Headers:** Standard library headers.

    *Example (in `my_feature.c`):*
    ```c
    #include "my_feature.h" // 1. Corresponding header first.

    #include "api.h"        // 2. Other project headers, sorted.
    #include "utils.h"

    #include <cjson/cJSON.h> // 3. External headers, sorted.
    #include <uv.h>

    #include <stdbool.h>   // 4. System headers, sorted.
    #include <stdio.h>
    #include <stdlib.h>
    ```

*   **Stand-alone Headers:** Header files must be self-contained. They should include all other headers required for their own contents to be successfully compiled. A `.c` file should not need to include a header's dependencies.

    *Bad (`my_header.h`):*
    ```c
    // Requires the .c file to also include <stddef.h>
    void process_data(size_t size);
    ```

    *Good (`my_header.h`):*
    ```c
    #pragma once
    #include <stddef.h> // Includes its own dependency

    void process_data(size_t size);
    ```

*   **Include What You Use (IWYU):** Every `.c` and `.h` file must explicitly include headers for all types, functions, and macros it uses directly. Do not rely on a header being included transitively by another header. This makes dependencies clear and prevents compilation errors when other headers are refactored.

    *Scenario:* `my_feature.c` uses `bool` and also includes `my_feature.h`, which happens to include `<stdbool.h>`.

    *Bad (`my_feature.c`):*
    ```c
    #include "my_feature.h" // Relies on my_feature.h to provide <stdbool.h>

    void do_something(void)
    {
        bool is_done = false; // This will break if my_feature.h changes.
    }
    ```

    *Good (`my_feature.c`):*
    ```c
    #include "my_feature.h"

    #include <stdbool.h> // Explicitly includes what it uses.

    void do_something(void)
    {
        bool is_done = false; // This code is now robust.
    }
    ```

## 8. Comments

*   **Comment Style:**
    *   Always use C-style block comments (`/* ... */`). Do not use C++ style line comments (`// ...`).
    *   Sentences within comments must begin with a capital letter and end with appropriate punctuation.

*   **Single-Line Comments:** Keep the entire comment on one line if it is brief.
    ```c
    /* This is a single-line comment. */
    int x = 1;
    ```

*   **Multi-Line Comments:** For comments that span multiple lines, the opening `/*` and closing `*/` should be on their own lines. Each intermediate line must start with an asterisk aligned under the first, followed by a single space.

    *Good:*
    ```c
    /*
     * This is a multi-line comment that explains something complex.
     * It follows the specified formatting rules.
     */
    ```
