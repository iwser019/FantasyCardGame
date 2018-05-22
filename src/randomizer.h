#ifndef RANDOM_H
#define RANDOM_H

#include <QVector>

/*!
 * \brief Класс Random - генератор (псевдо)случайных чисел
 * \details Предоставляет функции генерации случайных чисел
 */
class Random
{
	public:
		Random();
		static int randInt(int from, int to);
		// выбор случайного числа по дискретному распределению
		static int randIntWeighted(QVector<int> weights);
		static double randDbl();
		static double randDbl(double from, double to);
		// розыгрыш случайной величины
		static bool probability(double prob);
};

#endif // RANDOM_H
