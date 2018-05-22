#include "effect.h"

/*!
 * \brief Конструктор объекта эффекта
 * \param parent Родительский объект класса QObject
 */
IEffect::IEffect(QObject *parent):QObject(parent)
{

}

/*!
 * \brief Конструктор объекта эффекта атаки
 * \param parent Родительский объект класса QObject
 */
AttackEffect::AttackEffect(QObject *parent):IEffect(parent)
{

}

/*!
 * \brief Получение имени эффекта
 * \return Имя эффекта
 */
QString AttackEffect::getName()
{
	return name;
}

/*!
 * \brief Установка нового имени для эффекта
 * \param effectName Новое имя эффекта
 */
void AttackEffect::setName(QString effectName)
{
	name = effectName;
}

/*!
 * \brief Получение численного значения именнованной характеристики
 * \param statName Наименование характеристики
 * \return Численное значение
 */
int AttackEffect::getStat(QString statName)
{
	if (statName == "power")
		return power;
	else if (statName == "cost")
		return cost;
	else if (statName == "effectType")
		return 0;
	return 0;
}

/*!
 * \brief Установка нового значения для именлванной характеристики
 * \param statName Наименование характеристики
 * \param statVal Новое числовое значение
 */
void AttackEffect::setStat(QString statName, int statVal)
{
	if (statName == "power")
		power = statVal;
	else if (statName == "cost")
		cost = statVal;
}

/*!
 * \brief Задействование эффекта
 * \param cardsToAffect Передатчики характеристик карт, на к-рые напр-но возд-е
 * \param src Передатчик характеристик карты, от которой направлено действие
 */
void AttackEffect::act(QVector<StatsTransmitter *> cardsToAffect, StatsTransmitter *src)
{

	for (int i = 0; i < cardsToAffect.size(); i++)
	{
		src->setStat("energy", -1 * cost, true);
		cardsToAffect[i]->setStat("health", -1 * power, true);
	}
}

/*!
 * \brief Получение текстовой информации об эффекте
 * \return Текстовая информация
 */
QString AttackEffect::getFullInfo()
{
	return name + " (сила "
			+ QString::number(power)
			+ ", стоимость"
			+ QString::number(cost) + ");";
}

/*!
 * \brief Получение информации об эффекте, его типе и направленности
 * \return Элемент информации об эффекте
 */
effectInfoItem AttackEffect::getInfo()
{
	effectInfoItem result;
	result.info = getFullInfo();
	result.canAffectEnemy = true;
	result.canAffectPlayer = false;
	result.effectType = 0;
	return result;
}

/*!
 * \brief Клонирование эффекта
 * \return Объект-копия эффекта
 */
IEffect *AttackEffect::clone()
{
	AttackEffect *eff = new AttackEffect();
	eff->setName(name);
	eff->setStat("power", power);
	eff->setStat("cost", cost);
	return eff;
}

/*!
 * \brief Загрузка параметров эффекта из потока данных
 * \param stream Входной поток данных
 */
void AttackEffect::loadFromStream(QDataStream *stream)
{
	*stream >> name;
	*stream >> power;
	*stream >> cost;
}

/*!
 * \brief Запись параметров эффекта в поток данных
 * \param stream Выходной поток данных
 */
void AttackEffect::saveToStream(QDataStream *stream)
{
	*stream << 'A';
	*stream << name;
	*stream << power;
	*stream << cost;
}

/*!
 * \brief Конструктор объекта эффекта помощи
 * \param parent Родительский объект класса QObject
 */
HelpEffect::HelpEffect(QObject *parent):IEffect(parent)
{

}

/*!
 * \brief Получение имени эффекта
 * \return имя эффекта
 */
QString HelpEffect::getName()
{
	return name;
}

/*!
 * \brief Установка имени эффекта
 * \param effectName Новое имя эффекта
 */
void HelpEffect::setName(QString effectName)
{
	name = effectName;
}

/*!
 * \brief Получение численного значения именованной характеристики эффектам
 * \param statName Наименование характеристики
 * \return Численное значение
 */
