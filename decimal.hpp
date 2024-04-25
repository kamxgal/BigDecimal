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

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <numeric>
#include <string>
#include <sstream>


namespace strict
{

template<typename RetT>
constexpr RetT Power10(int n) noexcept {
    const RetT lookup[] = {
        static_cast<RetT>(1), // 10^0
        static_cast<RetT>(10), // 10^1
        static_cast<RetT>(100), // 10^2
        static_cast<RetT>(1000), // 10^3
        static_cast<RetT>(10000), // 10^4
        static_cast<RetT>(100000), // 10^5
        static_cast<RetT>(1000000), // 10^6
        static_cast<RetT>(10000000), // 10^7
        static_cast<RetT>(100000000), // 10^8
        static_cast<RetT>(1000000000), // 10^9
        static_cast<RetT>(10000000000), // 10^10
    };
    RetT res = lookup[n % 10];
    const int loops = n / 10;
    for (int i=0; i<loops; ++i) {
        res *= lookup[10];
    }
    return res;
}

template<typename UnderlyingType = int64_t, int Precision = 2>
struct decimal_t
{
    using underlying_type = UnderlyingType;
    static constexpr int PRECISION = std::max(Precision, std::numeric_limits<UnderlyingType>::max_digits10-1);
    static constexpr UnderlyingType DENOMINATOR = Power10<UnderlyingType>(PRECISION);
    static constexpr UnderlyingType HALF_DENOMINATOR = DENOMINATOR / 2;
    static constexpr UnderlyingType NAN_VALUE = std::numeric_limits<UnderlyingType>::max();
    static constexpr UnderlyingType INFINITY_PLUS = std::numeric_limits<UnderlyingType>::max() - 1;
    static constexpr UnderlyingType INFINITY_MINUS = std::numeric_limits<UnderlyingType>::min() + 1;

    struct nominator_t { UnderlyingType value{}; };

    explicit decimal_t() : mNominator{} {
    }

    explicit decimal_t(std::string num) {
        UnderlyingType integerPart{}, fractionPart{};
        char dotPlaceholder;
        std::istringstream ss(num);
        ss >> integerPart >> dotPlaceholder >> fractionPart;
        const ::size_t dotPos = num.find('.');

        if (dotPos == std::string::npos) {
            fractionPart = 0;
        } else {
            int fractionPartLen = static_cast<int>(num.size() - dotPos - 1);
            fractionPart *= fractionPartLen >= PRECISION ? UnderlyingType{1} : Power10<UnderlyingType>(PRECISION - fractionPartLen);
        }

        mNominator = decimal_t{integerPart, fractionPart}.mNominator;
    }

    template<typename FloatingT,
             std::enable_if_t<std::is_floating_point<FloatingT>::value, bool> = true>
    explicit decimal_t(FloatingT num) {
        int lastDigit = static_cast<underlying_type>(std::abs(num) * DENOMINATOR * 10) % 10;
        underlying_type signFactor = num >=0 ? 1 : -1;
        mNominator.value = static_cast<UnderlyingType>(num * DENOMINATOR) + (lastDigit >= 5 ? signFactor : 0);
    }

    template<typename IntegralT,
             std::enable_if_t<std::is_integral<IntegralT>::value, bool> = true>
    explicit decimal_t(IntegralT num) {
        mNominator.value = static_cast<UnderlyingType>(num * DENOMINATOR);
    }

    explicit decimal_t(typename decimal_t<UnderlyingType, PRECISION>::nominator_t nominator) : mNominator(nominator) {
    }

    explicit decimal_t(UnderlyingType integerPart, UnderlyingType fractionPart) {
        int fractionLength = static_cast<int>(std::log10(fractionPart)) + 1;
        if (fractionLength > PRECISION) {
            int lastSignificantDigit = fractionPart / Power10<UnderlyingType>(fractionLength - PRECISION -1) % 10;
            fractionPart = fractionPart / Power10<UnderlyingType>(fractionLength - PRECISION) + (lastSignificantDigit >= 5 ? 1 : 0);
        }
        mNominator.value = integerPart * DENOMINATOR + (integerPart >= 0 ? fractionPart : -fractionPart);
    }

