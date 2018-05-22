#include "namegen.h"

NameGenRuleFactory * NameGenRuleFactory::instance = NULL;

/*!
 * \brief Конструктор генератора имен
 */
NameGen::NameGen()
{

}

/*!
 * \brief Получение случайного имени для карты-персонажа
 * \return Случайное имя
 */
QString NameGen::randomActorName()
{
	currentGenerator = NameGenRuleFactory::getInstance()->getRuleGenerator("actornames");
	return currentGenerator->genByRule();
}

/*!
 * \brief Получение случайного имени для карты-действия
 * \return Случайное имя
 */
QString NameGen::randomActionName()
{
	currentGenerator = NameGenRuleFactory::getInstance()->getRuleGenerator("actionnames");
	return currentGenerator->genByRule();
}

/*!
 * \brief Получение случайного имени для карты-команды
 * \return Случайное имя
 */
QString NameGen::randomTeamName()
{
	currentGenerator = NameGenRuleFactory::getInstance()->getRuleGenerator("teamnames");
	return currentGenerator->genByRule();
}

/*!
 * \brief Получение случайного уникального ID для карты
 * \return Сгенерированный UID
 */
QString NameGen::randomUID()
{
	QString result;
	// 32 16-ичные цифры
	for (int i = 0; i < 32; i++)
	{
		result += QString::number(Random::randInt(0, 15), 16);
	}
	return result;
}

/*!
 * \brief Деструктор генератора имен
 */
NameGen::~NameGen()
{

}

/*!
 * \brief Получение случайного названия эффекта
 * \return Случайное название
 */
QString NameGen::randomEffectName()
{
	currentGenerator = NameGenRuleFactory::getInstance()->getRuleGenerator("effectnames");
	return currentGenerator->genByRule();
}

/*!
 * \brief Конструкор генератора имен по правилам
 */
ExternalRuleGen::ExternalRuleGen()
{

}

/*!
 * \brief Конструкор генератора имен по правилам
 * \param ruleSetName Название набора правил
 */
ExternalRuleGen::ExternalRuleGen(QString ruleSetName)
{
	int sizeInt = 0;
	QFile f("data/" + ruleSetName + ".dat");
	f.open(QFile::ReadOnly);
	QList<genRule> rules;
	QStringList data = QString(f.readAll()).replace("\r", "").split('\n');
	sizeInt = data.size();
	for (int i = 0; i < sizeInt; i++)
	{
		QStringList rule = QString(data.at(i)).split(" -> ");
		genRule r;
		r.ruleName = rule.at(0);
		r.ruleData = QString(rule.at(1)).split('|');
		rules.push_back(r);
	}
	ruleSet = rules;
}

/*!
 * \brief Генерация имени с использованием заданного набора правил
 * \return Сгенерированное имя
 */
QString ExternalRuleGen::genByRule()
{
	int sizeInt = 0;
	bool ok = true;
	QString result = "";
	sizeInt = ruleSet.size();
	for (int i = 0; i < sizeInt; i++)
	{
		if (ruleSet[i].ruleName == "<main>")
		{
			result = ruleSet[i].ruleData.at(Random::randInt(0, ruleSet[i].ruleData.size() - 1));
			break;
		}
	}
	do
	{
		ok = false;
		sizeInt = ruleSet.size();
		for (int i = 0; i < sizeInt; i++)
		{
			if (result.contains(ruleSet[i].ruleName))
			{
				ok = ok || true;
				int len = ruleSet[i].ruleName.length();
				int pos = result.indexOf(ruleSet[i].ruleName);
				result = result.left(pos)
						+ ruleSet[i].ruleData.at(Random::randInt(0, ruleSet[i].ruleData.size() - 1))
						+ result.right(result.length() - len - pos);

			}
		}
	} while (ok);
	return result;
}

/*!
 * \brief Конструктор интерфейса генератора имен
 */
INameGen::INameGen()
{

}

/*!
 * \brief Деструктор интерфейса генератора имен
 */
INameGen::~INameGen()
{

}

/*!
 * \brief Получение заранее сгенерированного генератора имен по правилам
 * \param ruleSetName Наименование набора правил
 * \return Объект генератора имен
 */
ExternalRuleGen * NameGenRuleFactory::getRuleGenerator(QString ruleSetName)
{
	ExternalRuleGen *erg;
	if (ruleGenSets.keys().indexOf(ruleSetName) != -1)
	{
		erg = ruleGenSets[ruleSetName];
	}
	else
	{
		erg = new ExternalRuleGen(ruleSetName);
		ruleGenSets[ruleSetName] = erg;
	}
	return erg;
}

/*!
 * \brief Конструктор фабрики генераторов имен
 */
NameGenRuleFactory::NameGenRuleFactory()
{

}

/*!
 * \brief Получение ссылки на фабрику генераторов имен
 * \return Объект фабрики генераторов имен
 */
NameGenRuleFactory *NameGenRuleFactory::getInstance()
{
	if (instance == NULL)
		instance = new NameGenRuleFactory;
	return instance;
}
