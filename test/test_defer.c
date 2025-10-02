#include "../src/defer.h"
#include "unity.h"
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// `__block` is Clang-only, GCC doesn't need it
#ifndef __block
#define __block
#endif

void setUp(void) {}
void tearDown(void) {}

void test_defer_basic(void) {
    __block int executed = 0;
    {
        defer({ executed = 1; });
    }
    TEST_ASSERT_EQUAL(1, executed);
}

void test_defer_reverse_order(void) {
    __block int a = 0;
    __block int b = 0;
    __block int c = 0;
    {
        defer({ a = 1; });
        defer({ b = 2; });
        defer({ c = 3; });
    }
    TEST_ASSERT_EQUAL(1, a);
    TEST_ASSERT_EQUAL(2, b);
    TEST_ASSERT_EQUAL(3, c);
}

void test_defer_early_return_helper(int *counter) {
    defer({ (*counter)++; });
    if (1)
        return;
    (*counter) += 100; // unreachable
}

void test_defer_early_return(void) {
    int counter = 0;
    test_defer_early_return_helper(&counter);
    TEST_ASSERT_EQUAL(1, counter);
}

void test_defer_nested_scopes(void) {
    __block int order = 0;
    __block int first = 0;
    __block int second = 0;
    __block int third = 0;
    {
        defer({ first = ++order; });
        {
            defer({ second = ++order; });
        }
        defer({ third = ++order; });
    }
    TEST_ASSERT_EQUAL(1, second); // inner scope defer executes first
    TEST_ASSERT_EQUAL(2, third);  // last declared in outer scope
    TEST_ASSERT_EQUAL(3, first);  // first declared in outer scope
}

// simulating file handle
void test_defer_resource_cleanup(void) {
    int *resource = (int *)malloc(sizeof(int));
    __block int cleanup_called = 0;
    {
        defer({
            free(resource);
            cleanup_called = 1;
        });
        *resource = 42;
        TEST_ASSERT_EQUAL(42, *resource);
    }
    TEST_ASSERT_EQUAL(1, cleanup_called);
}

void test_defer_conditional(void) {
    __block int cleanup_a = 0;
    __block int cleanup_b = 0;
    {
        int condition = 1;
        if (condition) {
            defer({ cleanup_a = 1; });
        } else {
            defer({ cleanup_b = 1; });
        }
    }
    TEST_ASSERT_EQUAL(1, cleanup_a);
    TEST_ASSERT_EQUAL(0, cleanup_b);
}

void test_defer_variable_capture(void) {
    __block int value = 10;
    {
        defer({ value = value * 2; });
        value = 20;
    }
    // in Clang blocks, variables are captured by reference
    // so the defer sees value=20 and doubles it to 40
    // in GCC, the block has access to the same scope
    TEST_ASSERT_EQUAL(40, value);
}

void test_defer_multiple_statements(void) {
    __block int a = 0;
    __block int b = 0;
    __block int c = 0;
    {
        defer({
            a = 1;
            b = 2;
            c = 3;
        });
    }
    TEST_ASSERT_EQUAL(1, a);
    TEST_ASSERT_EQUAL(2, b);
    TEST_ASSERT_EQUAL(3, c);
}

void test_defer_in_loop(void) {
    __block int counter = 0;
    for (int i = 0; i < 3; i++) {
        defer({ counter++; });
    }
    // each defer is scoped to the for loop iteration
    // so they should all execute, one per iteration
    TEST_ASSERT_EQUAL(3, counter);
}

void test_defer_with_goto_before(void) {
    __block int executed = 0;
    goto skip;
    {
        defer({ executed = 1; });
    }
skip:
    // defer never declared, should not execute
    TEST_ASSERT_EQUAL(0, executed);
}

void test_defer_pointer_access(void) {
    __block int value = 100;
    __block int result = 0;
    {
        int *ptr = &value;
        defer({ result = value; }); // access value directly, not through ptr
        *ptr = 200;
    }
    TEST_ASSERT_EQUAL(200, result);
}