int HelpEffect::getStat(QString statName)
{
	if (statName == "amount")
		return amount;
	else if (statName == "helptype")
		return helpType;
	else if (statName == "limit")
		return cardsLimit;
	else if (statName == "effectType")
		return 1;
	return 0;
}

/*!
 * \brief Установка значения для именованной характеристики эффекта
 * \param statName Наименование характеристики
 * \param statVal Новое значение
 */
void HelpEffect::setStat(QString statName, int statVal)
{
	if (statName == "amount")
		amount = statVal;
	else if (statName == "helptype")
		helpType = statVal;
	else if (statName == "limit")
		cardsLimit = statVal;
}

/*!
 * \brief Задействование эффекта
 * \param cardsToAffect Передатчики характеристик карт, на к-рые напр-но возд-е
 * \param src Передатчик характеристик карты, от которой направлено действие
 */
void HelpEffect::act(QVector<StatsTransmitter *> cardsToAffect, StatsTransmitter *src)
{
	for (int i = 0; i < cardsToAffect.size(); i++)
	{
		if (helpType == 0)
			cardsToAffect[i]->setStat("health", amount, true);
		else
			cardsToAffect[i]->setStat("energy", amount, true);
	}
}

/*!
 * \brief Получение текстовой информации об эффекте
 * \return Текстовая информация
 */
QString HelpEffect::getFullInfo()
{
	return name + " (уровень "
			+ QString::number(amount)
			+ ");";
}

/*!
 * \brief Получение информации об эффекте, его типе и направленности
 * \return Элемент информации об эффекте
 */
effectInfoItem HelpEffect::getInfo()
{
	effectInfoItem result;
	result.info = getFullInfo();
	result.canAffectEnemy = false;
	result.canAffectPlayer = true;
	result.effectType = 1;
	return result;
}

/*!
 * \brief Клонирование объекта эффекта
 * \return Объект-копия эффекта
 */
IEffect *HelpEffect::clone()
{
	HelpEffect *eff = new HelpEffect();
	eff->setName(name);
	eff->setStat("amount", amount);
	eff->setStat("helpType", helpType);
	eff->setStat("cardsLimit", cardsLimit);
	return eff;
}

/*!
 * \brief Загрузка параметров эффекта из потока данных
 * \param stream Входной поток данных
 */
void HelpEffect::loadFromStream(QDataStream *stream)
{
	*stream >> name;
	*stream >> amount;
	*stream >> helpType;
	*stream >> cardsLimit;
}

/*!
 * \brief Запись параметров эффекта в поток данных
 * \param stream Выходной поток данных
 */
void HelpEffect::saveToStream(QDataStream *stream)
{
	*stream << 'H';
	*stream << name;
	*stream << amount;
	*stream << helpType;
	*stream << cardsLimit;
}

/*!
 * \brief Конструктор объекта дестабилизатора эффекта
 * \param parent Родительский объект класса QObject
 */
EffectDestabilizer::EffectDestabilizer(QObject *parent):IEffect(parent)
{

}

/*!
 * \brief Конструктор объекта дестабилизатора эффекта с указанием ядра
 * \param core Исхоный эффект-ядро
 * \param parent Родительский эффект класса QObject
 */
EffectDestabilizer::EffectDestabilizer(IEffect *core, QObject *parent):EffectDestabilizer(parent)
{
	coreEffect = core;
}

/*!
 * \brief Получение имени эффекта
 * \return Имя эффекта совместно с именем ядра
 */
QString EffectDestabilizer::getName()
{
	return (coreEffect->getName() + " (" + name + ")");
}

/*!
 * \brief Установка имени эффекта
 * \param effectName Новое имя эффекта
 */
void EffectDestabilizer::setName(QString effectName)
{
	name = effectName;
}

/*!
 * \brief Получение численного значения именованной характеристики эффектам
 * \param statName Наименование характеристики
 * \return Численное значение
 */
int EffectDestabilizer::getStat(QString statName)
{
	if (statName == "distAmount")
		return amount;
	else
		return coreEffect->getStat(statName);
}

