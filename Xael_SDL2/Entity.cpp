#include "Entity.h"






Entity::Entity(SDL_Renderer& renderer, std::string& spriteSheet, char& symbol, std::string &name, float& level, float& health, int& endurance, int& money, float& yPos, float& xPos, int& zPos)
	: m_symbol(symbol), a_Name(name), m_level(level), m_maxHealth(health), m_maxEndurance(endurance), m_health(health)
	, m_endurance(endurance), m_money(money), m_yPos(yPos), m_xPos(xPos), m_zPos(zPos), m_hPotHealAmount(health), m_ePotRecoverAmount(endurance)
{
	
}

Entity::~Entity()
{
}

