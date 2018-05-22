#include "card.h"

/*!
 * \brief Конструктор карт, наследуемых от ICard
 * \param parent - родительский объект класса QObject
 */
ICard::ICard(QObject *parent):QObject(parent)
{

}
template <typename T>
/*!
 * \brief Конструктор объекта интерфейса коллекции
 */
ICollection<T>::ICollection()
{

}

/*!
 * \brief Конструктор объекта коллекции
 */
Collection::Collection()
{

}

/*!
 * \brief Конструктор объекта коллекции карт
 * \param _data Источник объектов карт
 */
Collection::Collection(QList<ICard *> _data):Collection()
{
	data = _data;
}

/*!
 * \brief Получение объекта карты из коллекции
 * \param id Позиция
 * \return Объект карты
 */
ICard * Collection::at(int id)
{
	return data[id];
}

/*!
 * \brief Удаление карты из коллекции на позиции
 * \param id Позиция карты
 */
void Collection::removeAt(int id)
{
	data.removeAt(id);
}

/*!
 * \brief Замена объекта карты на требуемой позиции
 * \param id Позиция
 * \param src Объект карты
 */
void Collection::replaceAt(int id, ICard *src)
{
	if (id < data.size())
		data[id] = src;
}

/*!
 * \brief Получение количества карт в коллекции
 * \return Количество карт
 */
int Collection::count()
{
	return data.size();
}

/*!
 * \brief Получение итератор коллекции
 * \return Итератор коллекции
 */
Iterator *Collection::getIterator()
{
	return new Iterator(this);
}


template <typename T>
/*!
 * \brief Конструктор объекта интерфейса итератора
 */
IIterator<T>::IIterator()
{

}

/*!
 * \brief Конструктор объекта итератора
 */
Iterator::Iterator()
{

}

/*!
 * \brief Конструктор объекта итератора с указанием коллекции
 * \param coll Коллекция карт
 */
Iterator::Iterator(ICollection<ICard *> *coll):Iterator()
{
	data = coll;
	currID = -1;
}

/*!
 * \brief Получение следующего объекта карты
 * \return Объект карты
 */
ICard * Iterator::getNext()
{
	return data->at(++currID);
}

/*!
 * \brief Удаление текущего элемента
 */
void Iterator::removeCurrentElement()
{
	data->removeAt(currID);
}

/*!
 * \brief Проверка на то, существует ли следующий элемент
 * \return Результат проверки
 */
bool Iterator::hasNext()
{
	return ((currID + 1) < data->count());
}

/*!
 * \brief Конструктор карты-персонажа (класс ActorCard)
 * \param parent Родительский объект класса QObject
 */
ActorCard::ActorCard(QObject *parent):ICard(parent)
{
	cio = NULL;
	NameGen *ng = new NameGen();
	bg = QColor(Random::randInt(0, 255),
				Random::randInt(0, 255),
				Random::randInt(0, 255));
	healthInit = health = 100;
	energyInit = energy = 100;
	selectedAction = -1;
	uid = ng->randomUID();
	delete ng;
}

/*!
 * \brief Задействование карты
 * \param cardsToAffect Список ссылок на объекты карт, на которые предполагается воздействие
 */
void ActorCard::act(QList<ICard *> cardsToAffect)
{
	GameSession *gs = GameSession::getInstance();
	if (energy <= 0)
		return;
	QVector<StatsTransmitter *> transmitters;
	QMap< StatsTransmitter *, ICard * > transMapping;
	StatsTransmitter *srcTrans = new StatsTransmitter();
	for (int i = 0; i < cardsToAffect.size(); i++)
	{
		StatsTransmitter *st = new StatsTransmitter();
		connect(st, SIGNAL(sendStat(QString,int,bool)), cardsToAffect[i], SLOT(acceptStat(QString,int,bool)));
		transmitters.push_back(st);
		transMapping[st] = cardsToAffect[i];
	}
	connect(srcTrans, SIGNAL(sendStat(QString,int,bool)), this, SLOT(acceptStat(QString,int,bool)));
	int effectsNum = actionEffect.size();
	if (effectsNum == 1)
	{
		gs->msgRepeater(this->getName() + " использует " + actionEffect[0]->getName());
		actionEffect[0]->act(transmitters, srcTrans);
	}
	else
	{
		if (selectedAction != -1)
		{
			actionEffect[selectedAction]->act(transmitters, srcTrans);
			selectedAction = -1;
		}
		else
		{
			QList<IEffect *> pick = defaultSelector->selectEffect(actionEffect);

			for (int i = 0; i < pick.size(); i++)
			{
				gs->msgRepeater(this->getName() + " использует " + pick[i]->getName());
			}
			QVector<StatsTransmitter *> selectedTrans;
			for (int j = 0; j < transmitters.size(); j++)
			{
				if ((transMapping[transmitters[j]])->getStat("health") > 0)
				{
					gs->msgRepeater(this->getName() + " воздействует на " + transMapping[transmitters[j]]->getName());
					selectedTrans.push_back(transmitters[j]);
				}
			}
			EffectChainlink *efc = new EffectChainlink(pick.takeLast());
			while (!pick.empty())
				efc = new EffectChainlink(pick.takeLast(), efc);
			efc->act(selectedTrans, srcTrans);
			//pick[i]->act(selectedTrans, srcTrans);
			delete efc;
		}
	}
	while (!transmitters.empty())
		delete transmitters.takeAt(0);
	delete srcTrans;

}

