#ifndef UTIL_H
#define UTIL_H

#include "defs.h"

template <class T>
/*!
 * \brief Интерфейс коллекции, используемой для хранения объектов
 */
class ICollection
{
	public:
		/*!
		 * \brief Конструктор
		 */
		ICollection();
		/*!
		 * \brief Конструктор
		 * \param _data Исходные данные
		 */
		ICollection(QList<T> _data);
		/*!
		 * \brief Получение объекта
		 * \param id Индекс объекта
		 * \return Объект
		 */
		virtual T at(int id) = 0;
		/*!
		 * \brief Удаление объекта
		 * \param id Индекс
		 */
		virtual void removeAt(int id) = 0;
		/*!
		 * \brief Замена объекта
		 * \param id Индекс
		 * \param src Конечный объект
		 */
		virtual void replaceAt(int id, T src) = 0;
		/*!
		 * \brief Получение количества объектов
		 * \return Количество объектов
		 */
		virtual int count() = 0;
};

template <class T>
/*!
 * \brief ИНтерфейс итератора, использующегося для последовательной обработки
 * объектов
 */
class IIterator
{
	public:
		/*!
		 * \brief Конструктор
		 */
		IIterator();
		/*!
		 * \brief Получение следующего элемента
		 * \return Следующий элемент
		 */
		virtual T getNext() = 0;
		/*!
		 * \brief Удаление текущего элемента
		 */
		virtual void removeCurrentElement() = 0;
		/*!
		 * \brief Проверка присутствия следующего элемента
		 * \return Результат проверки
		 */
		virtual bool hasNext() = 0;
};

#endif // UTIL_H
