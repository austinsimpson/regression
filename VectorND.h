#ifndef VECTORND_H
#define VECTORND_H

#include <QVector>

QVector<int> range(int startInclusive, int endExclusive)
{
    QVector<int> result(endExclusive - startInclusive);
    std::iota(result.begin(), result.end(), startInclusive);
    return result;
}

template <int n>
class VectorND
{
public:
    VectorND() : _values(n, 0.0){}
    VectorND(double fillValue) : _values(n, fillValue) {}
    VectorND(const VectorND<n>& other) : _values(other._values){}

    constexpr int dimension() const
    {
        return n;
    }

    qreal operator[](int i) const
    {
        if (0 <= i && i < n)
        {
            return _values[i];
        }
        return 0.0;
    }

    qreal& operator[](int i)
    {
        if (0 <= i && i < n)
        {
            return _values[i];
        }
        throw;
    }

    VectorND<n> operator+(const VectorND<n>& vector) const
    {
        VectorND<n> result;
        for (auto index : range(0, n))
        {
            result[index] = (*this)[index] + vector[index];
        }
        return result;
    }

    VectorND<n>& operator+=(const VectorND<n>& other)
    {
        for (auto index : range(0, n))
        {
            (*this)[index] += other[index];
        }
        return *this;
    }

    VectorND<n> operator-(const VectorND<n>& other) const
    {
        return (*this) + (-1.0 * other);
    }

    VectorND<n>& operator-=(const VectorND<n>& other)
    {
        (*this) += ((-1.0) * other);
        return *this;
    }

    qreal operator*(const VectorND<n>& vector) const
    {
        qreal result = 0.0;

        for (auto index : range(0, n))
        {
            result += (*this)[index] * vector[index];
        }

        return result;
    }

    VectorND<n> operator*(double scalar) const
    {
        VectorND<n> result;
        for (auto index : range(0, n))
        {
            result[index] = (*this)[index] * scalar;
        }
        return result;
    }

    VectorND<n + 1> asHomogenous() const
    {
        VectorND<n+1> result(1.0);
        for (int index : range(0, n))
        {
            result[index + 1] = (*this)[index];
        }
        return result;
    }
private:
    QVector<qreal> _values;
};


template <size_t n>
VectorND<n> operator*(double scalar, const VectorND<n>& vector)
{
    //Since multiplication of vectors and scalars commute, I just define this function in terms of the class function.
    return vector * scalar;
}

#endif // VECTORND_H
