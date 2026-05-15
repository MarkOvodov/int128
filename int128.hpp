#ifndef EXPRESSION_INT128_HPP
#define EXPRESSION_INT128_HPP

#include <cstdint>
#include <string>
#include <map>
#include <memory>
#include <ostream>

class Int128 {
    int64_t high;
    uint64_t low;

public:
    // Конструирование без аргументов
    Int128();

    // Конструирование от int64_t
    explicit Int128(int64_t num);

    // Конструирование от std::string_view
    explicit Int128(std::string_view str);

    // Явное приведение к int64_t
    explicit operator int64_t() const;

    // Явное приведение к double
    explicit operator double() const;

    // Перевод в строку: std::string str()
    std::string str() const;

    // Сложение: +, +=
    Int128 operator+(const Int128& rhs) const;
    Int128& operator+=(const Int128& rhs);

    // Вычитание: -, -=
    Int128 operator-(const Int128& rhs) const;
    Int128& operator-=(const Int128& rhs);

    // Умножение: *, *=
    Int128 operator*(const Int128& rhs) const;
    Int128& operator*=(const Int128& rhs);

    // Деление: /, /=
    Int128 operator/(const Int128& rhs) const;
    Int128& operator/=(const Int128& rhs);

    // Унарный минус: -
    Int128 operator-() const;

    // Сравнение на равенство: ==, !=
    bool operator==(const Int128& rhs) const;
    bool operator!=(const Int128& rhs) const;

    // Вывод в поток: <<
    friend std::ostream& operator<<(std::ostream& out, const Int128& board);

    // ============================== Custom ==============================

private:
    // Конструирование от int64_t(high) и uint64_t(low)
    explicit Int128(int64_t high, uint64_t low);

    // Сравнение побитовое (беззнаковое): <, <=
    bool bit_lt(const Int128& rhs) const;
    bool bit_le(const Int128& rhs) const;

public:
    // Строка из бит (с разделителем)
    std::string bit_string(bool separator = false) const;

    // Побитовое NOT (~)
    Int128 operator~() const;

    // Константы 0 и 1
    static const Int128 ZERO;
    static const Int128 ONE;

    // Битовый сдвиг влево
    Int128 operator<<(int shift) const;
    Int128& operator<<=(int shift);

    // Битовый сдвиг вправо
    Int128 operator>>(int shift) const;
    Int128& operator>>=(int shift);

    // Остаток от деления: %, %=
    Int128 operator%(const Int128& rhs) const;
    Int128& operator%=(const Int128& rhs);

    // Сравнение: <, <=
    bool operator<(const Int128& rhs) const;
    bool operator<=(const Int128& rhs) const;

    // Сравнение: >, >=
    bool operator>(const Int128& rhs) const;
    bool operator>=(const Int128& rhs) const;

    // Побитовое И: &, &=
    Int128 operator&(const Int128& rhs) const;
    Int128& operator&=(const Int128& rhs);

    // Побитовое ИЛИ: |, |=
    Int128 operator|(const Int128& rhs) const;
    Int128& operator|=(const Int128& rhs);

    // Модуль числа: abs()
    Int128 abs() const;
};

using Variables = std::map<std::string, Int128>;

class Expression {
public:
    virtual Int128 eval(const Variables& vars) const = 0;
    virtual Expression* clone() const = 0;
    virtual void print(std::ostream& out) const = 0;
    virtual ~Expression() = default;

    friend std::ostream& operator<<(std::ostream& out, const Expression& expr);
};

class Const : public Expression {
    Int128 value;
public:
    explicit Const(Int128 value);
    Int128 eval(const Variables& vars) const override;
    Const* clone() const override;
    void print(std::ostream& out) const override;
};

class Variable : public Expression {
    std::string name;
public:
    explicit Variable(std::string name);
    Int128 eval(const Variables& vars) const override;
    Variable* clone() const override;
    void print(std::ostream& out) const override;
};

class UnaryExpr : public Expression {
protected:
    std::unique_ptr<Expression> operand;
public:
    explicit UnaryExpr(const Expression& expr);
    UnaryExpr(const UnaryExpr& other);
};

class BinaryExpr : public Expression {
protected:
    std::unique_ptr<Expression> lhs, rhs;
public:
    BinaryExpr(const Expression& l, const Expression& r);
    BinaryExpr(const BinaryExpr& other);
};

class Negate : public UnaryExpr {
public:
    using UnaryExpr::UnaryExpr;
    Int128 eval(const Variables& vars) const override;
    Negate* clone() const override;
    void print(std::ostream& out) const override;
};

class Add : public BinaryExpr {
public:
    using BinaryExpr::BinaryExpr;
    Int128 eval(const Variables& vars) const override;
    Add* clone() const override;
    void print(std::ostream& out) const override;
};

class Subtract : public BinaryExpr {
public:
    using BinaryExpr::BinaryExpr;
    Int128 eval(const Variables& vars) const override;
    Subtract* clone() const override;
    void print(std::ostream& out) const override;
};

class Multiply : public BinaryExpr {
public:
    using BinaryExpr::BinaryExpr;
    Int128 eval(const Variables& vars) const override;
    Multiply* clone() const override;
    void print(std::ostream& out) const override;
};

class Divide : public BinaryExpr {
public:
    using BinaryExpr::BinaryExpr;
    Int128 eval(const Variables& vars) const override;
    Divide* clone() const override;
    void print(std::ostream& out) const override;
};

inline Add operator+(const Expression& l, const Expression& r) { return Add(l, r); }
inline Subtract operator-(const Expression& l, const Expression& r) { return Subtract(l, r); }
inline Multiply operator*(const Expression& l, const Expression& r) { return Multiply(l, r); }
inline Divide operator/(const Expression& l, const Expression& r) { return Divide(l, r); }
inline Negate operator-(const Expression& e) { return Negate(e); }

#endif  // EXPRESSION_INT128_HPP