/*!
 * \brief Получение имени карты
 * \return Имя карты
 */
QString ActorCard::getName()
{
	return name;
}

/*!
 * \brief Установка имени карты
 * \param cardName Новое имя карты
 */
void ActorCard::setName(QString cardName)
{
	name = cardName;
}

/*!
 * \brief Получение уникального ID карты
 * \return Уникальный ID карты
 */
QString ActorCard::getUID()
{
	return uid;
}

/*!
 * \brief Установка ID карты
 * \param id Новый ID карты
 */
void ActorCard::setUID(QString id)
{
	uid = id;
}

/*!
 * \brief Получение цвета карты для отрисовки
 * \return Цвет карты
 */
QColor ActorCard::getBG()
{
	return bg;
}

/*!
 * \brief Установка цвета карты, использующегося для отрисовки
 * \param bg Новый цвет
 */
void ActorCard::setBG(QColor bg)
{
	this->bg = bg;
}

/*!
 * \brief Получение определенного параметра карты
 * \param statName Название параметра
 * \return Числовое значение параметра
 */
int ActorCard::getStat(QString statName)
{
	if (statName == "health")
		return health;
	else if (statName == "energy")
		return energy;
	else if (statName == "healthInit")
		return healthInit;
	else if (statName == "energyInit")
		return energyInit;
	else if (statName == "actionNum")
		return actionEffect.size();
	else return -1;
}

/*!
 * \brief Установка значения определенного параметра карты
 * \param statName Название параметра
 * \param statVal Значение параметра
 * \param isInc Определяет, должно значение заменять прежнее или добавляться к нему
 */
void ActorCard::setStat(QString statName, int statVal, bool isInc)
{
	if (statName == "health")
	{
		if (isInc)
			health += statVal;
		else
			health = statVal;
	}
	else if (statName == "energy")
	{
		if (isInc)
			energy += statVal;
		else
			energy = statVal;
	}
	else if (statName == "healthInit")
	{
		if (isInc)
			healthInit += statVal;
		else
			healthInit = statVal;
	}
	else if (statName == "energyInit")
	{
		if (isInc)
			energyInit += statVal;
		else
			energyInit = statVal;
	}
	else if (statName == "selectedAction")
		selectedAction = statVal;
	updateStats();
}

/*!
 * \brief Установка набора эффектов
 * \param effects Коллекция объектов эффектов
 */
void ActorCard::setEffects(QList<IEffect *> effects)
{
	actionEffect = effects;
}

/*!
 * \brief Получение списка эффектов
 * \return Коллекция объектов эффектов
 */
QList<IEffect *> ActorCard::getEffects()
{
	return actionEffect;
}

/*!
 * \brief Установка выборщика эффектов
 * \param selector Объект выборщика эфектов
 */
void ActorCard::setEffectSelector(IEffectSelector *selector)
{
	defaultSelector = selector;
}

/*!
 * \brief Получение выборщика эффектов
 * \return Объект выборщика эффектов
 */
IEffectSelector *ActorCard::getEffectSelector()
{
	return defaultSelector;
}

/*!
 * \brief Обновление статистики после воздействия
 */
void ActorCard::updateStats()
{
	if (health <= 0)
	{
		disposeMe();
		if (cio)
			cio->dispose(this);
	}
	else
	{
		if (cio)
			cio->notify(listItem(this->getDrawable(), this->getPseudoDrawable(), this->name, this->uid));
	}
}

/*!
 * \brief Получение изображения карты
 * \param height Высота изображения
 * \return Изображение
 */
QImage ActorCard::getDrawable(int height)
{
	QImage img = QImage(height * 0.75, height, QImage::Format_RGBA8888);
	QPainter pntr(&img);
	// тут, по идее должна быть какая-нибудь текстурка с каким-либо героем
	pntr.fillRect(0, 0, height * 0.75, height, QBrush(bg));
	pntr.fillRect(height * 0.05 * 0.75, height * 0.05, height * 0.9 * 0.75, height * 0.5, QBrush(Qt::white));
	// окошко с состоянием
	pntr.fillRect(height * 0.05 * 0.75, height * 0.6, height * 0.9 * 0.75, height * 0.35, QBrush(Qt::white));
	QFont f = pntr.font();
	f.setPixelSize(height * 0.1);
	pntr.setFont(f);
	pntr.drawText(height * 0.75 * 0.1, height * 0.7, "HP: " + QString::number(health));
	pntr.drawText(height * 0.75 * 0.1, height * 0.85, "MP: " + QString::number(energy));
	pntr.end();
	return img;
}