/*!
 * \brief Установка значения для именованной характеристики эффекта
 * \param statName Наименование характеристики
 * \param statVal Новое значение
 */
void EffectDestabilizer::setStat(QString statName, int statVal)
{
	if (statName == "distAmount")
		amount = statVal;
	else
		coreEffect->setStat(statName, statVal);
}

/*!
 * \brief Задействование эффекта
 * \param cardsToAffect Передатчики характеристик карт, на к-рые напр-но возд-е
 * \param src Передатчик характеристик карты, от которой направлено действие
 */
void EffectDestabilizer::act(QVector<StatsTransmitter *> cardsToAffect, StatsTransmitter *src)
{
	coreEffect->act(cardsToAffect, src);
	int effType = coreEffect->getStat("effectType");
	switch (effType)
	{
		case 0:
			{
				for (int i = 0; i < cardsToAffect.size(); i++)
					cardsToAffect[i]->setStat("health", -1 * (Random::randInt(1, amount)), true);
				break;
			}
		case 1:
			{
				for (int i = 0; i < cardsToAffect.size(); i++)
				{
					if (coreEffect->getStat("helpType") == 0)
						cardsToAffect[i]->setStat("health", amount, true);
					else
						cardsToAffect[i]->setStat("energy", amount, true);
				}
			}
	}
}

/*!
 * \brief Получение текстовой информации об эффекте и его ядре
 * \return Тектстовая информация
 */
QString EffectDestabilizer::getFullInfo()
{
	return coreEffect->getFullInfo() + " (" + name + " (" + QString::number(amount) + "))";
}

/*!
 * \brief Получение информации об эффекте, его типе и направленности
 * \return Элемент информации об эффекте
 */
effectInfoItem EffectDestabilizer::getInfo()
{
	effectInfoItem result = coreEffect->getInfo();
	result.info = getFullInfo();
	return result;
}

/*!
 * \brief Клонирование объекта эффекта
 * \return Объект-копия эффекта
 */
IEffect *EffectDestabilizer::clone()
{
	EffectDestabilizer *eff = new EffectDestabilizer(coreEffect->clone());
	eff->setName(name);
	eff->setStat("distAmount", amount);
	return eff;
}

/*!
 * \brief Загрузка параметров эффекта и его ядра из потока данных
 * \param stream Входной поток данных
 */
void EffectDestabilizer::loadFromStream(QDataStream *stream)
{
	*stream >> name;
	*stream >> amount;
	IEffect *eff;
	int effType;
	*stream >> effType;
	switch (effType)
	{
		case 'A':
			eff = new AttackEffect();
			break;
		case 'H':
			eff = new HelpEffect();
			break;
		case 'D':
			eff = new EffectDestabilizer();
			break;
	}
	eff->loadFromStream(stream);
	coreEffect = eff;
}

/*!
 * \brief Запись параметров эффекта и его ядра в поток данных
 * \param stream Выходной поток данных
 */
void EffectDestabilizer::saveToStream(QDataStream *stream)
{
	*stream << 'D';
	*stream << name;
	*stream << amount;
	coreEffect->saveToStream(stream);
}

/*!
 * \brief Конструктор объекта звена цепочки эффектов для последовательного действия
 * \param effect Эффект в звене
 * \param next Следующее звено
 */
EffectChainlink::EffectChainlink(IEffect *effect, EffectChainlink *next)
{
	this->effect = effect;
	this->next = next;
}

/*!
 * \brief Задействование цепочки эффектов, начиная с данного звена
 * \param cardsToAffect Передатчики характеристик карт, на к-рые напр-но возд-е
 * \param src Передатчик характеристик карты, от которой направлено действие
 */
void EffectChainlink::act(QVector<StatsTransmitter *> cardsToAffect, StatsTransmitter *src)
{
	if (effect != nullptr)
		effect->act(cardsToAffect, src);
	if (next != nullptr)
		next->act(cardsToAffect, src);
}

/*!
 * \brief EffectChainlink::~EffectChainlink
 * Деструктор цепочки эффектов
 */
EffectChainlink::~EffectChainlink()
{
	if (next != nullptr)
		delete next;
}