    UnderlyingType nominator() const { return mNominator.value; }
    UnderlyingType integer_part() const { return mNominator.value / DENOMINATOR; }
    UnderlyingType fraction_part() const { return std::abs(mNominator.value) % DENOMINATOR; }

    constexpr bool operator<(const decimal_t<underlying_type, PRECISION>& other) const { return mNominator.value < other.mNominator.value; }
    constexpr bool operator<=(const decimal_t<underlying_type, PRECISION>& other) const { return mNominator.value <= other.mNominator.value; }
    constexpr bool operator==(const decimal_t<underlying_type, PRECISION>& other) const { return mNominator.value == other.mNominator.value; }
    constexpr bool operator!=(const decimal_t<underlying_type, PRECISION>& other) const { return mNominator.value != other.mNominator.value; }
    constexpr bool operator>=(const decimal_t<underlying_type, PRECISION>& other) const { return mNominator.value >= other.mNominator.value; }
    constexpr bool operator>(const decimal_t<underlying_type, PRECISION>& other) const { return mNominator.value > other.mNominator.value; }

    decimal_t<underlying_type, PRECISION> operator+(const decimal_t<underlying_type, PRECISION>& rhs) const {
        return decimal_t<UnderlyingType, PRECISION>{nominator_t{mNominator.value + rhs.mNominator.value}};
    }
    decimal_t<underlying_type, PRECISION> operator-(const decimal_t<underlying_type, PRECISION>& rhs) const {
        return decimal_t<UnderlyingType, PRECISION>{nominator_t{mNominator.value - rhs.mNominator.value}};
    }
    decimal_t<underlying_type, PRECISION> operator-() const {
        return decimal_t<UnderlyingType, PRECISION>{nominator_t{-mNominator.value}};
    }
    decimal_t<underlying_type, PRECISION> operator*(const decimal_t<underlying_type, PRECISION>& rhs) const {
        using this_type = decimal_t<underlying_type, PRECISION>;
        underlying_type res = mNominator.value * rhs.mNominator.value;
        if constexpr (PRECISION == 0) {
            return this_type{nominator_t{res}};
        } else {
            underlying_type fractionPart = std::abs(res) % DENOMINATOR;
            underlying_type signFactor = res >= 0 ? 1 : -1;
            return this_type{nominator_t{res / DENOMINATOR + (fractionPart >= HALF_DENOMINATOR ? signFactor : 0)}};
        }
    }
    template<typename RhsUnderlyingType, int RhsPrecision>
    decimal_t<underlying_type, PRECISION> operator*(const decimal_t<RhsUnderlyingType, RhsPrecision>& rhs) const {
        decimal_t<underlying_type, PRECISION> res = *this;
        res *= rhs;
        return res;
    }
    template<typename RhsUnderlyingType, int RhsPrecision>
    decimal_t<underlying_type, PRECISION> operator/(const decimal_t<RhsUnderlyingType, RhsPrecision>& rhs) const {
        decimal_t<underlying_type, PRECISION> res = *this;
        res /= rhs;
        return res;
    }
    decimal_t<underlying_type, PRECISION>& operator+=(const decimal_t<underlying_type, PRECISION>& rhs) {
        mNominator.value += rhs.mNominator.value;
        return *this;
    }
    decimal_t<underlying_type, PRECISION>& operator-=(const decimal_t<underlying_type, PRECISION>& rhs) {
        mNominator.value -= rhs.mNominator.value;
        return *this;
    }
    decimal_t<underlying_type, PRECISION>& operator*=(const decimal_t<underlying_type, PRECISION>& rhs) {
        underlying_type res = mNominator.value * rhs.mNominator.value;
        if constexpr (PRECISION == 0) {
            mNominator.value = mNominator.value * rhs.mNominator.value;
        } else {
            underlying_type fractionPart = std::abs(res) % DENOMINATOR;
            underlying_type signFactor = res >= 0 ? 1 : -1;
            mNominator.value = mNominator.value * rhs.mNominator.value / DENOMINATOR + (fractionPart >= HALF_DENOMINATOR ? signFactor : 0);
        }
        return *this;
    }

