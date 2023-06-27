#include "Creature.h"
#include "Common.h"
#include "SkillLibary.h"
#include "EffectManager.h"

using namespace std;

double getTypeRate(TYPE attack, TYPE defend)
{
    static const float effectTable[18][18] =
	{
		{1,1,1,1,1,1,1,1,1,1,1,1,0.5,0,1,1,0.5,1},
		{1,0.5,0.5,1,2,2,1,1,1,1,1,2,0.5,1,0.5,1,2,1},
		{1,2,0.5,1,0.5,1,1,1,2,1,1,1,2,1,0.5,1,1,1},
		{1,1,2,0.5,0.5,1,1,1,0,2,1,1,1,1,0.5,1,1,1},
		{1,0.5,2,1,0.5,1,1,0.5,2,0.5,1,0.5,2,1,0.5,1,0.5,1},
		{1,0.5,0.5,1,2,0.5,1,1,2,2,1,1,1,1,2,1,0.5,1},
		{2,1,1,1,1,2,1,0.5,1,0.5,0.5,0.5,2,0,1,2,2,0.5},
		{1,1,1,1,2,1,1,0.5,0.5,1,1,1,0.5,0.5,1,1,0,2},
		{1,2,1,2,0.5,1,1,2,1,0,1,0.5,2,1,1,1,2,1},
		{1,1,1,0.5,2,1,2,1,1,1,1,2,0.5,1,1,1,0.5,1},
		{1,1,1,1,1,1,2,2,1,1,0.5,1,1,1,1,0,0.5,1},
		{1,0.5,1,1,2,1,0.5,0.5,1,0.5,2,1,1,0.5,1,2,0.5,0.5},
		{1,2,1,1,1,2,0.5,1,0.5,2,1,2,1,1,1,1,0.5,1},
		{0,1,1,1,1,1,1,1,1,1,2,1,1,2,1,0.5,1,1},
		{1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,0.5,1,0},
		{1,1,1,1,1,1,0.5,1,1,1,2,1,1,2,1,0.5,0.5,0.5},
		{1,0.5,0.5,0.5,1,2,1,1,1,1,1,1,2,1,1,1,0.5,2}
	};
	return effectTable[static_cast<int>(attack)][static_cast<int>(defend)];
}

Creature::Creature()
{
	paralyzed = false;
	isHuman = true;
	isFainting = false;
}


Creature::Creature(const Creature& creature)
{
	this->name = creature.name;
	this->maxHp = creature.maxHp;
	this->hp = creature.hp;
	this->atk = creature.atk;
	this->def = creature.def;
	this->spAtk = creature.spAtk;
	this->spDef = creature.spDef;
	this->speed = creature.speed;
	this->types = creature.types;
	this->skills = creature.skills;
	this->paralyzed = creature.paralyzed;
	this->isHuman = creature.isHuman;
	this->isFainting = creature.isFainting;
}


// 回傳type 傷害
void Creature::useSkill(int index, Creature& target, int turn, bool humanAttack,bool isTesting)
{
	if (this->paralyzed) {
		cout << "[Turn " << turn << "] ";
		cout << getPrintName() << " is paralyzed! " << endl;
		cout << "[Turn " << turn << "] ";
		cout << "It can't move!" << endl;
		return;
	}
	Skill& nowSkill = skills[index];
	int damage = 0;
	double atk;
	double def;
	double stabDamange;
	double typeDamange;
    double criticalDamage = 1;

	if (nowSkill.PP <= 0) {
		
		cout << "PP不足" << endl;
		return;
	}

	cout << "[Turn " << turn << "] ";
	if (!this->isHuman) {
		cout << "The opposing ";
	}
	cout << name << " used " << nowSkill.name << "!" << endl;


//	string typeName;
	nowSkill.PP--;

    if(!isTesting)
    {
        srand(time(0));

        int r = rand()%100;

        if(r>nowSkill.accuracy)
        {
            cout<< "[Turn " << turn << "] "<<target.getName()<<" avoided the attack!" << endl;
            return;
        }
    }

	if (nowSkill.skillType == PHYSICAL) {
		atk = this->atk;
		def = target.def;
	}
	else if (nowSkill.skillType == SPECIAL) {
		atk = this->spAtk;
		def = target.spDef;
	}
	else if (nowSkill.skillType == STATUS) {
		if (nowSkill.effect != EFFECT_NAME::NONE) {
			EffectManager::addEffect(nowSkill.effect, &target, turn, humanAttack);
		}
		return;
	}
	else {
		cout << "技能類型錯誤" << endl;
		return;
	}
	

	if (this->isSameType(nowSkill.type)) {
		stabDamange = 1.5;
	}
	else {
		stabDamange = 1;
	}

	if (target.getTypeSize() == 2) {
		typeDamange = getTypeRate(nowSkill.type, target.types[0]) * getTypeRate(nowSkill.type, target.types[1]);
	}
	else {
		typeDamange = getTypeRate(nowSkill.type, target.types[0]);
	}

	if (typeDamange >= 2) {
		cout << "[Turn " <<  turn << "] " << "It's super effective!" << endl;
    }
    else if (typeDamange == 0) {
        cout << "[Turn " << turn << "] " << "It doesn't affect..." << endl;
    }
	else if (typeDamange <= 0.5) {
        cout << "[Turn " << turn << "] " << "It's not very effective..." << endl;
    }

    if (!isTesting) { // if cit
        int r = rand()%100;

        if(r<=30)
        {
            criticalDamage = 1.5;
            cout << "[Turn " << turn << "] " << "A critical hit!" << endl;
        }
	}

	// 技能有特殊效果
	if (nowSkill.effect != EFFECT_NAME::NONE) {
		EffectManager::addEffect(nowSkill.effect, &target, turn, humanAttack);
	}

    damage = int((double(2) * level + double(10)) / double(250) * nowSkill.power * atk / def + double(2)) * criticalDamage * stabDamange * typeDamange;
	target.beRealDamange(damage);
}

