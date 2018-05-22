#ifndef NAMEGEN_H
#define NAMEGEN_H

#include "defs.h"

/*!
 * \brief Структура genRule - правило граммаики для реализации
 */
struct genRule
{
		QString ruleName;
		QStringList ruleData;
};

/*!
 * \brief Класс ExternalRuleGen - генератор имен по грамматике
 * \details Используя набор правил, считанных из файла data/&lt;имя&gt;.dat, с разметкой вида: \n
 * &lt;main&gt; -&gt; x&lt;y1&gt;|...|x \n
 * &lt;y1&gt; -&gt; y|...|y \n
 * генерирует имена.
 */
class ExternalRuleGen
{
	protected:
		/*!
		 * \brief Набор правил
		 */
		QList<genRule> ruleSet;
	public:
		ExternalRuleGen();
		ExternalRuleGen(QString ruleSetName);
		QString genByRule();
};

/*!
 * \brief The NameGenRuleFactory - фабрика генераторов имен
 * \details При первом запросе создает объект нужного генератора имен, при последующих
 * отдает уже готовые генераторы
 */
class NameGenRuleFactory
{
	protected:
		/*!
		 * \brief Экземпляр
		 */
		static NameGenRuleFactory *instance;
		/*!
		 * \brief Созданные генераторы
		 */
		QMap< QString, ExternalRuleGen* > ruleGenSets;
		NameGenRuleFactory();
	public:
		static NameGenRuleFactory * getInstance();
		ExternalRuleGen *getRuleGenerator(QString ruleSetName);
};

/*!
 * \brief Класс INameGen - интерфейс оболочки генераторов имен
 */
class INameGen
{
	public:
		INameGen();
		/*!
		 * \brief Генерация имени персонажа
		 * \return Имя
		 */
		virtual QString randomActorName() = 0;
		/*!
		 * \brief Генерация имени эффекта
		 * \return Имя
		 */
		virtual QString randomEffectName() = 0;
		/*!
		 * \brief Генерация имени действия
		 * \return Имя
		 */
		virtual QString randomActionName() = 0;
		/*!
		 * \brief Генерация имени команды
		 * \return Имя
		 */
		virtual QString randomTeamName() = 0;
		/*!
		 * \brief Генерация уникального ID
		 * \return ID
		 */
		virtual QString randomUID() = 0;
		~INameGen();
};

/*!
 * \brief Класс NameGen - оболочка генератора имен
 * \details Содержит несколько готовых методов для генерации имен того или иного типа
 */
class NameGen : public INameGen
{
	protected:
		/*!
		 * \brief Текущий генератор
		 */
		ExternalRuleGen *currentGenerator;
		/*!
		 * \brief Объект фабрики генераторов имен
		 */
		NameGenRuleFactory *usedFactory;
	public:
		NameGen();
		QString randomActorName();
		QString randomEffectName();
		QString randomActionName();
		QString randomTeamName();
		QString randomUID();
		~NameGen();
};

#endif // NAMEGEN_H