    template<typename RhsUnderlyingType, int RhsDecimalPrecision>
    decimal_t<underlying_type, PRECISION>& operator*=(const decimal_t<RhsUnderlyingType, RhsDecimalPrecision>& rhs) {
        using OperatingType = std::conditional_t<sizeof(underlying_type) >= sizeof(RhsUnderlyingType), underlying_type, RhsUnderlyingType>;
        const OperatingType integerPart = static_cast<OperatingType>(this->integer_part());
        OperatingType fractionPart = static_cast<OperatingType>(this->fraction_part());
        const OperatingType rhsIntegerPart = static_cast<OperatingType>(rhs.integer_part());
        OperatingType rhsFractionPart = static_cast<OperatingType>(rhs.fraction_part());

        constexpr int CommonPrecision = PRECISION < RhsDecimalPrecision ? RhsDecimalPrecision : PRECISION;

        // setting values of fractionPart and rhsFractionPart as having common denominator
        fractionPart *= Power10<OperatingType>(CommonPrecision) / DENOMINATOR;
        rhsFractionPart *= Power10<OperatingType>(CommonPrecision) / static_cast<underlying_type>(std::decay_t<decltype(rhs)>::DENOMINATOR);

        OperatingType resultInteger = std::abs(integerPart * rhsIntegerPart);
        underlying_type signFactor = integerPart * rhsIntegerPart >= 0 ? 1 : -1;
        // commonFraction has denominator equal to commonDenominator
        OperatingType commonFraction = std::abs(integerPart) * rhsFractionPart + fractionPart * std::abs(rhsIntegerPart);
        resultInteger += commonFraction / Power10<OperatingType>(CommonPrecision);
        commonFraction %= Power10<OperatingType>(CommonPrecision);
        // from now on denominator of commonFraction equals commonDenominator*commonDenominator
        commonFraction *= Power10<OperatingType>(CommonPrecision);
        commonFraction += fractionPart * rhsFractionPart;
        // from now on commonFraction is treated as if its denominator is equal Power10<underlying_type>(PRECISION + 1);
        commonFraction /= Power10<OperatingType>(CommonPrecision + CommonPrecision - PRECISION - 1);
        const int lastSignificantDigit = commonFraction % 10;
        commonFraction /= 10;
        commonFraction += lastSignificantDigit >= 5 ? 1 : 0;

        mNominator.value = static_cast<underlying_type>(resultInteger * DENOMINATOR + commonFraction);
        mNominator.value *= signFactor;
        return *this;
    }

    template <typename RhsUnderlyingType, int RhsPrecision, std::enable_if_t<PRECISION != RhsPrecision, bool> = true>
    decimal_t<underlying_type, PRECISION>& operator/=(const decimal_t<RhsUnderlyingType, RhsPrecision>& rhs)
    {
        if (mNominator.value == 0 && rhs.nominator() == 0) {
            mNominator.value = NAN_VALUE;
            return *this;
        }
        if (rhs.nominator() == 0) {
            mNominator.value = mNominator.value >= 0 ? INFINITY_PLUS : INFINITY_MINUS;
            return *this;
        }

        using OperationType = std::conditional_t<sizeof(underlying_type) >= sizeof(RhsUnderlyingType), underlying_type, RhsUnderlyingType>;
        OperationType res = static_cast<OperationType>(mNominator.value);
        res *= Power10<OperationType>(RhsPrecision+1);
        res /= static_cast<OperationType>(rhs.nominator());
        OperationType signFactor = res >= 0 ? 1 : -1;
        int lastSignificantDigit = std::abs(res) % 10;
        res /= 10;
        res += lastSignificantDigit >= 5 ? signFactor : 0;
        mNominator.value = static_cast<UnderlyingType>(res);
        return *this;
    }

