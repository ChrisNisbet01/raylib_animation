#include "style_demo.h"

#include <stdbool.h>
#include <stdio.h>

/*
 * ## 6. Functions
 * - Extract Conditional Blocks: This function exists to be called from a
 *   conditional block, keeping the calling function cleaner.
 */
static void
log_error(ErrorCode const code)
{
    /* ## 4. Pointers and Memory - Pointer Declaration */
    char const * error_string = "Unknown Error";

    /* ## 1. Formatting - Whitespace After Declarations */

    /* ## 2. Braces and Control Structures - Explicit Comparisons */
    if (code == ERR_INVALID_PARAM)
    {
        error_string = "Invalid Parameter";
    }

    printf("Logged Error: %s\n", error_string);
}

/*
 * ## 5. Const Correctness
 * - Use East Const: `int const count`
 * - Use Const Aggressively: Parameters that are not modified are const.
 *
 * ## 6. Functions
 * - The `const` on `count` is present here in the implementation, but was
 *   omitted from the prototype in the header file.
 */
void
process_short_list(int const * const items, int const count)
{
    printf("Processing %d items.\n", count);
    /* ## 2. Braces and Control Structures - Always Use Braces */
    /* ## 4. Pointers and Memory - Array Notation */
    for (int i = 0; i < count; ++i)
    {
        printf("Item: %d\n", items[i]);
    }
}

ErrorCode
find_item_in_database(
    char const * query, size_t * out_item_id, int const * const context)
{
    printf("Finding '%s' with context %d.\n", query, *context);
    *out_item_id = 123; /* ## 4. Pointers and Memory - Pointer Dereferencing */
    return ERR_NONE;
}

void
update_complex_entity(
    int const entity_id,
    char const * const new_name,
    bool const should_reindex,
    bool const is_urgent
)
{
    printf("Updating entity %d\n", entity_id);
}


/*
 * ## 6. Functions
 * - Return Type Position: `int` is on its own line above `main`.
 */
int
main(void)
{
    /* ## 5. Const Correctness - Use Const Aggressively */
    int const initial_value = 42;

    /* ## 1. Formatting - Whitespace After Declarations */

    /* ## 3. Variables and Types - Declaration Scope (declared where needed) */
    int items[] = {1, 2, 3};
    size_t found_id = 0;

    /* ## 3. Variables and Types - Explanatory Variables */
    bool const is_critical_search = initial_value > 40;
    bool const has_valid_context = true;

    /*
     * ## 2. Braces and Control Structures
     * - Brace Placement: Opening brace is on its own line.
     */
    if (is_critical_search && has_valid_context)
    {
        /* ## 6. Functions - Extract Conditional Blocks */
        ErrorCode const code = find_item_in_database("test", &found_id, &initial_value);

        if (code != ERR_NONE)
        {
            log_error(code);
        }
    }

    process_short_list(items, 3);

    return 0;
}
