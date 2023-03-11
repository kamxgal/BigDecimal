/*
 * Author: Kamil Galant
 * e-mail: kamil.galant@gmail.com
 * github: https://github.com/kamxgal
 *
 * Copyright (C) 2023 Kamil Galant. All Rights Reserved.
 *
 * License
 * BigDecimal is released under BSD-3-Clause license.
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

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <numeric>
#include <string>
#include <sstream>


namespace big_decimal
{

template<typename RetT>
constexpr RetT Power10(int n) noexcept {
    RetT res = static_cast<RetT>(1);
    for (int i=1; i<=n; ++i) {
        res *= static_cast<RetT>(10);
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
    static constexpr UnderlyingType DOUBLED_DENOMINATOR = Power10<UnderlyingType>(PRECISION+PRECISION);
    struct nominator_t { UnderlyingType value{}; };

    explicit decimal_t(std::string num) {
        double parsed;
        std::istringstream ss(num);
        ss >> parsed;
        mNominator = decimal_t(parsed).mNominator;
    }

    template<typename FloatingT,
             std::enable_if_t<std::is_floating_point<FloatingT>::value, bool> = true>
    explicit decimal_t(FloatingT num) {
        int lastDigit = static_cast<int>(std::abs(num) * DENOMINATOR * 10) % 10;
        int signFactor = num >=0 ? 1 : -1;
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

    constexpr bool operator<(const decimal_t<underlying_type, PRECISION>& other) { return mNominator < other.mNominator; }
    constexpr bool operator<=(const decimal_t<underlying_type, PRECISION>& other) { return mNominator <= other.mNominator; }
    constexpr bool operator==(const decimal_t<underlying_type, PRECISION>& other) { return mNominator == other.mNominator; }
    constexpr bool operator>=(const decimal_t<underlying_type, PRECISION>& other) { return mNominator >= other.mNominator; }
    constexpr bool operator>(const decimal_t<underlying_type, PRECISION>& other) { return mNominator > other.mNominator; }

    constexpr decimal_t<underlying_type, PRECISION> operator+(const decimal_t<underlying_type, PRECISION>& rhs) const {
        return decimal_t<UnderlyingType, PRECISION>{nominator_t{mNominator.value + rhs.mNominator.value}};
    }
    constexpr decimal_t<underlying_type, PRECISION> operator-(const decimal_t<underlying_type, PRECISION>& rhs) const {
        return decimal_t<UnderlyingType, PRECISION>{nominator_t{mNominator.value - rhs.mNominator.value}};
    }
    constexpr decimal_t<underlying_type, PRECISION> operator*(const decimal_t<underlying_type, PRECISION>& rhs) const {
        underlying_type res = mNominator.value * rhs.mNominator.value;
        underlying_type fractionPart = std::abs(res) % DENOMINATOR;
        underlying_type signFactor = res >= 0 ? 1 : -1;
        return decimal_t<UnderlyingType, PRECISION>{
            nominator_t{mNominator.value * rhs.mNominator.value / DENOMINATOR + (fractionPart >= HALF_DENOMINATOR ? signFactor : 0)}
        };
    }
    constexpr decimal_t<underlying_type, PRECISION> operator/(const decimal_t<underlying_type, PRECISION>& rhs) const {
        nominator_t res{mNominator.value};
        res.value *= Power10<underlying_type>(PRECISION+1);
        res.value /= rhs.nominator();
        int lastSignificantDigit = mNominator.value % 10;
        res.value /= 10;
        res.value += lastSignificantDigit >= 5 ? 1 : 0;
        return decimal_t<UnderlyingType, PRECISION>{res};
    }
    constexpr decimal_t<underlying_type, PRECISION>& operator+=(const decimal_t<underlying_type, PRECISION>& rhs) {
        mNominator.value += rhs.mNominator.value;
        return *this;
    }
    constexpr decimal_t<underlying_type, PRECISION>& operator-=(const decimal_t<underlying_type, PRECISION>& rhs) {
        mNominator.value -= rhs.mNominator.value;
        return *this;
    }
    constexpr decimal_t<underlying_type, PRECISION>& operator*=(const decimal_t<underlying_type, PRECISION>& rhs) {
        underlying_type res = mNominator.value * rhs.mNominator.value;
        underlying_type fractionPart = std::abs(res) % DENOMINATOR;
        underlying_type signFactor = res >= 0 ? 1 : -1;
        mNominator.value = mNominator.value * rhs.mNominator.value / DENOMINATOR + (fractionPart >= HALF_DENOMINATOR ? signFactor : 0);
        return *this;
    }

    template<int RhsDecimalPrecision>
    constexpr decimal_t<underlying_type, PRECISION>& operator*=(const decimal_t<underlying_type, RhsDecimalPrecision>& rhs) {
        const underlying_type integerPart = this->integer_part();
        underlying_type fractionPart = this->fraction_part();
        const underlying_type rhsIntegerPart = rhs.integer_part();
        underlying_type rhsFractionPart = rhs.fraction_part();

        // first = least common denominator; second=doubled least common denominator
        const auto denominatorInfo = []() -> std::pair<underlying_type, underlying_type> {
            if (DENOMINATOR < std::decay_t<decltype(rhs)>::DENOMINATOR) {
                return {std::decay_t<decltype(rhs)>::DENOMINATOR, std::decay_t<decltype(rhs)>::DOUBLED_DENOMINATOR};
            } else {
                return {DENOMINATOR, DOUBLED_DENOMINATOR};
            }
        }();

        // setting values of fractionPart and rhsFractionPart as having common denominator
        fractionPart *= denominatorInfo.first / DENOMINATOR;
        rhsFractionPart *= denominatorInfo.first / std::decay_t<decltype(rhs)>::DENOMINATOR;

        underlying_type resultInteger = std::abs(integerPart * rhsIntegerPart);
        underlying_type signFactor = integerPart * rhsIntegerPart >= 0 ? 1 : -1;
        // commonFraction has denominator equal to commonDenominator
        underlying_type commonFraction = std::abs(integerPart) * rhsFractionPart + fractionPart * std::abs(rhsIntegerPart);
        resultInteger += commonFraction / denominatorInfo.first;
        commonFraction %= denominatorInfo.first;
        // from now on denominator of commonFraction equals commonDenominator*commonDenominator
        commonFraction *= denominatorInfo.first;
        commonFraction += fractionPart * rhsFractionPart;
        // from now on commonFraction is treated as if its denominator is equal Power10<underlying_type>(PRECISION + 1);
        commonFraction /= denominatorInfo.second / Power10<underlying_type>(PRECISION + 1);
        const int lastSignificantDigit = commonFraction % 10;
        commonFraction /= 10;
        commonFraction += lastSignificantDigit >= 5 ? 1 : 0;

        mNominator.value = resultInteger * DENOMINATOR + commonFraction;
        mNominator.value *= signFactor;
        return *this;
    }

    template<int RhsDecimalPrecision>
    constexpr decimal_t<underlying_type, PRECISION>& operator/=(const decimal_t<underlying_type, RhsDecimalPrecision>& rhs) {
        mNominator.value *= Power10<underlying_type>(std::decay_t<decltype(rhs)>::PRECISION+1);
        mNominator.value /= rhs.nominator();
        int lastSignificantDigit = std::abs(mNominator.value) % 10;
        int signFactor = mNominator.value >= 0 ? 1 : -1;
        mNominator.value /= 10;
        mNominator.value += lastSignificantDigit >= 5 ? signFactor : 0;
        return *this;
    }

    friend bool operator==(const decimal_t<underlying_type, PRECISION>& lhs,
                           const decimal_t<underlying_type, PRECISION>& rhs) {
        return lhs.mNominator.value == rhs.mNominator.value;
    }

    friend bool operator!=(const decimal_t<underlying_type, PRECISION>& lhs,
                           const decimal_t<underlying_type, PRECISION>& rhs) {
        return lhs.mNominator.value != rhs.mNominator.value;
    }

    friend std::ostream& operator<<(std::ostream& out, const decimal_t<underlying_type, PRECISION>& decimal) {
        out << decimal.to_string();
        return out;
    }

    float to_float() const { return static_cast<float>(mNominator.value) / DENOMINATOR; }
    double to_double() const { return static_cast<double>(mNominator.value) / DENOMINATOR; }
    std::string to_string() const {
        std::stringstream ss;
        ss << integer_part() << '.' << std::setfill('0') << std::setw(PRECISION) << fraction_part();
        return ss.str();
    }

private:
    nominator_t mNominator{};
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

template<typename NewUnderlyingType, int Precision, typename OldUnderlyingType, int OldPrecision>
inline typename std::enable_if<Precision == OldPrecision, decimal_t<NewUnderlyingType, Precision>>::type
decimal_cast(const decimal_t<OldUnderlyingType, OldPrecision>& oldDecimal) noexcept {
    return decimal_t<NewUnderlyingType, Precision>(typename decimal_t<NewUnderlyingType, Precision>::nominator_t{
            static_cast<NewUnderlyingType>(oldDecimal.nominator())
        });
}

}  // namespace big_decimal