    template <typename RhsUnderlyingType>
    decimal_t<underlying_type, PRECISION>& operator/=(const decimal_t<RhsUnderlyingType, PRECISION>& rhs)
    {
        if (mNominator.value == 0 && rhs.nominator() == 0) {
            mNominator.value = NAN_VALUE;
            return *this;
        }
        if (rhs.nominator() == 0) {
            mNominator.value = mNominator.value >= 0 ? INFINITY_PLUS : INFINITY_MINUS;
            return *this;
        }

        using OperationType = std::conditional_t<sizeof(underlying_type) >= sizeof(RhsUnderlyingType), underlying_type, RhsUnderlyingType>;
        OperationType integerPart = std::abs(integer_part()) * Power10<OperationType>(PRECISION + 1);
        OperationType fractionPart = fraction_part() * Power10<OperationType>(1);

        OperationType signFactor = std::clamp<OperationType>(nominator(), -1, 1) * std::clamp<OperationType>(rhs.nominator(), -1, 1);

        integerPart = static_cast<int>(1.0 * integerPart / std::fabs(rhs.to_double()));
        fractionPart = static_cast<int>(1.0 * fractionPart / std::fabs(rhs.to_double()));

        OperationType res = integerPart + fractionPart;
        int lastSignificantDigit = std::abs(res) % 10;
        res /= 10;
        res *= signFactor;
        res += lastSignificantDigit >= 5 ? signFactor : 0;
        mNominator.value = static_cast<UnderlyingType>(res);
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const decimal_t<underlying_type, PRECISION>& decimal) {
        out << decimal.to_string();
        return out;
    }

    float to_float() const {
        switch (mNominator.value)
        {
        case NAN_VALUE: return std::nanf("");
        case INFINITY_PLUS: return std::numeric_limits<float>::infinity();
        case INFINITY_MINUS: return -std::numeric_limits<float>::infinity();
        default: return static_cast<float>(mNominator.value) / DENOMINATOR;
        }
    }
    double to_double() const {
        switch (mNominator.value)
        {
        case NAN_VALUE: return std::nan("");
        case INFINITY_PLUS: return std::numeric_limits<double>::infinity();
        case INFINITY_MINUS: return -std::numeric_limits<double>::infinity();
        default: return static_cast<double>(mNominator.value) / DENOMINATOR;
        }
    }

    std::string to_string() const {
        if constexpr (PRECISION == 0) {
            return std::to_string(mNominator.value);
        } else {
            switch (mNominator.value)
            {
            case NAN_VALUE: return std::string("nan");
            case INFINITY_PLUS: return std::string("inf");
            case INFINITY_MINUS: return std::string("-inf");
            default:
                std::stringstream ss;
                ss << integer_part() << '.' << std::setfill('0') << std::setw(PRECISION) << fraction_part();
                return ss.str();
            }
        }
    }

protected:
    nominator_t mNominator{};
};

template<typename UnderlyingType, int Precision, UnderlyingType MinValue, UnderlyingType MaxValue>
struct ranged_decimal_t : public decimal_t<UnderlyingType, Precision>
{
    using base_type = decimal_t<UnderlyingType, Precision>;
    using this_type = ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>;
    static constexpr int PRECISION = decimal_t<UnderlyingType, Precision>::PRECISION;
    static constexpr UnderlyingType MIN_VALUE = std::clamp<UnderlyingType>(MinValue,
        std::numeric_limits<UnderlyingType>::min() / base_type::DENOMINATOR,
        std::numeric_limits<UnderlyingType>::max() / base_type::DENOMINATOR);
    static constexpr UnderlyingType MAX_VALUE = std::clamp<UnderlyingType>(MaxValue,
        std::numeric_limits<UnderlyingType>::min() / base_type::DENOMINATOR,
        std::numeric_limits<UnderlyingType>::max() / base_type::DENOMINATOR);
    static constexpr UnderlyingType NOMINATOR_MIN_VALUE = MIN_VALUE * base_type::DENOMINATOR;
    static constexpr UnderlyingType NOMINATOR_MAX_VALUE = MAX_VALUE * base_type::DENOMINATOR;

    explicit ranged_decimal_t() : base_type() {
        this->mNominator.value = std::clamp(this->mNominator.value, NOMINATOR_MIN_VALUE, NOMINATOR_MAX_VALUE);
    }

    explicit ranged_decimal_t(std::string num) : base_type(num) {
        if (this->mNominator.value != base_type::NAN_VALUE) {
            this->mNominator.value = std::clamp(this->mNominator.value, NOMINATOR_MIN_VALUE, NOMINATOR_MAX_VALUE);
        }
    }

