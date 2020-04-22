#pragma once
#include <SFML/Graphics.hpp>

class Tile
{
public:
	Tile(sf::RenderWindow& t_window);
	~Tile();

	void render();
	void setUp(sf::Vector2f t_size);
	void setPosition(sf::Vector2f t_position,sf::Font& t_font);
	void setID(int t_ID);
	void setValue(int t_value);
	void setGoalDistance(int t_distance);

	void setColor(sf::Color t_color);

	void goalTileUpdate(bool t_state);
	void starttTileUpdate(bool t_state);
	void wallTileUpdate(bool t_state);


	bool getGoalState();
	bool getStartState();
	bool getWallState();


	sf::Vector2f getTilePosition();
	sf::Vector2f getTileSize();
	int getID();
	int getValue();
	int getGoalDistance();
	



private:
	sf::RenderWindow& m_window;
	sf::RectangleShape m_tile;
	
	sf::Text m_valueText; 


	int m_ID;
	int m_value = NULL;
	int m_goalDistance = NULL;
	bool m_goal = false;
	bool m_start = false;
	bool m_wall = false;

	
};