/*!
 * \brief Получение символьного обозначения карты
 * \return Символьное обозначение карты
 */
QString ActorCard::getPseudoDrawable()
{
	return "A(HP:" + QString::number(health) + ";MP:" + QString::number(energy) + ")";
}

/*!
 * \brief Получение информации о доступных эффектах
 * \return Информация об эффектах
 */
QString ActorCard::getFullInfo()
{
	QString result = "";
	result = name + "\nСпособности:";
	for (int i = 0; i < actionEffect.size(); i++)
		result += "\n" + actionEffect[i]->getFullInfo();
	return result;
}

/*!
 * \brief Получение информации об эффектах
 * \return Список элементов информации об эффектах
 */
QList<effectInfoItem> ActorCard::getEffectsInfo()
{
	QList<effectInfoItem> result;
	for (int i = 0; i < actionEffect.size(); i++)
	{
		result.push_back(actionEffect[i]->getInfo());
	}
	return result;
}

/*!
 * \brief Сброс основных параметров карты
 */
void ActorCard::reset()
{
	health = healthInit;
	energy = energyInit;
	selectedAction = -1;
}

/*!
 * \brief Установка объекта наблюдателя
 * \param observer Объект наблюдателя
 */
void ActorCard::attachObserver(CardInfoObserver *observer)
{
	cio = observer;
}

/*!
 * \brief Удаление наблюдателя
 * \param observer Объект наблюдателя для проверки
 */
void ActorCard::detachObserver(CardInfoObserver *observer)
{
	if (cio == observer)
		cio = NULL;
}

/*!
 * \brief Загрузка карты из потока данных
 * \param stream Объект потока данных
 */
void ActorCard::loadFromStream(QDataStream *stream)
{
	*stream >> uid;
	*stream >> name;
	*stream >> bg;
	*stream >> healthInit;
	*stream >> energyInit;
	*stream >> health;
	*stream >> energy;
	int effCount = 0;
	*stream >> effCount;
	QList<IEffect *> effs;
	for (int i = 0; i < effCount; i++)
	{
		int effType;
		*stream >> effType;
		IEffect *eff;
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
		effs.push_back(eff);
	}
	while (!actionEffect.empty())
		delete actionEffect.takeFirst();
	actionEffect = effs;
	int selectorType = 0;
	*stream >> selectorType;
	switch (selectorType)
	{
		case 0:
			defaultSelector = new EffectSelectorAll();
			break;
		case 1:
			defaultSelector = new EffectSelectorRandom();
			break;
		default:
			break;
	}
}

/*!
 * \brief Запись карты в поток данных
 * \param stream Поток данных
 */
void ActorCard::saveToStream(QDataStream *stream)
{
	int sizeInt = 0;
	*stream << 'A';
	*stream << uid;
	*stream << name;
	*stream << bg;
	*stream << healthInit;
	*stream << energyInit;
	*stream << health;
	*stream << energy;
	sizeInt = actionEffect.size();
	*stream << sizeInt;
	for (int i = 0; i < sizeInt; i++)
	{
		actionEffect[i]->saveToStream(stream);
	}
	*stream << defaultSelector->getType();
}

/*!
 * \brief Установка посетителя
 * \param visitor объект посетителя
 */
void ActorCard::acceptVisitor(CardVisitor *visitor)
{
	visitor->visitActorCard(this);
}

/*!
 * \brief Установка текущего эффекта
 * \param id Номер эффекта
 */
void ActorCard::makeActionSelection(int id)
{
	selectedAction = id;
}

/*!
 * \brief Принятие параметра карты (перенаправлен)
 * \param statName Наименование параметра
 * \param statVal Значение параметра
 * \param isInc Флаг замены/добавления
 */
void ActorCard::acceptStat(QString statName, int statVal, bool isInc)
{
	this->setStat(statName, statVal, isInc);
}

/*!
 * \brief Конструктор карты-действия
 * \param parent Родительский объект класса QObject
 */
ActionCard::ActionCard(QObject *parent):ICard(parent)
{
	cio = NULL;
	NameGen *ng = new NameGen();
	bg = QColor(Random::randInt(0, 255),
				Random::randInt(0, 255),
				Random::randInt(0, 255));
	uid = ng->randomUID();
	delete ng;
}

/*!
 * \brief Задействование карты
 * \param cardsToAffect Коллекция карт, на которые предполагается воздействие
 */
void ActionCard::act(QList<ICard *> cardsToAffect)
{
	GameSession *gs = GameSession::getInstance();
	QVector<StatsTransmitter *> transmitters;
	StatsTransmitter *srcTrans = new StatsTransmitter();
	gs->msgRepeater(this->getName() + " использует " + actionEffect->getName());
	for (int i = 0; i < cardsToAffect.size(); i++)
	{
		gs->msgRepeater(this->getName() + " воздействует на " + cardsToAffect[i]->getName());
		StatsTransmitter *st = new StatsTransmitter();
		connect(st, SIGNAL(sendStat(QString,int,bool)), cardsToAffect[i], SLOT(acceptStat(QString,int,bool)));
		transmitters.push_back(st);
	}
	connect(srcTrans, SIGNAL(sendStat(QString,int,bool)), this, SLOT(acceptStat(QString,int,bool)));
	actionEffect->act(transmitters, srcTrans);
	while (!transmitters.empty())
		delete transmitters.takeAt(0);
	delete srcTrans;
	disposeMe();
	if (cio != NULL)
		cio->dispose(this);
}