    template<typename FloatingT,
             std::enable_if_t<std::is_floating_point<FloatingT>::value, bool> = true>
    explicit ranged_decimal_t(FloatingT num) : base_type(num) {
        if (this->mNominator.value != base_type::NAN_VALUE) {
            this->mNominator.value = std::clamp(this->mNominator.value, NOMINATOR_MIN_VALUE, NOMINATOR_MAX_VALUE);
        }
    }

    template<typename IntegralT,
             std::enable_if_t<std::is_integral<IntegralT>::value, bool> = true>
    explicit ranged_decimal_t(IntegralT num) : base_type(num) {
        if (this->mNominator.value != base_type::NAN_VALUE) {
            this->mNominator.value = std::clamp(this->mNominator.value, NOMINATOR_MIN_VALUE, NOMINATOR_MAX_VALUE);
        }
    }

    explicit ranged_decimal_t(typename decimal_t<UnderlyingType, Precision>::nominator_t nominator)
        : base_type(nominator) {
        if (this->mNominator.value != base_type::NAN_VALUE) {
            this->mNominator.value = std::clamp(this->mNominator.value, NOMINATOR_MIN_VALUE, NOMINATOR_MAX_VALUE);
        }
    }

    explicit ranged_decimal_t(UnderlyingType integerPart, UnderlyingType fractionPart)
        : base_type(integerPart, fractionPart) {
        this->mNominator.value = std::clamp(this->mNominator.value, NOMINATOR_MIN_VALUE, NOMINATOR_MAX_VALUE);
    }

