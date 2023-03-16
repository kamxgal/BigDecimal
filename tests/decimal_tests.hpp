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
#pragma once


bool test_constructing_decimal_from_double_and_parsing_to_string();
bool test_constructing_decimal_from_double_with_fraction_part_around_half();
bool test_constructing_decimal_from_float_and_parsing_to_float();
bool test_constructing_decimal_from_double_and_parsing_to_double();
bool test_constructing_decimal_from_integer_and_parsing();
bool test_constructing_decimal_from_direct_constructor_and_parsing();
bool test_constructing_decimal_from_string_and_parsing();

bool test_adding_two_decimals();
bool test_subtracting_two_decimals();
bool test_multiplying_two_decimals();
bool test_dividing_two_decimals();

bool test_adding_decimal();
bool test_subtracting_decimal();
bool test_multiplying_by_decimal();
bool test_dividing_by_decimal();

bool test_getting_integer_and_fraction_part();

bool test_multiplying_by_decimal_with_different_precision();
bool test_dividing_decimals_with_different_precision();
bool test_dividing_by_decimal_with_different_precision();

bool test_creation_of_negative_numbers();
bool test_adding_negative_decimals();
bool test_subtracking_negative_decimals();
bool test_multiplying_negative_decimals();

bool test_dividing_negative_decimals();
bool test_multiplying_by_negative_decimals();
bool test_dividing_by_negative_decimals();

bool test_multiplying_by_negative_decimals_with_different_precision();
bool test_dividing_by_negative_decimals_with_different_precision();
bool test_casting_precision_down_positive_decimals();
bool test_casting_precision_up_positive_decimals();
bool test_casting_precision_down_negative_decimals();
bool test_casting_precision_up_negative_decimals();
bool test_casting_to_different_underlying_type();
bool test_casting_to_different_precision();
bool test_comparing_decimals();

bool test_mutiplying_small_precision_by_big_precision_decimal();

bool test_handling_divide_positive_infinity();
bool test_handling_divide_negative_infinity();
bool test_handling_divide_nan();

bool test_multiplying_floats_by_strict_integers();
bool test_multiplying_strict_integers();
bool test_multiplying_integer_by_float();
bool test_direct_multiplying_integer_by_float();

bool test_divide_floats_by_strict_integers();
bool test_divide_strict_integers();
bool test_divide_integer_by_float();
bool test_direct_divide_integer_by_float();

bool test_comparison_operators();