/*!
 * \brief Получение имени карты
 * \return Имя карты
 */
QString ActionCard::getName()
{
	return name;
}

/*!
 * \brief Установка имени карты
 * \param cardName Новое имя карты
 */
void ActionCard::setName(QString cardName)
{
	name = cardName;
}

/*!
 * \brief Получение уникального ID карты
 * \return Уникальный ID карты
 */
QString ActionCard::getUID()
{
	return uid;
}

/*!
 * \brief Установка нового ID карты
 * \param id Новый ID карты
 */
void ActionCard::setUID(QString id)
{
	uid = id;
}

/*!
 * \brief Получение цвета фона карты для отрисовки
 * \return Цвет фона
 */
QColor ActionCard::getBG()
{
	return bg;
}

/*!
 * \brief Установка цвета фона карты
 * \param bg Новый цвет фона
 */
void ActionCard::setBG(QColor bg)
{
	this->bg = bg;
}

/*!
 * \brief Получение определенного параметра карты
 * \param statName Наименование параметра
 * \return Численное значение параметра
 */
int ActionCard::getStat(QString statName)
{
	if (statName == "actionNum")
		return 1;
	else if (statName == "health" || statName == "energy")
		return 999;
	return 0;
}

/*!
 * \brief Установка нового значения параметра
 * \param statName Наименование параметра
 * \param statVal Значение параметра
 * \param isInc Определение того, заменяется ли значение или складывется
 */
void ActionCard::setStat(QString statName, int statVal, bool isInc)
{
	return;
}

/*!
 * \brief Установка эффектов карты
 * \param effects Коллекция объектов эффектов
 */
void ActionCard::setEffects(QList<IEffect *> effects)
{
	actionEffect = effects[0];
}

/*!
 * \brief Получение эффектов карты
 * \return Коллекция объектов эффектов
 */
QList<IEffect *> ActionCard::getEffects()
{
	return {actionEffect};
}

/*!
 * \brief Обновление параметров карты
 */
void ActionCard::updateStats()
{
	// заглушка
}

/*!
 * \brief Получение графического изображения карты
 * \param height Высота графического изображения
 * \return Графическое изображение карты
 */\
QImage ActionCard::getDrawable(int height)
{
	QImage img = QImage(height * 0.75, height, QImage::Format_RGBA8888);
	QPainter pntr(&img);
	pntr.fillRect(0, 0, height * 0.75, height, QBrush(bg));
	pntr.fillRect(height * 0.05 * 0.75, height * 0.05, height * 0.9 * 0.75, height * 0.5, QBrush(Qt::white));
	pntr.end();
	return img;
}

/*!
 * \brief Получение символьного изображения карты
 * \return Сиимвольное изображение карты
 */
QString ActionCard::getPseudoDrawable()
{
	return "E";
}

/*!
 * \brief Получение полной информации о карте
 * \return Текст с информацией
 */
QString ActionCard::getFullInfo()
{
	QString result = "";
	result = name + "\nСпособности:";
	result += "\n" + actionEffect->getFullInfo();
	return result;
}

/*!
 * \brief Получение информации об эффектах карты
 * \return Коллекция элементов информации об эффектах
 */
QList<effectInfoItem> ActionCard::getEffectsInfo()
{
	return {actionEffect->getInfo()};
}

/*!
 * \brief Сброс параметров карты (заглушка)
 */
void ActionCard::reset()
{
	// заглушка
}

/*!
 * \brief Установка информационного наблюдателя
 * \param observer Объект инфрормационного наблюдателя
 */
void ActionCard::attachObserver(CardInfoObserver *observer)
{
	cio = observer;
}

/*!
 * \brief Удаление информационного наблюдателя
 * \param observer Объект информационного наблюдателя (для проверки)
 */
void ActionCard::detachObserver(CardInfoObserver *observer)
{
	if (cio == observer)
		cio = NULL;
}

/*!
 * \brief Чтение параметров карты из потока данных
 * \param stream Поток данных
 */
void ActionCard::loadFromStream(QDataStream *stream)
{
	*stream >> uid;
	*stream >> name;
	*stream >> bg;
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
	actionEffect = eff;
}

/*!
 * \brief Запись параметров карты в поток данных
 * \param stream Поток данных
 */
void ActionCard::saveToStream(QDataStream *stream)
{
	*stream << 'E';
	*stream << uid;
	*stream << name;
	*stream << bg;
	actionEffect->saveToStream(stream);
}

/*!
 * \brief Допуск посетителя для дальнейших действий
 * \param visitor Объект посетителя
 */
