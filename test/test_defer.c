#include "unity.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void) {}

void tearDown(void) {}

void test_basic_assertion(void) { TEST_ASSERT_TRUE(1); }

void test_equality(void) { TEST_ASSERT_EQUAL(42, 42); }

void test_string_equality(void) { TEST_ASSERT_EQUAL_STRING("hello", "hello"); }

void test_null_assertion(void) {
    void *ptr = NULL;
    TEST_ASSERT_NULL(ptr);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_basic_assertion);
    RUN_TEST(test_equality);
    RUN_TEST(test_string_equality);
    RUN_TEST(test_null_assertion);

    return UNITY_END();
}
