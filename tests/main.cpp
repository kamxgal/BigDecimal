/*
 * Author: Kamil Galant
 * e-mail: kamil.galant@gmail.com
 * github: https://github.com/kamxgal
 *
 * Copyright (C) 2023 Kamil Galant. All Rights Reserved.
 *
 * License
 * StrictDecimal is released under BSD-3-Clause license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  (1) Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in
 *  the documentation and/or other materials provided with the
 *  distribution.
 *
 *  (3)The name of the author may not be used to
 *  endorse or promote products derived from this software without
 *  specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 *  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 *  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */
#include "test_macros.hpp"
#include "decimal_tests.hpp"
#include "ranged_decimal_tests.hpp"

void RunDecimalTests(TestSuite& testSuite)
{
    EXECUTE_TEST(testSuite, test_constructing_decimal_from_double_and_parsing_to_string);
    EXECUTE_TEST(testSuite, test_constructing_decimal_from_double_with_fraction_part_around_half);
    EXECUTE_TEST(testSuite, test_constructing_decimal_from_float_and_parsing_to_float);
    EXECUTE_TEST(testSuite, test_constructing_decimal_from_double_and_parsing_to_double);
    EXECUTE_TEST(testSuite, test_constructing_decimal_from_integer_and_parsing);
    EXECUTE_TEST(testSuite, test_constructing_decimal_from_direct_constructor_and_parsing);
    EXECUTE_TEST(testSuite, test_constructing_decimal_from_string_and_parsing);

    EXECUTE_TEST(testSuite, test_adding_two_decimals);
    EXECUTE_TEST(testSuite, test_subtracting_two_decimals);
    EXECUTE_TEST(testSuite, test_multiplying_two_decimals);
    EXECUTE_TEST(testSuite, test_dividing_two_decimals);

    EXECUTE_TEST(testSuite, test_adding_decimal);
    EXECUTE_TEST(testSuite, test_subtracting_decimal);
    EXECUTE_TEST(testSuite, test_multiplying_by_decimal);
    EXECUTE_TEST(testSuite, test_dividing_by_decimal);

    EXECUTE_TEST(testSuite, test_getting_integer_and_fraction_part);

    EXECUTE_TEST(testSuite, test_multiplying_by_decimal_with_different_precision);
    EXECUTE_TEST(testSuite, test_dividing_decimals_with_different_precision);
    EXECUTE_TEST(testSuite, test_dividing_by_decimal_with_different_precision);

    EXECUTE_TEST(testSuite, test_creation_of_negative_numbers);
    EXECUTE_TEST(testSuite, test_adding_negative_decimals);
    EXECUTE_TEST(testSuite, test_subtracking_negative_decimals);
    EXECUTE_TEST(testSuite, test_multiplying_negative_decimals);

    EXECUTE_TEST(testSuite, test_dividing_negative_decimals);
    EXECUTE_TEST(testSuite, test_multiplying_by_negative_decimals);
    EXECUTE_TEST(testSuite, test_dividing_by_negative_decimals);

    EXECUTE_TEST(testSuite, test_multiplying_by_negative_decimals_with_different_precision);
    EXECUTE_TEST(testSuite, test_dividing_by_negative_decimals_with_different_precision);

    EXECUTE_TEST(testSuite, test_casting_precision_down_positive_decimals);
    EXECUTE_TEST(testSuite, test_casting_precision_up_positive_decimals);

    EXECUTE_TEST(testSuite, test_casting_precision_down_negative_decimals);
    EXECUTE_TEST(testSuite, test_casting_precision_up_negative_decimals);

    EXECUTE_TEST(testSuite, test_casting_to_different_underlying_type);
    EXECUTE_TEST(testSuite, test_casting_to_different_precision);

    EXECUTE_TEST(testSuite, test_comparing_decimals);

    EXECUTE_TEST(testSuite, test_mutiplying_small_precision_by_big_precision_decimal);

    EXECUTE_TEST(testSuite, test_handling_divide_positive_infinity);
    EXECUTE_TEST(testSuite, test_handling_divide_negative_infinity);
    EXECUTE_TEST(testSuite, test_handling_divide_nan);

    EXECUTE_TEST(testSuite, test_multiplying_floats_by_strict_integers);
    EXECUTE_TEST(testSuite, test_multiplying_strict_integers);
    EXECUTE_TEST(testSuite, test_multiplying_integer_by_float);
    EXECUTE_TEST(testSuite, test_direct_multiplying_integer_by_float);

    EXECUTE_TEST(testSuite, test_divide_floats_by_strict_integers);
    EXECUTE_TEST(testSuite, test_divide_strict_integers);
    EXECUTE_TEST(testSuite, test_divide_integer_by_float);
    EXECUTE_TEST(testSuite, test_direct_divide_integer_by_float);

    EXECUTE_TEST(testSuite, test_comparison_operators);
}

