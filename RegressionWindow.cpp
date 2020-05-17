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

#include "RegressionWindow.h"
#include "PointSetModel.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QTextStream>

RegressionWindow::RegressionWindow
(
    QWidget* parent
):
    QMainWindow(parent),
    Ui::RegressionWindow()
{
    setupUi(this);

	connect(_actionOpen, &QAction::triggered, this, &RegressionWindow::browseCsvFile);
}

RegressionWindow::~RegressionWindow()
{
}

void RegressionWindow::showEvent
(
    QShowEvent* showEvent
)
{
	_graphWidget->centerOnLogicalCoordinate(QPointF(0.0, 0.0));
}

void RegressionWindow::browseCsvFile()
{
	auto filePath = QFileDialog::getOpenFileName(this, "Choose a file containing coordinate pairs", QDir::homePath(), "*.csv");
	if (filePath != QString(""))
	{
		loadCsv(filePath);
	}
}

void RegressionWindow::loadCsv
(
	const QString& filePath
)
{
	Regression<1>::TrainingPoints result;
	QVector<QPointF> modelPoints;
	QFile file(filePath);
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream textStream(&file);
		while (textStream.atEnd() == false)
		{
			const auto line = textStream.readLine();
			const auto splitOnComma = line.split(",");
			if (splitOnComma.length() == 2)
			{
				const auto x = splitOnComma[0].toDouble();
				const auto y = splitOnComma[1].toDouble();

				result.push_back({ x, y});
				modelPoints.push_back({x, y});
			}
		}
		file.close();
	}

	_graphWidget->setModel(std::make_unique<PointSetModel>(modelPoints));

	_regression.setTrainingPoints(result);
	_regression.train(50);
}