void ActionCard::acceptVisitor(CardVisitor *visitor)
{
	visitor->visitActionCard(this);
}

/*!
 * \brief Выбор текущего действия. Так как этот метод относится к карте-действию,
 * он, как предполагается, остается заглушкой
 * \param id Номер действия (не нужен)
 */
void ActionCard::makeActionSelection(int id)
{
	return;
}

/*!
 * \brief Принятие свойств
 * \param statName Наименование параметра
 * \param statVal Значение параметра
 * \param isInc Определение того, заменяется ли значение или складывется
 */
void ActionCard::acceptStat(QString statName, int statVal, bool isInc)
{
	this->setStat(statName, statVal, isInc);
}

/*!
 * \brief Конструктор объекта карты-команды
 * \param parent Родительский объект класса QObject
 */
TeamCard::TeamCard(QObject *parent):ICard(parent)
{
	cio = NULL;
	NameGen *ng = new NameGen();
	bg = QColor(Random::randInt(0, 255),
				Random::randInt(0, 255),
				Random::randInt(0, 255));
	uid = ng->randomUID();
	delete ng;
}

/*!
 * \brief Задействование карты
 * \details Действие карты. При действии вызывается действие случайного члена команды,
 * энергия которого достаточна.
 * \param cardsToAffect Коллекция объектов карт, на которые предполагается
 * воздействие.
 */
void TeamCard::act(QList<ICard *> cardsToAffect)
{
	GameSession *gs = GameSession::getInstance();
	if (this->getStat("energy") == 0)
		return;
	ICard *member;
	do
	{
		member = members[Random::randInt(0, members.size() - 1)];
	} while (member->getStat("energy") <= 0);
	gs->msgRepeater(this->getName() + " выводит " + member->getName());
	member->act(cardsToAffect);
	updateStats();
}

/*!
 * \brief Получение имени карты
 * \return Имя карты
 */
QString TeamCard::getName()
{
	return name;
}

/*!
 * \brief Установка имени карты
 * \param cardName Новое имя карты
 */
void TeamCard::setName(QString cardName)
{
	name = cardName;
}

/*!
 * \brief Получение уникального ID карты
 * \return Уникальный ID карты
 */
QString TeamCard::getUID()
{
	return uid;
}

/*!
 * \brief Установка нового ID карты
 * \param id Новый ID карты
 */
void TeamCard::setUID(QString id)
{
	uid = id;
}

/*!
 * \brief Получение фонового цвета карты для отрисовки
 * \return Цвет карты
 */
QColor TeamCard::getBG()
{
	return bg;
}

/*!
 * \brief Установка фонового цвета карты для отрисовки
 * \param bg Новый цвет карты
 */
void TeamCard::setBG(QColor bg)
{
	this->bg = bg;
}

/*!
 * \brief Получение значения именованного параметра карты
 * \param statName Наименование параметра
 * \return Численно значение параметра
 */
int TeamCard::getStat(QString statName)
{
	int result = 0;
	Collection *col = new Collection(members);
	Iterator *i = new Iterator(col);
	while (i->hasNext())
	{
		result += i->getNext()->getStat(statName);
	}
	return result;
}

/*!
 * \brief Установка значения именованного параметра карты
 * \param statName Наименование параметра
 * \param statVal Численное значение параметра
 * \param isInc Способ задания, сложение (true) или замена
 */
void TeamCard::setStat(QString statName, int statVal, bool isInc)
{
	members[Random::randInt(0, members.size() - 1)]->setStat(statName, statVal, isInc);
	updateStats();
}

/*!
 * \brief Установка действий карты (заглушка). Так как команда содержит множество членов,
 * у них предполагается наличие разных действий, поэтому задание эффектов для
 * всей карты не считается целесообразным.
 * \param effects Коллекция объектов эффектов
 */\
void TeamCard::setEffects(QList<IEffect *> effects)
{
	return;
}

/*!
 * \brief Получение действий карты (заглушка).
 * \return Пустой список эффектов.
 */
QList<IEffect *> TeamCard::getEffects()
{
	return QList<IEffect *>();
}

/*!
 * \brief Обновление параметров карты
 */
void TeamCard::updateStats()
{
	if (this->getStat("health") <= 0)
	{
		disposeMe();
		if (cio)
			cio->dispose(this);
	}
	else
	{
		if (cio)
			cio->notify(listItem(this->getDrawable(), this->getPseudoDrawable(), this->name, this->uid));
	}
}

/*!
 * \brief Получение графического изображения карты
 * \param height Высота графического изображения
 * \return Графическое изображение карты
 */