    constexpr bool operator<(const base_type& other) const { return this->mNominator.value < other.nominator(); }
    constexpr bool operator<=(const base_type& other) const { return this->mNominator.value <= other.nominator(); }
    constexpr bool operator==(const base_type& other) const { return this->mNominator.value == other.nominator(); }
    constexpr bool operator==(const this_type& other) const { return this->mNominator.value == other.nominator(); }
    constexpr bool operator>=(const base_type& other) const { return this->mNominator.value >= other.nominator(); }
    constexpr bool operator>(const base_type& other) const { return this->mNominator.value > other.nominator(); }

    ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>
    operator+(const ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>& rhs) const {
        const auto res = static_cast<const base_type&>(*this) + static_cast<const base_type&>(rhs);
        return ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>(typename base_type::nominator_t{res.nominator()});
    }
    ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>
    operator-(const ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>& rhs) const {
        const auto res = static_cast<const base_type&>(*this) - static_cast<const base_type&>(rhs);
        return ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>(typename base_type::nominator_t{res.nominator()});
    }
    ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>
    operator-() const {
        return ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>(
                    typename base_type::nominator_t{-static_cast<const base_type&>(*this).nominator()});
    }
    ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>
    operator*(const ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>& rhs) const {
        const auto res = static_cast<const base_type&>(*this) * static_cast<const base_type&>(rhs);
        return ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>(typename base_type::nominator_t{res.nominator()});
    }
    template<typename RhsUnderlyingType, int RhsPrecision>
    ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>
    operator*(const decimal_t<RhsUnderlyingType, RhsPrecision>& rhs) const {
        base_type res = static_cast<const base_type&>(*this);
        res *= rhs;
        return ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>(typename base_type::nominator_t{res.nominator()});
    }
    template<typename RhsUnderlyingType, int RhsPrecision>
    ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>
    operator/(const decimal_t<RhsUnderlyingType, RhsPrecision>& rhs) const {
        base_type res = static_cast<const base_type&>(*this) / rhs;
        return ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>(typename base_type::nominator_t{res.nominator()});
    }
    ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>&
    operator+=(const ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>& rhs) {
        static_cast<base_type&>(*this) += static_cast<const base_type&>(rhs);
        this->mNominator.value = std::clamp(this->mNominator.value, NOMINATOR_MIN_VALUE, NOMINATOR_MAX_VALUE);
        return *this;
    }
    ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>&
    operator-=(const ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>& rhs) {
        static_cast<base_type&>(*this) -= static_cast<const base_type&>(rhs);
        this->mNominator.value = std::clamp(this->mNominator.value, NOMINATOR_MIN_VALUE, NOMINATOR_MAX_VALUE);
        return *this;
    }
    template<typename RhsUnderlyingType, int RhsDecimalPrecision>
    ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>&
    operator*=(const decimal_t<RhsUnderlyingType, RhsDecimalPrecision>& rhs) {
        static_cast<base_type&>(*this) *= rhs;
        this->mNominator.value = std::clamp(this->mNominator.value, NOMINATOR_MIN_VALUE, NOMINATOR_MAX_VALUE);
        return *this;
    }
    template<typename RhsUnderlyingType, int RhsDecimalPrecision>
    ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>&
    operator/=(const decimal_t<RhsUnderlyingType, RhsDecimalPrecision>& rhs) {
        static_cast<base_type&>(*this) /= rhs;
        if (this->mNominator.value != base_type::NAN_VALUE) {
            this->mNominator.value = std::clamp(this->mNominator.value, NOMINATOR_MIN_VALUE, NOMINATOR_MAX_VALUE);
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const ranged_decimal_t<UnderlyingType, Precision, MinValue, MaxValue>& decimal) {
        out << decimal.to_string();
        return out;
    }
};

template<typename NewUnderlyingType, int NewPrecision, typename OldUnderlyingType, int OldPrecision>
inline typename std::enable_if<NewPrecision != OldPrecision, decimal_t<NewUnderlyingType, NewPrecision>>::type
decimal_cast(const decimal_t<OldUnderlyingType, OldPrecision>& oldDecimal) noexcept {
    NewUnderlyingType nominator = NewPrecision > OldPrecision ?
                static_cast<NewUnderlyingType>(oldDecimal.nominator()) * Power10<NewUnderlyingType>(NewPrecision - OldPrecision + 1)
              : static_cast<NewUnderlyingType>(oldDecimal.nominator()) / Power10<NewUnderlyingType>(OldPrecision - NewPrecision - 1);
    int lastSignificantDigit = std::abs(nominator) % 10;
    int signFactor = nominator >= 0 ? 1 : -1;
    return decimal_t<NewUnderlyingType, NewPrecision>(typename decimal_t<NewUnderlyingType, NewPrecision>::nominator_t{
            nominator / 10 + (lastSignificantDigit >= 5 ? signFactor : 0)
        });
}

template<typename NewUnderlyingType, int NewPrecision, typename OldUnderlyingType, int OldPrecision>
inline typename std::enable_if<NewPrecision == OldPrecision, decimal_t<NewUnderlyingType, NewPrecision>>::type
decimal_cast(const decimal_t<OldUnderlyingType, OldPrecision>& oldDecimal) noexcept {
    return decimal_t<NewUnderlyingType, NewPrecision>(typename decimal_t<NewUnderlyingType, NewPrecision>::nominator_t{
            static_cast<NewUnderlyingType>(oldDecimal.nominator())
        });
}

template<typename ResDecimalType, typename OldUnderlyingType, int OldPrecision>
inline ResDecimalType
decimal_cast(const decimal_t<OldUnderlyingType, OldPrecision>& oldDecimal) noexcept {
    return decimal_cast<typename ResDecimalType::underlying_type, ResDecimalType::PRECISION,
            OldUnderlyingType, OldPrecision>(oldDecimal);
}

template<typename UnderlyingType, int Precision>
const decimal_t<UnderlyingType, Precision>&
min(const decimal_t<UnderlyingType, Precision>& first, const decimal_t<UnderlyingType, Precision>& second)
{
    return first < second ? first : second;
}

template<typename UnderlyingType, int Precision>
const decimal_t<UnderlyingType, Precision>&
max(const decimal_t<UnderlyingType, Precision>& first, const decimal_t<UnderlyingType, Precision>& second)
{
    return first > second ? first : second;
}

using integer_t = decimal_t<int, 0>;
using size_t = decimal_t<size_t, 0>;
using float32_3d_t = decimal_t<int32_t, 3>;
using float32_2d_t = decimal_t<int32_t, 2>;
using ratio64_t = ranged_decimal_t<int64_t, 5, 0, 1>;

}  // namespace strict
