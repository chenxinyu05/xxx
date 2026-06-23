#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_set>

#include "xxx_base/xxx_string.h"
#include "xxx_base/xxx_array.h"
#include "xxx_base/xxx_i32array.h"
#include "xxx_base/xxx_stack.h"
#include "xxx_base/xxx_i32stack.h"

int main() {
    xxx_string_t s;
    xxx_string_init(&s);
    xxx_string_assign(&s, "abcde", 5);
    xxx_string_append(&s, xxx_string_cstr(&s), xxx_string_length(&s));
    printf("%zu %s\n", xxx_string_length(&s), xxx_string_cstr(&s));
}

