#ifndef RANGE_H
#define RANGE_H

#include <QVector>

const inline QVector<int> range(int startInclusive, int endExclusive)
{
		QVector<int> result(endExclusive - startInclusive);
		std::iota(result.begin(), result.end(), startInclusive);
		return result;
}

#endif // RANGE_H
