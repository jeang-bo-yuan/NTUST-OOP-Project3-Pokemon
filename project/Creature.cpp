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
}


// 回傳type 傷害
void Creature::useSkill(int index, Creature& target, int turn, bool humanAttack)
{
	if (this->paralyzed) {
		cout << "[Turn " << turn << " ] ";
		if (!this->isHuman) {
			cout << "The opposing ";
		}
		cout << name << " is paralyzed! It can't move!" << endl;
		return;
	}
	Skill& nowSkill = skills[index];
	int damage = 0;
	double atk;
	double def;
	double stabDamange;
	double typeDamange;

	if (nowSkill.PP <= 0) {
		
		cout << "PP不足" << endl;
		return;
	}

	cout << "[Turn " << turn << " ] ";
	if (!this->isHuman) {
		cout << "The opposing ";
	}
	cout << name << " used " << nowSkill.name << "!" << endl;

	// 技能有特殊效果
	if (nowSkill.effect != EFFECT_NAME::NONE) {
		EffectManager::addEffect(nowSkill.effect, &target, turn, humanAttack);
	}

	string typeName;
	nowSkill.PP--;


	if (nowSkill.skillType == PHYSICAL) {
		atk = this->atk;
		def = target.def;
	}
	else {
		atk = this->spAtk;
		def = target.spDef;
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
		cout << "[Turn " <<  turn << " ] " << "it's super effective!" << endl;
	}
	else if (typeDamange <= 0.5) {
		cout << "[Turn " << turn << " ] " << "it's not very effective..." << endl;
	}
	else if (typeDamange == 0) {
		cout << "[Turn " << turn << " ] " << "it doesn't affect..." << endl;
	}

	if (false) { // if cit
		cout << "[Turn " << turn << " ] " << "it's a critical hit!" << endl;
	}

	damage = int((double(2) * level + double(10)) / double(250) * nowSkill.power * atk / def + 2) * 1 * stabDamange * typeDamange;

	target.beRealDamange(damage);
}

void Creature::useSkill(string skillName, Creature& target, int turn, bool humanAttack)
{
	int index = 0;
    for (const auto& i : skills) {
		if (i.name == skillName) {
			break;
		}
		index++;
	}
	useSkill(index, target, turn, humanAttack);
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
	return *this;
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