void test_defer_same_variable_multiple_times(void) {
    __block int counter = 0;
    {
        defer({ counter += 1; });
        defer({ counter += 10; });
        defer({ counter += 100; });
    }
    TEST_ASSERT_EQUAL(111, counter);
}

// defer with pointer to array (arrays can't be captured in Clang blocks)
void test_defer_array_manipulation(void) {
    int arr[3] = {1, 2, 3};
    int *arr_ptr = arr; // can't use `ptr` name due to GCC macro parameter
    {
        defer({
            arr_ptr[0] = 10;
            arr_ptr[1] = 20;
            arr_ptr[2] = 30;
        });
    }
    TEST_ASSERT_EQUAL(10, arr[0]);
    TEST_ASSERT_EQUAL(20, arr[1]);
    TEST_ASSERT_EQUAL(30, arr[2]);
}

static int helper_increment(int *val) {
    (*val)++;
    return *val;
}

void test_defer_function_call(void) {
    __block int value = 0;
    {
        defer({ helper_increment(&value); });
    }
    TEST_ASSERT_EQUAL(1, value);
}

void test_defer_in_switch(void) {
    __block int executed = 0;
    int choice = 1;
    switch (choice) {
    case 1: {
        defer({ executed = 1; });
        break;
    }
    case 2: {
        defer({ executed = 2; });
        break;
    }
    }
    TEST_ASSERT_EQUAL(1, executed);
}

void test_defer_scope_not_entered(void) {
    __block int executed = 0;
    if (0) {
        defer({ executed = 1; });
    }
    TEST_ASSERT_EQUAL(0, executed);
}

void test_defer_complex_nesting(void) {
    __block int outer = 0;
    __block int inner = 0;
    {
        defer({ outer = 1; });
        {
            defer({ inner = 1; });
        }
        TEST_ASSERT_EQUAL(1, inner);
        TEST_ASSERT_EQUAL(0, outer);
    }
    TEST_ASSERT_EQUAL(1, outer);
}

void test_defer_string_operations(void) {
    char *str = (char *)malloc(20);
    __block int freed = 0;
    {
        defer({
            free(str);
            freed = 1;
        });
        strcpy(str, "test");
        TEST_ASSERT_EQUAL_STRING("test", str);
    }
    TEST_ASSERT_EQUAL(1, freed);
}

static int global_counter = 0;

void test_defer_global_variable(void) {
    global_counter = 0;
    {
        defer({ global_counter = 99; });
    }
    TEST_ASSERT_EQUAL(99, global_counter);
}

// comma breaks GCC macro
void test_defer_semicolon_separator(void) {
    __block int a = 0;
    __block int b = 0;
    {
        defer({
            a = 1;
            b = 2;
        });
    }
    TEST_ASSERT_EQUAL(1, a);
    TEST_ASSERT_EQUAL(2, b);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_defer_basic);
    RUN_TEST(test_defer_reverse_order);
    RUN_TEST(test_defer_early_return);
    RUN_TEST(test_defer_nested_scopes);
    RUN_TEST(test_defer_resource_cleanup);
    RUN_TEST(test_defer_conditional);
    RUN_TEST(test_defer_variable_capture);
    RUN_TEST(test_defer_multiple_statements);
    RUN_TEST(test_defer_in_loop);
    RUN_TEST(test_defer_with_goto_before);
    RUN_TEST(test_defer_pointer_access);
    RUN_TEST(test_defer_same_variable_multiple_times);
    RUN_TEST(test_defer_array_manipulation);
    RUN_TEST(test_defer_function_call);
    RUN_TEST(test_defer_in_switch);
    RUN_TEST(test_defer_scope_not_entered);
    RUN_TEST(test_defer_complex_nesting);
    RUN_TEST(test_defer_string_operations);
    RUN_TEST(test_defer_global_variable);
    RUN_TEST(test_defer_semicolon_separator);

    return UNITY_END();
}
