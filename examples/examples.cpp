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
#include "../decimal.hpp"
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[])
{
    // Numbers below are rounded to 3 decimal digits and each of them are equal:
    const auto number0 = strict::decimal_t<int64_t, 3>{"10.2346"};  // "10.235" after rounding to 3 digits
    const auto number1 = strict::decimal_t<int64_t, 3>{10.2346};
    const auto number2 = strict::decimal_t<int64_t, 3>{10.2346f};
    const auto number3 = strict::decimal_t<int64_t, 3>{10, 2346};

    std::cout << std::boolalpha;
    std::cout << (number0 == number1) << std::endl;  // true
    std::cout << (number0 == number2) << std::endl;  // true
    std::cout << (number0 == number3) << std::endl;  // true
    std::cout << (number0 != number3) << std::endl;  // false
    std::cout << std::endl;

    const auto str1 = number0.to_string(); // "10.235"
    const auto float1 = number0.to_float(); // 10.235f
    const auto double1 = number0.to_double(); // 10.235

    std::cout << str1 << std::endl;  // "10.2346"
    std::cout << float1 << std::endl;  // 10.235f
    std::cout << double1 << std::endl;  // 10.235
    std::cout << std::endl;

    // It is possible to case one decimal to another using decimal_cast:
    const auto number4d = strict::decimal_t<int64_t, 4>{"10.2346"};

    const auto casted5d = strict::decimal_cast<int64_t, 5>(number4d);  // 5 digits fraction
    const auto casted3d = strict::decimal_cast<int64_t, 3>(number4d);  // 3 digits fraction
    const auto casted1d = strict::decimal_cast<int64_t, 1>(number4d);  // 1 digits fraction
    const auto casted0d = strict::decimal_cast<int64_t, 0>(number4d);  // 0 digits fraction

    // Streaming decimals to output (automatically parsed to std::string):
    std::cout << number4d << std::endl;  // "10.2346"
    std::cout << casted5d << std::endl;  // "10.23460"
    std::cout << casted3d << std::endl;  // "10.235"
    std::cout << casted1d << std::endl;  // "10.2"
    std::cout << casted0d << std::endl;  // "10.0"
}