QImage TeamCard::getDrawable(int height)
{
	QList<QImage> memberDrawables;
	QImage img = QImage(height * 0.75, height, QImage::Format_RGBA8888);
	QPainter pntr(&img);
	pntr.fillRect(0, 0, height * 0.75, height, QBrush(bg));
	pntr.fillRect(height * 0.05 * 0.75, height * 0.05, height * 0.9 * 0.75, height * 0.5, QBrush(Qt::white));
	Collection *col = new Collection(members);
	Iterator *it = new Iterator(col);
	int health = 0, energy = 0;
	while (it->hasNext())
	{
		ICard *member;
		member = it->getNext();
		memberDrawables.push_back(member->getDrawable(height * 0.5 * 0.75));
		health += member->getStat("health");
		energy += member->getStat("energy");
	}
	double xOffset = (height * 0.5 * 0.75) / (double)(memberDrawables.size());
	for (int i = 0; i < memberDrawables.size(); i++)
	{
		pntr.drawImage(height * 0.1 * 0.75 + xOffset * i, height * 0.1, memberDrawables[i]);
	}
	pntr.fillRect(height * 0.05 * 0.75, height * 0.6, height * 0.9 * 0.75, height * 0.35, QBrush(Qt::white));
	QFont f = pntr.font();
	f.setPixelSize(height * 0.1);
	pntr.setFont(f);
	pntr.drawText(height * 0.75 * 0.1, height * 0.7, "HP: " + QString::number(health));
	pntr.drawText(height * 0.75 * 0.1, height * 0.85, "MP: " + QString::number(energy));
	pntr.end();
	return img;
}

/*!
 * \brief Получение символьного изображения карты
 * \return Текст с символьным изображением карты
 */
QString TeamCard::getPseudoDrawable()
{
	QString result = "@{";
	Collection *col = new Collection(members);
	Iterator *it = new Iterator(col);
	int health = 0, energy = 0;
	while (it->hasNext())
	{
		ICard *member;
		member = it->getNext();
		result += member->getPseudoDrawable();
		result += ";";
		health += member->getStat("health");
		energy += member->getStat("energy");
	}
	result += "}(HP:" + QString::number(health) + ";MP:" + QString::number(energy) + ")";
	return result;
}

/*!
 * \brief Получение полной информации о карте и ее членах
 * \return Текст с информацией о карте
 */
QString TeamCard::getFullInfo()
{
	QString result;
	result = name;
	result += ":\n";
	Collection *col = new Collection(members);
	Iterator *it = col->getIterator();
	while (it->hasNext()) {
		result += "{";
		result += it->getNext()->getFullInfo();
		result += "}\n";
	}
	return result;
}

/*!
 * \brief Получение информации об эффектах карты
 * \return Список элементов информации о эффектах
 */
QList<effectInfoItem> TeamCard::getEffectsInfo()
{
	effectInfoItem result;
	result.canAffectEnemy = true;
	result.canAffectPlayer = true;
	result.effectType = 0;
	result.info = "(смешанный)";
	return {result};
}

/*!
 * \brief Установка членов команды
 * \param _members Коллекция объектов карт
 * \param beaten Устанавливать живых членов или битых (используется для загрузки)
 */
void TeamCard::setMembers(QList<ICard *> _members, bool beaten)
{
	if (!beaten)
	{
		members = _members;
		int sizeInt = members.size();
		for (int i = 0; i < sizeInt; i++)
			connect(members[i], SIGNAL(disposeMe()), this, SLOT(cardDispose()));
	}
	else
	{
		membersBeaten = _members;
	}
}

/*!
 * \brief Получение членов команды карты
 * \param beaten Получать битых или живых
 * \return Коллекция объектов карт-членов команды
 */
QList<ICard *> TeamCard::getMembers(bool beaten)
{
	return (beaten ? membersBeaten : members);
}

/*!
 * \brief Сброс параметров карты и восстановление всех членов команды
 */
void TeamCard::reset()
{
	while (!membersBeaten.empty())
	{
		ICard *member = membersBeaten.takeAt(0);
		member->reset();
		members.push_back(member);
	}
}

/*!
 * \brief Установка информацонного наблюдателя
 * \param observer Объект информационного наблюдателя
 */
void TeamCard::attachObserver(CardInfoObserver *observer)
{
	cio = observer;
}

/*!
 * \brief Удаление информационного наблюдателя
 * \param observer Объект информационного наблюдателя (для проверки)
 */
void TeamCard::detachObserver(CardInfoObserver *observer)
{
	if (cio == observer)
		cio = NULL;
}

/*!
 * \brief Загрузка параметров карты из потока данных
 * \param stream Поток даннных
 */