void Creature::useSkill(string skillName, Creature& target, int turn, bool humanAttack, bool isTesting)
{
	int index = 0;

    for (const auto& i : skills) {
		if (i.name == skillName) {
			break;
		}
		index++;
	}

	if (index == skills.size()) {
		cout << "技能名稱錯誤: " << skillName << endl;
		for (int i = 0; i < target.getSkillSize(); i++) {
			cout << target.skills[i].name << endl;
		}
		return;
	}
	else {
        useSkill(index, target, turn, humanAttack,isTesting);
	}
}

bool Creature::isSameType(TYPE type)
{
	for (auto i : types) {
		if (i == type) {
			return true;
		}
	}

	return false;
}


istream& operator>>(istream& is, Creature& creature)
{
	string name;
	int typeNum;
	int hp, atk, def, spAtk, spDef, speed;

	is >> name;

	creature.name = name;

	is >> typeNum;
	for (int i = 0; i < typeNum; i++) {
		string type;

		is >> type;
		creature.types.push_back(strToType(type));
	}

	is >> hp >> atk >> def >> spAtk >> spDef >> speed;
	creature.hp = hp;
	creature.maxHp = creature.hp = hp;
	creature.atk = atk;
	creature.def = def;
	creature.spAtk = spAtk;
	creature.spDef = spDef;
	creature.speed = speed;
	return is;
}

// Intent: 得到真傷
// Pre: value 非負
// Post: creature被造成value真傷
void Creature::beRealDamange(int value)
{
	hp -= value;

	if (hp < 0) {
		hp = 0;
	}
}

double Creature::getDodgeRate()
{
	return dodgeRate;
}


Creature& Creature::operator=(const Creature& creature)
{
	if (&creature == this) {
		return *this;
	}

	name = creature.name;
	types = creature.types;
	hp = creature.hp;
	maxHp = creature.maxHp;
	atk = creature.atk;
	def = creature.def;
	spAtk = creature.spAtk;
	spDef = creature.spDef;
	speed = creature.speed;
	dodgeRate = creature.dodgeRate;
	skills = creature.skills;
	paralyzed = creature.paralyzed;
	isFainting = creature.isFainting;
	return *this;
}

const string Creature::getPrintName() const
{
	if (isHuman) {
		return name;
	}
	else {
		return string("The opposing ") + name;
	}
}

int Creature::getHp() const
{
	return hp;
}

int Creature::getMaxHp() const
{
	return maxHp;
}

int Creature::getSpeed() const
{
	return speed;
}

int Creature::getAtk() const { return this->atk; }

int Creature::getDef() const
{
    return def;
}

int Creature::getSpAtk() const
{
	return spAtk;
}

int Creature::getSpDef() const
{
	return spDef;
}

bool Creature::isParalyzed() const
{
	return paralyzed;
}

bool Creature::isFaint() const
{
	return isFainting;
}

double Creature::getDodgeRate() const
{
	return dodgeRate;
}


int Creature::getSkillSize() const
{
	return skills.size();
}

string Creature::getSkillName(int index) const
{
	return skills[index].name;
}

Skill Creature::getSkill(int index) const
{
	if (index >= skills.size() && index < 0) {
		cout << "技能索引錯誤: " << index << endl;
		return Skill();
	}
	return skills[index];
}

int Creature::getTypeSize() const
{
	return types.size();
}

const string Creature::getTypeName(int index) const
{
	return typeToStr(types[index]);
}

void Creature::addSkill(Skill skill)
{
	skills.push_back(skill);
}

const string& Creature::getName() const
{
	return name;
}

void Creature::heal(int volume)
{
	hp = min(hp + volume, maxHp);
}

void Creature::decreaseSpeed()
{
	speed = speed / 2;
}

void Creature::setParalyzed(bool can)
{
	paralyzed = can;
}

void Creature::setFaint(bool can)
{
	isFainting = can;
}
