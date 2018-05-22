#include "randomizer.h"

/*!
 * \brief Конструктор объекта генератора псевдослучайных чисел
 */
Random::Random()
{

}

/*!
 * \brief Генерация псевдослучайного целого числа
 * \param from Начальная величина (включительно)
 * \param to Конечная величина (включительно)
 * \return Псевдослучайное целое число
 */
int Random::randInt(int from, int to)
{
	if (from != to)
		return (qrand()%(to - from + 1)) + from;
	else
		return from;
}

/*!
 * \brief Генерация псевдослучайного дискретного номера
 * \param weights Набор весовых величин
 * \return Псевдослучайный номер
 */
int Random::randIntWeighted(QVector<int> weights)
{
	int weightsCount = weights.size();
	int sum = 0, totalsum = 0;
	for (int i = 0; i < weightsCount; i++)
		totalsum += weights[i];
	int randNum = randInt(0, totalsum - 1);
	for (int i = 0; i < weightsCount; i++)
	{
		sum += weights[i];
		if (randNum < sum)
			return i;
	}
}

/*!
 * \brief Генерация псевдослучайного вещественного числа
 * \return Псевдослучайное вещественное число в диапазоне [0.0;1.0]
 */
double Random::randDbl()
{
	return ((double)qrand()) / (double)RAND_MAX;
}

/*!
 * \brief Генерация псевдослучайного вещественного числа
 * \param from Начальная величина (включительно)
 * \param to Конечная величина (включительно)
 * \return Псевдослучайное вещественное число
 */
double Random::randDbl(double from, double to)
{
	return from + (to - from) * randDbl();
}

/*!
 * \brief Розыгрыш случайного события
 * \param prob Вероятность
 * \return Результат
 */
bool Random::probability(double prob)
{
	return (randDbl() < prob);
}