void TeamCard::loadFromStream(QDataStream *stream)
{
	int sizeInt = 0;
	*stream >> uid;
	*stream >> name;
	*stream >> bg;
	QList<ICard *> storedMembers, storedMembersBeaten;
	*stream >> sizeInt;
	for (int i = 0; i < sizeInt; i++)
	{
		ICard *card;
		int cardType;
		*stream >> cardType;
		switch (cardType)
		{
			case 'A':
				card = new ActorCard();
				break;
			case 'E':
				card = new ActionCard();
				break;
			case 'T':
				card = new TeamCard();
				break;
		}
		card->loadFromStream(stream);
		connect(card, SIGNAL(disposeMe()), this, SLOT(cardDispose()));
		storedMembers.push_back(card);
	}
	*stream >> sizeInt;
	for (int i = 0; i < sizeInt; i++)
	{
		ICard *card;
		int cardType;
		*stream >> cardType;
		switch (cardType)
		{
			case 'A':
				card = new ActorCard();
				break;
			case 'E':
				card = new ActionCard();
				break;
			case 'T':
				card = new TeamCard();
				break;
		}
		card->loadFromStream(stream);
		connect(card, SIGNAL(disposeMe()), this, SLOT(cardDispose()));
		storedMembersBeaten.push_back(card);
	}
	while (!members.empty())
		delete members.takeFirst();
	while (!membersBeaten.empty())
		delete membersBeaten.takeFirst();
	members = storedMembers;
	membersBeaten = storedMembersBeaten;
}

/*!
 * \brief Запись параметров карты в поток данных
 * \param stream Поток данных
 */
void TeamCard::saveToStream(QDataStream *stream)
{
	int sizeInt = 0;
	*stream << 'T';
	*stream << uid;
	*stream << name;
	*stream << bg;
	sizeInt = members.size();
	*stream << sizeInt;
	for (int i = 0; i < sizeInt; i++)
		members[i]->saveToStream(stream);
	sizeInt = membersBeaten.size();
	*stream << sizeInt;
	for (int i = 0; i < sizeInt; i++)
		membersBeaten[i]->saveToStream(stream);
}

/*!
 * \brief Допуск посетителя к дальнейшим действиям
 * \param visitor Объект посетителя
 */
void TeamCard::acceptVisitor(CardVisitor *visitor)
{
	visitor->visitTeamCard(this);
}

/*!
 * \brief Выбор дальнейшего действия карты (заглушка)
 * \param id Номер действия
 */
void TeamCard::makeActionSelection(int id)
{
	return;
}

/*!
 * \brief Принятие параметров карты
 * \param statName Наименование параметра
 * \param statVal Значение параметра
 * \param isInc Определение того, заменяется ли значение или складывется
 */
void TeamCard::acceptStat(QString statName, int statVal, bool isInc)
{
	this->setStat(statName, statVal, isInc);
}

/*!
 * \brief Выброс карты-члена команды в битые и запрос на обновление параметров
 */
void TeamCard::cardDispose()
{
	QObject *o = QObject::sender();
	ICard *card = (ICard *)o;
	int cardID = members.indexOf(card);
	if (cardID != -1)
	{
		membersBeaten.push_back(members.takeAt(cardID));
	}
	updateStats();
}

/*!
 * \brief Конструктор объекта интерфейса информационного наблюдателя
 * \param parent Родительский объект класса QObject
 */
CardInfoObserver::CardInfoObserver(QObject *parent) : QObject(parent)
{

}

/*!
 * \brief оповещение об изменении параметров карты
 * \param litem Элемент списка карт
 */
void CardInfoObserver::notify(listItem litem)
{
	GameSession::getInstance()->listItemUpdateRepeater(litem);
}

/*!
 * \brief Запрос на перенос карты в битые
 * \param me Объект карты
 */
void CardInfoObserver::dispose(ICard *me)
{
	GameSession::getInstance()->disposeCard(me);
}

/*!
 * \brief Получение элемента списка карт
 * \param card Карта
 * \return Элемент списка карт
 */
listItem getCardListItem(ICard *card)
{
	return listItem(card->getDrawable(), card->getPseudoDrawable(), card->getName(), card->getUID());
}

/*!
 * \brief Конструкор объекта записи карт в поток данных
 */
CardStreamWriter::CardStreamWriter() : CardVisitor()
{

}

/*!
 * \brief Установка потока данных для записи в него
 * \param stream Поток данных
 */
void CardStreamWriter::setupStream(QDataStream *stream)
{
	this->stream = stream;
}

/*!
 * \brief Запись карты в поток данных
 * \param card Карта для записи
 */
void CardStreamWriter::writeCard(ICard *card)
{
	card->acceptVisitor(this);
}

/*!
 * \brief Запись карты-персонажа в поток данных
 * \param card Карта-персонаж для записи
 */
void CardStreamWriter::visitActorCard(ActorCard *card)
{
	int sizeInt = 0;
	*stream << 'A';
	*stream << card->getUID();
	*stream << card->getName();
	*stream << card->getBG();
	*stream << card->getStat("healthInit");
	*stream << card->getStat("energyInit");
	*stream << card->getStat("health");
	*stream << card->getStat("energy");
	QList<IEffect *> effs = card->getEffects();
	sizeInt = effs.size();
	*stream << sizeInt;
	for (int i = 0; i < sizeInt; i++)
	{
		effs[i]->saveToStream(stream);
	}
	*stream << card->getEffectSelector()->getType();
}

/*!
 * \brief Запись карты-действия в поток данных
 * \param card Карта-действие для записи
 */
void CardStreamWriter::visitActionCard(ActionCard *card)
{
	*stream << 'E';
	*stream << card->getUID();
	*stream << card->getName();
	*stream << card->getBG();
	card->getEffects().at(0)->saveToStream(stream);
}

