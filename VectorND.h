#ifndef VECTORND_H
#define VECTORND_H

#include <QVector>

//Copyright(c) 2020 Austin Simpson
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#include "Range.h"

template <int n>
class VectorND
{
public:
    VectorND() : _values(n, 0.0){}
	VectorND(std::initializer_list<qreal> list)
	{
		Q_ASSERT (list.size() == n);
		for (auto value : list)
		{
			_values.push_back(value);
		}
	}

    VectorND(double fillValue) : _values(n, fillValue) {}
    VectorND(const VectorND<n>& other) : _values(other._values){}

	VectorND<n>& operator= (const VectorND<n>& other)
	{
		for (auto index : range(0, n))
		{
			_values[index] = other[index];
		}
		return *this;
	}

	VectorND<n>& operator= (std::initializer_list<qreal> list)
	{
		Q_ASSERT (list.size() == n);
		for (auto value : list)
		{
			_values.push_back(value);
		}
	}

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
		(*this) += (other * (-1.0));
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

	VectorND<n> operator/(double scalar) const
	{
		return operator*(1.0 / scalar);
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

	VectorND<n-1> asNonhomogenous() const
	{
		VectorND<n-1> result(0.0);

		for (int index : range(1, n))
		{
			result[index - 1] = _values[index];
		}
		return result;
	}
private:
    QVector<qreal> _values;
};


template <size_t m>
VectorND<m> operator*(double scalar, const VectorND<m>& vector)
{
    //Since multiplication of vectors and scalars commute, I just define this function in terms of the class function.
    return vector * scalar;
}

#endif // VECTORND_H
