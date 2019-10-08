#pragma once
#include "GameManager.h"
#include "Zone.h"

class Zone;


class Entity
{

public: //setters and getters
	std::string getName() { return a_Name; }
	float getLevel() { return m_level; }
	int getHealth() { return m_health; }
	int getMaxHealth() { return m_maxHealth; }
	int getEndurance() { return m_endurance; }
	int getMaxEndurance() { return m_maxEndurance; }
	int getZpos() { return m_zPos; }
	float getPosX() { return m_xPos; }
	float getPosY() { return m_yPos; }
	char getSymbol() { return m_symbol; }
	int getMoney() { return m_money; }
	int getHpotHealAmount() { return m_hPotHealAmount; }
	int getEpotHealAmount() { return m_ePotRecoverAmount; }

	float getDmg() { return m_damage; }

	void setLevel(float amount) { m_level += amount; }
	void setHealth(float amount) { m_health += amount; }
	void increaseMaxHealth()	{ m_maxHealth += (int)(m_maxHealth * 0.1) * (int)m_level; }
	void setEndurance(int amount) { m_endurance += amount; }
	void increaseMaxEndurance() { m_maxEndurance += (int)(m_baseEndurance * .30) * (int)m_level; }
	void setZpos(float move) { m_zPos += move; }
	void setPosX(float moveX) { m_xPos += moveX; }
	void setPosY(float moveY) { m_yPos += moveY; }
	void setMoney(int amount) { m_money += amount; }
	void setHpotHealAmount() { m_hPotHealAmount = m_maxHealth; }
	void setEpotHealAmount() { m_ePotRecoverAmount = m_maxEndurance; }

public:
	Entity(SDL_Renderer& renderer, std::string& spriteSheet, char& symbol, std::string& name, float& level, float& health, int& endurance, int& money, float& yPos, float& xPos, int& zPos);
	virtual ~Entity();


	//bool move(Player *player, Zone *zone, GameManager *GM);

protected:


	enum weaponRange {Melee = 1, ShortRange = 3, LongRange = 5};

	float m_damage = 1;
	char m_symbol;
	float m_level;
	float m_health;
	int m_endurance;
	float m_maxHealth;
	int m_maxEndurance;
	float m_yPos;
	float m_xPos;
	int m_zPos;
	int m_money;
	int m_healthPotions = 0;
	int m_endurPotions = 0;
	int m_teleportSpells = 0;
	int m_hPotHealAmount;
	int m_ePotRecoverAmount;
	int m_baseHealth = 18;
	int m_baseEndurance = 15;
	std::string a_Name;


	
	int m_giveBonusHeal;


private://variables
public://variables
	const float giveBaseXP = .03f;
	int getBonusHeal() { return m_giveBonusHeal; };
	static const int mk_potsAllowed = 3;
	
private:
	
};

