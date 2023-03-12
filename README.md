# StrictDecimal
Header-only decimal implementation for C++ which stores strict values and can be used for example for financial purposes.

Two types of data are available for the user:
1. `strict::decimal_t` - to store decimals in range which uses all capabilities of defined underlying data type
2. `strict::ranged_decimal_t` - to store decimals in range defined by user

When creating a decimal, user provides an underlying data type to store nominator of the decimal and its precision. Based on that a denominator is calculated in compile time. The nominator is stored as member variable of given decimal. Decimals are rounded in such way that if the last digit is less than 5, the previous digit is rounded down, otherwise it is rounded up.

When using `strict::ranged_decimal_t` if a value goes beyond defined range it is clamped.

Shortly, writing `auto number = strict::decimal_t<int64_t, 2>()` creates a decimal which stores nominator in `int64_t` with precision of two decimal digits which means that the denominator (accessed via strict::decimal_t<int64_t, 2>::DENOMINATOR) is equal to `10^2 = 100`.

Defining `auto number = strict::ranged_decimal_t<int64_t, 2, -1000, 1000>()` creates a number which stores nominator in `int64_t` with precision of 2 decimal digits and with certainty that it is in range between -1000 and 1000.

Example:

```
#include <StrictDecimal/decimal.hpp>
```

Numbers below are rounded to 3 decimal digits and they are all equal:
```
const auto number0 = strict::decimal_t<int64_t, 3>{"10.2346"};  // "10.235" after rounding to 3 digits
const auto number1 = strict::decimal_t<int64_t, 3>{10.2346};
const auto number2 = strict::decimal_t<int64_t, 3>{10.2346f};
const auto number3 = strict::decimal_t<int64_t, 3>{10, 2346};

std::cout << std::boolalpha;
std::cout << (number0 == number1) << std::endl;  // true
std::cout << (number0 == number2) << std::endl;  // true
std::cout << (number0 == number3) << std::endl;  // true
std::cout << (number0 != number3) << std::endl;  // false
```
   
Decimals can be parsed to strings, floats and doubles
```
const auto str1 = number0.to_string(); // "10.235"
const auto float1 = number0.to_float(); // 10.235f
const auto double1 = number0.to_double(); // 10.235

std::cout << str1 << std::endl;  // "10.2346"
std::cout << float1 << std::endl;  // 10.235f
std::cout << double1 << std::endl;  // 10.235
```

It is possible to cast one decimal to another using decimal_cast:
```
const auto number4d = strict::decimal_t<int64_t, 4>{"10.2346"};
    
const auto casted5d = strict::decimal_cast<int64_t, 5>(number4d);  // 5 digits fraction
const auto casted3d = strict::decimal_cast<int64_t, 3>(number4d);  // 3 digits fraction
const auto casted1d = strict::decimal_cast<int64_t, 1>(number4d);  // 1 digits fraction
const auto casted0d = strict::decimal_cast<int64_t, 0>(number4d);  // 0 digits fraction
```

Decimals can be used as input to streams. In such case they are parsed to std::string.
```
std::cout << number4d << std::endl;  // "10.2346"
std::cout << casted5d << std::endl;  // "10.23460"
std::cout << casted3d << std::endl;  // "10.235"
std::cout << casted1d << std::endl;  // "10.2"
std::cout << casted0d << std::endl;  // "10.0"
```
