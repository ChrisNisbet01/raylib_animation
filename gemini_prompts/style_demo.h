#pragma once

#include <stdbool.h>
#include <stddef.h>

/*
 * ## 3. Variables and Types
 * - Enums Over #define: Grouping related constants with a type-safe enum.
 */
typedef enum
{
    ERR_NONE = 0,
    ERR_INVALID_PARAM,
    ERR_RESOURCE_UNAVAILABLE,
} ErrorCode;


/*
 * ## 6. Functions
 * - Const in Prototypes: Note `int count` instead of `int const count`. The
 *   `const` is an implementation detail for the .c file.
 * - Parameter Formatting: Demonstrates all three parameter layout rules.
 * - Return Type Position: Return type is on its own line.
 */

/* Example 1: Fits on one line */
void
process_short_list(int const * items, int count);

/* Example 2: Parameters moved to the next line */
ErrorCode
find_item_in_database(
    char const * query, size_t * out_item_id, int const * context
);

/* Example 3: Each parameter on its own line */
void
update_complex_entity(
    int entity_id,
    char const * new_name,
    bool should_reindex,
    bool is_urgent
);