void RunRangedDecimalTests(TestSuite& testSuite)
{
    EXECUTE_TEST(testSuite, test_constructing_ranged_decimal_from_double_and_parsing_to_string);
    EXECUTE_TEST(testSuite, test_constructing_ranged_decimal_from_double_with_fraction_part_around_half);
    EXECUTE_TEST(testSuite, test_constructing_ranged_decimal_from_float_and_parsing_to_float);
    EXECUTE_TEST(testSuite, test_constructing_ranged_decimal_from_double_and_parsing_to_double);
    EXECUTE_TEST(testSuite, test_constructing_ranged_decimal_from_integer_and_parsing);
    EXECUTE_TEST(testSuite, test_constructing_ranged_decimal_from_direct_constructor_and_parsing);
    EXECUTE_TEST(testSuite, test_constructing_ranged_decimal_from_string_and_parsing);

    EXECUTE_TEST(testSuite, test_adding_two_ranged_decimals);
    EXECUTE_TEST(testSuite, test_subtracting_two_ranged_decimals);
    EXECUTE_TEST(testSuite, test_multiplying_two_ranged_decimals);
    EXECUTE_TEST(testSuite, test_dividing_two_ranged_decimals);

    EXECUTE_TEST(testSuite, test_adding_ranged_decimal);
    EXECUTE_TEST(testSuite, test_subtracting_ranged_decimal);
    EXECUTE_TEST(testSuite, test_multiplying_by_ranged_decimal);
    EXECUTE_TEST(testSuite, test_dividing_by_ranged_decimal);

    EXECUTE_TEST(testSuite, test_getting_integer_and_fraction_part_from_ranged_decimal);

    EXECUTE_TEST(testSuite, test_multiplying_by_ranged_decimal_with_different_precision);
    EXECUTE_TEST(testSuite, test_dividing_by_ranged_decimal_with_different_precision);

    EXECUTE_TEST(testSuite, test_creation_of_negative_ranged_decimal_numbers);
    EXECUTE_TEST(testSuite, test_adding_negative_ranged_decimals);
    EXECUTE_TEST(testSuite, test_subtracking_negative_ranged_decimals);
    EXECUTE_TEST(testSuite, test_multiplying_negative_ranged_decimals);

    EXECUTE_TEST(testSuite, test_dividing_negative_ranged_decimals);
    EXECUTE_TEST(testSuite, test_multiplying_by_negative_ranged_decimals);

    EXECUTE_TEST(testSuite, test_dividing_by_negative_ranged_decimals);
    EXECUTE_TEST(testSuite, test_multiplying_by_negative_ranged_decimals_with_different_precision);

    EXECUTE_TEST(testSuite, test_dividing_by_negative_ranged_decimals_with_different_precision);
    EXECUTE_TEST(testSuite, test_casting_precision_down_positive_ranged_decimals);

    EXECUTE_TEST(testSuite, test_casting_precision_up_positive_ranged_decimals);
    EXECUTE_TEST(testSuite, test_casting_precision_down_negative_ranged_decimals);

    EXECUTE_TEST(testSuite, test_casting_precision_up_negative_ranged_decimals);
    EXECUTE_TEST(testSuite, test_casting_ranged_decimal_to_different_underlying_type);
    EXECUTE_TEST(testSuite, test_casting_ranged_decimal_to_different_precision);

    EXECUTE_TEST(testSuite, test_comparing_ranged_decimals);

    EXECUTE_TEST(testSuite, test_narrowing_above_max_values);
    EXECUTE_TEST(testSuite, test_narrowing_below_min_values);

    EXECUTE_TEST(testSuite, test_multiplying_ranged_decimal_by_decimal);

    EXECUTE_TEST(testSuite, test_handling_divide_ranged_decimal_to_positive_infinity);
    EXECUTE_TEST(testSuite, test_handling_divide_ranged_decimal_to_negative_infinity);
    EXECUTE_TEST(testSuite, test_handling_divide_ranged_decimal_to_nan);
}

int main()
{
    TestSuite decimalTests("DecimalTests");
    TestSuite rangedDecimalTests("RangedDecimalTests");

    RunDecimalTests(decimalTests);
    RunRangedDecimalTests(rangedDecimalTests);

    decimalTests.print_failed();
    rangedDecimalTests.print_failed();
    return 0;
}
