#include "Tile.h"

Tile::Tile(sf::RenderWindow& t_window) :
m_window(t_window)
{
}

Tile::~Tile()
{
}

void Tile::render()
{
	m_window.draw(m_tile);
	m_window.draw(m_valueText);
}

void Tile::setUp(sf::Vector2f t_size)
{
	m_tile.setFillColor(sf::Color(0, 0, 0, 0));
	m_tile.setOutlineColor(sf::Color::White);
	m_tile.setOutlineThickness(1);
	m_tile.setSize(t_size);
}

void Tile::setPosition(sf::Vector2f t_position, sf::Font& t_font)
{
	m_tile.setPosition(t_position);
	m_valueText.setFont(t_font);
	m_valueText.setCharacterSize(10);
	m_valueText.setPosition(m_tile.getPosition());
}

void Tile::setID(int t_ID){m_ID = t_ID;}
void Tile::setValue(int t_value)
{
	m_value = t_value;
	m_valueText.setString(std::to_string(m_value));
}

void Tile::setGoalDistance(int t_distance)
{
	m_goalDistance = t_distance;
}

void Tile::goalTileUpdate(bool t_state) {m_goal = t_state;}
void Tile::starttTileUpdate(bool t_state) {m_start = t_state;}
void Tile::wallTileUpdate(bool t_state) {m_wall = t_state;}

bool Tile::getGoalState() {return m_goal;}
bool Tile::getStartState() {return m_start;}
bool Tile::getWallState() {return m_wall;}

void Tile::setColor(sf::Color t_color) {m_tile.setFillColor(t_color);}



sf::Vector2f Tile::getTilePosition()
{
	return m_tile.getPosition();
}
sf::Vector2f Tile::getTileSize()
{
	return m_tile.getSize();
}
int Tile::getID()
{
	return m_ID;
}

int Tile::getValue()
{
	return m_value;
}

int Tile::getGoalDistance()
{
	return m_goalDistance;
}
