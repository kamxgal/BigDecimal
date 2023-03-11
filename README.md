# BigDecimal
Header-only Big Decimal implementation for C++.

Example:

```
#include <BigDecimal/decimal.hpp>
```

Numbers below are rounded to 3 decimal digits and each of them are equal:
```
const auto number0 = big_decimal::decimal_t<int64_t, 3>{"10.2346"};  // "10.235" after rounding to 3 digits
const auto number1 = big_decimal::decimal_t<int64_t, 3>{10.2346};
const auto number2 = big_decimal::decimal_t<int64_t, 3>{10.2346f};
const auto number3 = big_decimal::decimal_t<int64_t, 3>{10, 2346};

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
const auto number4d = big_decimal::decimal_t<int64_t, 4>{"10.2346"};
    
const auto casted5d = big_decimal::decimal_cast<int64_t, 5>(number4d);  // 5 digits fraction
const auto casted3d = big_decimal::decimal_cast<int64_t, 3>(number4d);  // 3 digits fraction
const auto casted1d = big_decimal::decimal_cast<int64_t, 1>(number4d);  // 1 digits fraction
const auto casted0d = big_decimal::decimal_cast<int64_t, 0>(number4d);  // 0 digits fraction
```

Decimals can be used as input to streams. In such case they are parsed to std::string.
```
std::cout << number4d << std::endl;  // "10.2346"
std::cout << casted5d << std::endl;  // "10.23460"
std::cout << casted3d << std::endl;  // "10.235"
std::cout << casted1d << std::endl;  // "10.2"
std::cout << casted0d << std::endl;  // "10.0"
```
