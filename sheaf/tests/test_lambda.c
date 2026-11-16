#include "../src/lambda.h"
#include "../src/types.h"
#include <stdbool.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

void test_lambda_basic_integer_return(void) {
    i32 (*add)(i32, i32) = lambda(
        i32, (i32 a, i32 b) { return a + b; });

    i32 result = add(5, 3);
    TEST_ASSERT_EQUAL(8, result);
}

void test_lambda_basic_void_return(void) {
    i32 counter = 0;
    void (*increment)(void) = lambda(
        void, (void) { counter++; });

    increment();
    TEST_ASSERT_EQUAL(1, counter);

    increment();
    TEST_ASSERT_EQUAL(2, counter);
}

void test_lambda_multiple_parameters(void) {
    i32 (*multiply_three)(i32, i32, i32) = lambda(
        i32, (i32 a, i32 b, i32 c) { return a * b * c; });

    i32 result = multiply_three(2, 3, 4);
    TEST_ASSERT_EQUAL(24, result);
}

void test_lambda_single_parameter(void) {
    i32 (*square)(i32) = lambda(
        i32, (i32 x) { return x * x; });

    TEST_ASSERT_EQUAL(25, square(5));
    TEST_ASSERT_EQUAL(16, square(4));
    TEST_ASSERT_EQUAL(9, square(-3));
}

void test_lambda_no_parameters(void) {
    i32 (*get_constant)(void) = lambda(
        i32, (void) { return 42; });

    TEST_ASSERT_EQUAL(42, get_constant());
}

void test_lambda_floating_point(void) {
    f32 (*divide)(f32, f32) = lambda(
        f32, (f32 a, f32 b) { return a / b; });

    f32 result = divide(10.0f, 3.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.333333f, result);
}

void test_lambda_boolean_return(void) {
    bool (*is_even)(i32) = lambda(
        bool, (i32 n) { return n % 2 == 0; });

    TEST_ASSERT_TRUE(is_even(4));
    TEST_ASSERT_FALSE(is_even(5));
    TEST_ASSERT_TRUE(is_even(0));
    TEST_ASSERT_TRUE(is_even(-2));
}

void test_lambda_complex_logic(void) {
    i32 (*factorial)(i32) = lambda(
        i32, (i32 n) {
            if (n <= 1)
                return 1;
            i32 result = 1;
            for (i32 i = 2; i <= n; i++) {
                result *= i;
            }
            return result;
        });

    TEST_ASSERT_EQUAL(1, factorial(0));
    TEST_ASSERT_EQUAL(1, factorial(1));
    TEST_ASSERT_EQUAL(2, factorial(2));
    TEST_ASSERT_EQUAL(6, factorial(3));
    TEST_ASSERT_EQUAL(24, factorial(4));
    TEST_ASSERT_EQUAL(120, factorial(5));
}

void test_lambda_pointer_parameters(void) {
    void (*modify_array)(i32 *, i32) = lambda(
        void, (i32 * arr, i32 size) {
            for (i32 i = 0; i < size; i++) {
                arr[i] *= 2;
            }
        });

    i32 numbers[] = {1, 2, 3, 4, 5};
    modify_array(numbers, 5);

    TEST_ASSERT_EQUAL(2, numbers[0]);
    TEST_ASSERT_EQUAL(4, numbers[1]);
    TEST_ASSERT_EQUAL(6, numbers[2]);
    TEST_ASSERT_EQUAL(8, numbers[3]);
    TEST_ASSERT_EQUAL(10, numbers[4]);
}

void test_lambda_string_operations(void) {
    i32 (*string_length)(const char *) = lambda(
        i32, (const char *str) {
            i32 len = 0;
            while (str[len] != '\0') {
                len++;
            }
            return len;
        });

    TEST_ASSERT_EQUAL(5, string_length("hello"));
    TEST_ASSERT_EQUAL(0, string_length(""));
    TEST_ASSERT_EQUAL(13, string_length("hello, world!"));
}

void test_lambda_nested_calls(void) {
    i32 (*add)(i32, i32) = lambda(
        i32, (i32 a, i32 b) { return a + b; });
    i32 (*multiply)(i32, i32) = lambda(
        i32, (i32 a, i32 b) { return a * b; });

    i32 result = multiply(add(2, 3), add(4, 6));
    TEST_ASSERT_EQUAL(50, result); // (2+3) * (4+6) = 5 * 10 = 50
}

void test_lambda_variable_capture(void) {
    i32 external_var = 10;
    i32 (*use_external)(i32) = lambda(
        i32, (i32 x) { return x + external_var; });

    TEST_ASSERT_EQUAL(15, use_external(5));
    TEST_ASSERT_EQUAL(22, use_external(12));
}

void test_lambda_multiple_lambdas(void) {
    i32 (*add_one)(i32) = lambda(
        i32, (i32 x) { return x + 1; });
    i32 (*add_two)(i32) = lambda(
        i32, (i32 x) { return x + 2; });
    i32 (*add_three)(i32) = lambda(
        i32, (i32 x) { return x + 3; });

    TEST_ASSERT_EQUAL(6, add_one(5));
    TEST_ASSERT_EQUAL(7, add_two(5));
    TEST_ASSERT_EQUAL(8, add_three(5));
}

void test_lambda_conditional_return(void) {
    i32 (*max)(i32, i32) = lambda(
        i32, (i32 a, i32 b) {
            if (a > b) {
                return a;
            } else {
                return b;
            }
        });

    TEST_ASSERT_EQUAL(10, max(5, 10));
    TEST_ASSERT_EQUAL(15, max(15, 8));
    TEST_ASSERT_EQUAL(7, max(7, 7));
}

void test_lambda_with_structs(void) {
    typedef struct {
        i32 x;
        i32 y;
    } Point;

    i32 (*distance_squared)(Point, Point) = lambda(
        i32, (Point p1, Point p2) {
            i32 dx = p1.x - p2.x;
            i32 dy = p1.y - p2.y;
            return dx * dx + dy * dy;
        });

    Point p1 = {0, 0};
    Point p2 = {3, 4};

    TEST_ASSERT_EQUAL(25, distance_squared(p1, p2)); // 3^2 + 4^2 = 9 + 16 = 25
}

i32 main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_lambda_basic_integer_return);
    RUN_TEST(test_lambda_basic_void_return);
    RUN_TEST(test_lambda_multiple_parameters);
    RUN_TEST(test_lambda_single_parameter);
    RUN_TEST(test_lambda_no_parameters);
    RUN_TEST(test_lambda_floating_point);
    RUN_TEST(test_lambda_boolean_return);
    RUN_TEST(test_lambda_complex_logic);
    RUN_TEST(test_lambda_pointer_parameters);
    RUN_TEST(test_lambda_string_operations);
    RUN_TEST(test_lambda_nested_calls);
    RUN_TEST(test_lambda_variable_capture);
    RUN_TEST(test_lambda_multiple_lambdas);
    RUN_TEST(test_lambda_conditional_return);
    RUN_TEST(test_lambda_with_structs);

    return UNITY_END();
}