/*!
 * \brief Запись карты-команды в поток данных
 * \param card Карта-команда для записи
 */
void CardStreamWriter::visitTeamCard(TeamCard *card)
{
	int sizeInt = 0;
	*stream << 'T';
	*stream << card->getUID();
	*stream << card->getName();
	*stream << card->getBG();
	QList<ICard *> members = card->getMembers(), membersBeaten = card->getMembers(true);
	sizeInt = members.size();
	*stream << sizeInt;
	while (!members.empty())
		writeCard(members.takeFirst());
	sizeInt = membersBeaten.size();
	*stream << sizeInt;
	while (!membersBeaten.empty())
		writeCard(membersBeaten.takeFirst());
}

/*!
 * \brief \param cardType Тип карты (A - персонаж, E - действие, T - команда)
 * \return Объект карты соответствующего типа
 */
ICard *matchCardPointer(char cardType)
{
	ICard *card;
	switch (cardType)
	{
		case 'A':
			card = new ActorCard();
			break;
		case 'E':
			card = new ActionCard();
			break;
		case 'T':
			card = new TeamCard();
			break;
		default:
			card = nullptr;
			break;
	}
	return card;
}

/*!
 * \brief Конструктор объекта интерфейса посетителя
 */
CardVisitor::CardVisitor()
{

}

/*!
 * \brief Конструктор объекта чтения карты из потока данных
 */
CardStreamReader::CardStreamReader() : CardVisitor()
{

}

/*!
 * \brief Установка потока данных для чттения карты из него
 * \param stream Поток данных
 */
void CardStreamReader::setupStream(QDataStream *stream)
{
	this->stream = stream;
}

/*!
 * \brief Чтение карты в поток данных
 * \param card Объект карты для чтения данных в него
 */
void CardStreamReader::readCard(ICard *card)
{
	card->acceptVisitor(this);
}

/*!
 * \brief Чтение параметров карты-персонажа из потока данных
 * \param card Объект карты-персонажа для чтения в него
 */
void CardStreamReader::visitActorCard(ActorCard *card)
{
	int bufInt;
	QString bufString;
	QColor bufCol;
	*stream >> bufString;
	card->setUID(bufString);
	*stream >> bufString;
	card->setName(bufString);
	*stream >> bufCol;
	card->setBG(bufCol);
	*stream >> bufInt;
	card->setStat("healthInit", bufInt);
	*stream >> bufInt;
	card->setStat("energyInit", bufInt);
	*stream >> bufInt;
	card->setStat("health", bufInt);
	*stream >> bufInt;
	card->setStat("energy", bufInt);
	int effCount = 0;
	*stream >> effCount;
	QList<IEffect *> effs;
	for (int i = 0; i < effCount; i++)
	{
		int effType;
		*stream >> effType;
		IEffect *eff;
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
		effs.push_back(eff);
	}
	card->setEffects(effs);
	IEffectSelector *effSel = nullptr;
	int selectorType = 0;
	*stream >> selectorType;
	switch (selectorType)
	{
		case 0:
			effSel = new EffectSelectorAll();
			break;
		case 1:
			effSel = new EffectSelectorRandom();
			break;
		default:
			break;
	}
	card->setEffectSelector(effSel);
}

/*!
 * \brief Чтение параметров карты-действия из потока данных
 * \param card Объект карты-действия для чтения в него
 */
void CardStreamReader::visitActionCard(ActionCard *card)
{
	QString bufString;
	QColor bufCol;
	*stream >> bufString;
	card->setUID(bufString);
	*stream >> bufString;
	card->setName(bufString);
	*stream >> bufCol;
	card->setBG(bufCol);
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
	card->setEffects({eff});
}

/*!
 * \brief Чтение параметров карты-команды из потока данных
 * \param card Объект карты-команды для чтения в него
 */
void CardStreamReader::visitTeamCard(TeamCard *card)
{
	int sizeInt = 0;
	QString bufString;
	QColor bufCol;
	*stream >> bufString;
	card->setUID(bufString);
	*stream >> bufString;
	card->setName(bufString);
	*stream >> bufCol;
	card->setBG(bufCol);
	QList<ICard *> storedMembers, storedMembersBeaten;
	*stream >> sizeInt;
	for (int i = 0; i < sizeInt; i++)
	{
		int cardType;
		*stream >> cardType;
		ICard *member = matchCardPointer(cardType);
		readCard(member);
		storedMembers.push_back(member);
	}
	*stream >> sizeInt;
	for (int i = 0; i < sizeInt; i++)
	{
		int cardType;
		*stream >> cardType;
		ICard *member = matchCardPointer(cardType);
		readCard(member);
		QObject::connect(member, SIGNAL(disposeMe()), card, SLOT(cardDispose()));
		storedMembersBeaten.push_back(member);
	}
	card->setMembers(storedMembers);
	card->setMembers(storedMembersBeaten, true);
}
