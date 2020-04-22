
#include "Game.h"
#include <iostream>


Game::Game() :
	m_window{ sf::VideoMode{ 2000, 1800U, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	setUp();
	generateGrid();
}

Game::~Game()
{
}

void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (sf::Event::MouseButtonPressed == newEvent.type)
		{
			processClicks(newEvent);
		}
	}
}

void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
	if (sf::Keyboard::Space == t_event.key.code)
	{
		runValueSet();
	}
}

void Game::processClicks(sf::Event t_event)
{
	if (sf::Mouse::Left == t_event.key.code)
	{
		setGoalTile();
	}
	if (sf::Mouse::Right == t_event.key.code)
	{
		setStartTile();
	}
	if (sf::Mouse::Middle == t_event.key.code)
	{
		setWallTile();
	}
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
}

void Game::render()
{
	m_window.clear(sf::Color::Blue);
	for (int i = 0; i < m_totalTiles; i++)
	{
		m_tiles[i]->render();
	}
	m_window.display();
}

void Game::setUp()
{
	if (!m_font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading font" << std::endl;
	}

}

void Game::generateGrid()
{

	for (int i = 0; i < (m_totalTiles); i++)
	{
		m_tiles.push_back(new Tile(m_window));
		m_tiles[i]->setID(i);
		m_tiles[i]->setUp(sf::Vector2f(m_window.getSize().x / m_numTilesHigh, m_window.getSize().y / m_numTilesWide));
		
	}

	float x = 0;
	float y = 0;
	float xcount = 0;
	float ycount = 0;
	for (int i = 0; i < (m_totalTiles); i++)
	{
		m_tiles[i]->setPosition(sf::Vector2f(x, y),m_font);

		if (x < (m_window.getSize().x)- m_tiles[i]->getTileSize().x)
		{
			x += m_tiles[i]->getTileSize().x;
		}
		else
		{
			xcount++;
			ycount++;
			x = 0;
			y = (m_tiles[i]->getTileSize().y * ycount);
		}
	}
}

void Game::setGoalTile()
{
	sf::Vector2f m_click = sf::Vector2f(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);

	for (int i = 0; i < (m_totalTiles); i++)
	{
		if (m_tiles[i]->getGoalState() == true)
		{
			m_tiles[i]->goalTileUpdate(false);
			m_tiles[i]->setColor(sf::Color::Blue);
			m_tiles[i]->setValue(NULL);
		}
	}
	int clickX = m_click.x / m_tiles[0]->getTileSize().x;
	int clickY = m_click.y / m_tiles[0]->getTileSize().y;

	std::cout << "goal click"<< "[" << m_click.x << ", " << m_click.y << "]" << std::endl;
	std::cout << "goal tile" << "[" << clickX << ", " << clickY << "]" << std::endl;
 
	m_tiles[clickX+(clickY*50)]->setColor(sf::Color::Green);
	m_tiles[clickX + (clickY * 50)]->goalTileUpdate(true);
	m_tiles[clickX + (clickY * 50)]->wallTileUpdate(false);
	m_tiles[clickX + (clickY * 50)]->starttTileUpdate(false);
	m_tiles[clickX + (clickY * 50)]->setValue(0);
	m_goalTileID = (clickX + (clickY * 50));
	m_goalRow = clickX;
	m_goalCol = clickY;

}
void Game::setStartTile()
{
	sf::Vector2f m_click = sf::Vector2f(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);

	for (int i = 0; i < (m_totalTiles); i++)
	{
		if (m_tiles[i]->getStartState() == true)
		{
			m_tiles[i]->starttTileUpdate(false);
			m_tiles[i]->setColor(sf::Color::Blue);
			m_tiles[i]->setValue(m_tiles[i]->getID());
		}
	}

	int clickX = m_click.x / m_tiles[0]->getTileSize().x;
	int clickY = m_click.y / m_tiles[0]->getTileSize().y;

	std::cout << "start click" << "[" << m_click.x << ", " << m_click.y << "]" << std::endl;
	std::cout << "start tile" << "[" << clickX << ", " << clickY << "]" << std::endl;

	m_tiles[clickX + (clickY * 50)]->setColor(sf::Color::Red);
	m_tiles[clickX + (clickY * 50)]->starttTileUpdate(true);
	m_tiles[clickX + (clickY * 50)]->wallTileUpdate(false);
	m_tiles[clickX + (clickY * 50)]->goalTileUpdate(false);
	m_tiles[clickX + (clickY * 50)]->setValue(999);
	m_startTileID = (clickX + (clickY * 50));
	m_startRow = clickX;
	m_startCol = clickY;
}
void Game::setWallTile()
{
	sf::Vector2f m_click = sf::Vector2f(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);

	int clickX = m_click.x / m_tiles[0]->getTileSize().x;
	int clickY = m_click.y / m_tiles[0]->getTileSize().y;

	if (m_tiles[clickX + (clickY * 50)]->getWallState() == true)
	{
		m_tiles[clickX + (clickY * 50)]->wallTileUpdate(false);
		m_tiles[clickX + (clickY * 50)]->setColor(sf::Color::Blue);
		m_tiles[clickX + (clickY * 50)]->setValue(NULL);
	}
	else
	{
		std::cout << "wall click" << "[" << m_click.x << ", " << m_click.y << "]" << std::endl;
		std::cout << "wall tile" << "[" << clickX << ", " << clickY << "]" << std::endl;

		m_tiles[clickX + (clickY * 50)]->setColor(sf::Color::Black);
		m_tiles[clickX + (clickY * 50)]->wallTileUpdate(true);
		m_tiles[clickX + (clickY * 50)]->starttTileUpdate(false);
		m_tiles[clickX + (clickY * 50)]->goalTileUpdate(false);
		m_tiles[clickX + (clickY * 50)]->setValue(999);
	}
}

void Game::runValueSet()
{
	//loop to reset tiles to null
	for (int i = 0; i < m_totalTiles; i++)
	{
		if (m_tiles[i]->getWallState() == false)
		{
			m_tiles[i]->setValue(NULL);
		}		
	}


	int m_goalID = m_goalRow + (m_goalCol * 50);
	sf::Vector2f m_goalPosition = m_tiles[m_goalID]->getTilePosition();
	sf::Vector2f m_tileSize = m_tiles[0]->getTileSize();

	std::vector<Tile*> m_neighbours;

	//initial loop to get neighbours of the goal
	for (int i = 0; i < (m_totalTiles); i++)
	{
		sf::Vector2f m_tilePosition = m_tiles[i]->getTilePosition();

	
		if (m_tiles[i]->getWallState() == false)
		{
			//Left tile
			if (m_tilePosition.x == (m_goalPosition.x - m_tileSize.x) && m_tilePosition.y == m_goalPosition.y)
			{
				m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
			}
			//Top left
			else if (m_tilePosition.x == (m_goalPosition.x - m_tileSize.x) && m_tilePosition.y == m_goalPosition.y - m_tileSize.y)
			{
				m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
			}
			//Top right
			else if (m_tilePosition.x == (m_goalPosition.x + m_tileSize.x) && m_tilePosition.y == m_goalPosition.y - m_tileSize.y)
			{
				m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
			}
			//Right
			else if (m_tilePosition.x == (m_goalPosition.x + m_tileSize.x) && m_tilePosition.y == m_goalPosition.y)
			{
				m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
			}
			//Bottom right
			else if (m_tilePosition.x == (m_goalPosition.x + m_tileSize.x) && m_tilePosition.y == m_goalPosition.y + m_tileSize.y)
			{
				m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
			}
			//Bottom left
			else if (m_tilePosition.x == (m_goalPosition.x - m_tileSize.x) && m_tilePosition.y == m_goalPosition.y + m_tileSize.y)
			{
				m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
			}
			//UP
			if (m_tilePosition.x == (m_goalPosition.x) && m_tilePosition.y == m_goalPosition.y + m_tileSize.y)
			{
				m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
			}
			//Down
			if (m_tilePosition.x == (m_goalPosition.x) && m_tilePosition.y == m_goalPosition.y - m_tileSize.y)
			{
				m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
			}
		}

	}

	//while loop to loop through all neighbours and there neigbours expanding
	while (m_neighbours.size() != 0)
		{

			int n = m_neighbours.size();
			sf::Vector2f m_neighbourPosition = m_neighbours[0]->getTilePosition();
			int neighbourValue = m_neighbours[0]->getValue();

			for (int i = 0; i < (m_totalTiles); i++)
			{
				sf::Vector2f m_tilePosition = m_tiles[i]->getTilePosition();

				if (m_tiles[i]->getGoalState() == false && m_tiles[i]->getWallState() == false)
				{
					//Left
					if (m_tilePosition.x == (m_neighbourPosition.x - m_tileSize.x) && m_tilePosition.y == (m_neighbourPosition.y))
					{

						if (m_tiles[i]->getValue() == NULL )
						{
							m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(neighbourValue + 1);
						}
					}

					//Top left
					else if (m_tilePosition.x == (m_neighbourPosition.x - m_tileSize.x) && m_tilePosition.y == m_neighbourPosition.y - m_tileSize.y)
					{
						if (m_tiles[i]->getValue() == NULL)
						{
							m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(neighbourValue + 1);
						}
					}

					//Top right
					else if (m_tilePosition.x == (m_neighbourPosition.x + m_tileSize.x) && m_tilePosition.y == m_neighbourPosition.y - m_tileSize.y)
					{
						if (m_tiles[i]->getValue() == NULL)
						{
							m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(neighbourValue + 1);
						}
					}

					//Right
					else if (m_tilePosition.x == (m_neighbourPosition.x + m_tileSize.x) && m_tilePosition.y == m_neighbourPosition.y)
					{
						if (m_tiles[i]->getValue() == NULL)
						{
							m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(neighbourValue + 1);
						}
					}

					//Bottom right
					else if (m_tilePosition.x == (m_neighbourPosition.x + m_tileSize.x) && m_tilePosition.y == m_neighbourPosition.y + m_tileSize.y)
					{
						if (m_tiles[i]->getValue() == NULL)
						{
							m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(neighbourValue + 1);
						}
					}

					//Bottom left
					else if (m_tilePosition.x == (m_neighbourPosition.x - m_tileSize.x) && m_tilePosition.y == m_neighbourPosition.y + m_tileSize.y)
					{
						if (m_tiles[i]->getValue() == NULL)
						{
							m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(neighbourValue + 1);
						}
					}

					//UP
					if (m_tilePosition.x == (m_neighbourPosition.x) && m_tilePosition.y == m_neighbourPosition.y + m_tileSize.y)
					{
						if (m_tiles[i]->getValue() == NULL)
						{
							m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(neighbourValue + 1);
						}
					}

					//Down
					if (m_tilePosition.x == (m_neighbourPosition.x) && m_tilePosition.y == m_neighbourPosition.y - m_tileSize.y)
					{
						if (m_tiles[i]->getValue() == NULL)
						{
							m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(neighbourValue + 1);
						}
					}
				}
			}
			m_neighbours.erase(m_neighbours.begin());
			m_neighbours.erase(std::unique(m_neighbours.begin(), m_neighbours.end()),m_neighbours.end());
			
		}

	//update values next to walls 
	std::vector<Tile*> m_walls;
	for (int i = 0; i < m_totalTiles; i++)
	{
		if (m_tiles[i]->getWallState() == true)
		{
			m_walls.push_back(m_tiles[i]);
		}
	}
	for (int w = 0; w < m_walls.size(); w++)
	{
		for (int i = 0; i < (m_totalTiles); i++)
		{
			if (m_tiles[i]->getWallState() == false)
			{
				sf::Vector2f m_tilePosition = m_tiles[i]->getTilePosition();

				//Left tile
				if (m_tilePosition.x == (m_goalPosition.x - m_tileSize.x) && m_tilePosition.y == m_goalPosition.y)
				{
					m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
				}
				//Top left
				else if (m_tilePosition.x == (m_goalPosition.x - m_tileSize.x) && m_tilePosition.y == m_goalPosition.y - m_tileSize.y)
				{
					m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
				}
				//Top right
				else if (m_tilePosition.x == (m_goalPosition.x + m_tileSize.x) && m_tilePosition.y == m_goalPosition.y - m_tileSize.y)
				{
					m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
				}
				//Right
				else if (m_tilePosition.x == (m_goalPosition.x + m_tileSize.x) && m_tilePosition.y == m_goalPosition.y)
				{
					m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
				}
				//Bottom right
				else if (m_tilePosition.x == (m_goalPosition.x + m_tileSize.x) && m_tilePosition.y == m_goalPosition.y + m_tileSize.y)
				{
					m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
				}
				//Bottom left
				else if (m_tilePosition.x == (m_goalPosition.x - m_tileSize.x) && m_tilePosition.y == m_goalPosition.y + m_tileSize.y)
				{
					m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
				}
				//UP
				if (m_tilePosition.x == (m_goalPosition.x) && m_tilePosition.y == m_goalPosition.y + m_tileSize.y)
				{
					m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
				}
				//Down
				if (m_tilePosition.x == (m_goalPosition.x) && m_tilePosition.y == m_goalPosition.y - m_tileSize.y)
				{
					m_neighbours.push_back(m_tiles[i]); m_tiles[i]->setValue(1);
				}

			}
		}
	}
	for (int i = 0; i < (m_totalTiles); i++)
	{
		if (m_tiles[i]->getGoalState() == true)
		{
			m_tiles[i]->setColor(sf::Color::Green);
		}
		else if (m_tiles[i]->getStartState() == true)
		{
			m_tiles[i]->setColor(sf::Color::Red);
		}
		else if (m_tiles[i]->getWallState() == true)
		{
			m_tiles[i]->setColor(sf::Color::Black);
		}
		else
		{
			if (m_tiles[i]->getValue() == NULL)
			{
				m_tiles[i]->setColor(sf::Color(80, 80, 80, 255));
			}
			else
			{
				int fade = m_tiles[i]->getValue() * 5;
				if (fade > 255)
				{
					fade = 255;
				}

				m_tiles[i]->setColor(sf::Color(0, 0, 0, fade));
			}
			
		}
	}

	SetFlow();
}

void Game::SetFlow()
{

	//loop to reset tiles to null
	for (int i = 0; i < m_totalTiles; i++)
	{
		if (m_tiles[i]->getWallState() == false)
		{
			m_tiles[i]->setGoalDistance(NULL);
		}
	}

	int m_goalID = m_goalRow + (m_goalCol * 50);
	sf::Vector2f m_goalPosition = m_tiles[m_goalID]->getTilePosition();
	sf::Vector2f m_tileSize = m_tiles[0]->getTileSize();

	std::vector<Tile*> m_neighbours;
	std::vector<Tile*> m_path;


	//initial loop to get neighbours of the goal
	for (int i = 0; i < (m_totalTiles); i++)
	{
		sf::Vector2f m_tilePosition = m_tiles[i]->getTilePosition();

		//Left tile
		if (m_tilePosition.x == (m_goalPosition.x - m_tileSize.x) && m_tilePosition.y == m_goalPosition.y)
		{
			m_neighbours.push_back(m_tiles[i]);
			m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x)- (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y- m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));
		}
		//Top left
		else if (m_tilePosition.x == (m_goalPosition.x - m_tileSize.x) && m_tilePosition.y == m_goalPosition.y - m_tileSize.y)
		{
			m_neighbours.push_back(m_tiles[i]);
			m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));
		}
		//Top right
		else if (m_tilePosition.x == (m_goalPosition.x + m_tileSize.x) && m_tilePosition.y == m_goalPosition.y - m_tileSize.y)
		{
			m_neighbours.push_back(m_tiles[i]);
			m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));
		}
		//Right
		else if (m_tilePosition.x == (m_goalPosition.x + m_tileSize.x) && m_tilePosition.y == m_goalPosition.y)
		{
			m_neighbours.push_back(m_tiles[i]);
			m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));
		}
		//Bottom right
		else if (m_tilePosition.x == (m_goalPosition.x + m_tileSize.x) && m_tilePosition.y == m_goalPosition.y + m_tileSize.y)
		{
			m_neighbours.push_back(m_tiles[i]); 
			m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));
		}
		//Bottom left
		else if (m_tilePosition.x == (m_goalPosition.x - m_tileSize.x) && m_tilePosition.y == m_goalPosition.y + m_tileSize.y)
		{
			m_neighbours.push_back(m_tiles[i]);
			m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));
		}
		//UP
		if (m_tilePosition.x == (m_goalPosition.x) && m_tilePosition.y == m_goalPosition.y + m_tileSize.y)
		{
			m_neighbours.push_back(m_tiles[i]);
			m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));
		}
		//Down
		if (m_tilePosition.x == (m_goalPosition.x) && m_tilePosition.y == m_goalPosition.y - m_tileSize.y)
		{
			m_neighbours.push_back(m_tiles[i]); 
			m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));
		}

	}

	//while loop to loop through all neighbours and there neigbours expanding
	while (m_neighbours.size() != 0)
	{

		int n = m_neighbours.size();
		sf::Vector2f m_neighbourPosition = m_neighbours[0]->getTilePosition();
		int neighbourValue = m_neighbours[0]->getValue();

		for (int i = 0; i < (m_totalTiles); i++)
		{
			sf::Vector2f m_tilePosition = m_tiles[i]->getTilePosition();

			if (m_tiles[i]->getGoalState() == false && m_tiles[i]->getWallState() == false)
			{
				//Left
				if (m_tilePosition.x == (m_neighbourPosition.x - m_tileSize.x) && m_tilePosition.y == (m_neighbourPosition.y))
				{

					if (m_tiles[i]->getGoalDistance() == NULL)
					{
						m_neighbours.push_back(m_tiles[i]); 
						m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));

					}
				}

				//Top left
				else if (m_tilePosition.x == (m_neighbourPosition.x - m_tileSize.x) && m_tilePosition.y == m_neighbourPosition.y - m_tileSize.y)
				{
					if (m_tiles[i]->getGoalDistance() == NULL)
					{
						m_neighbours.push_back(m_tiles[i]); 
						m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));

					}
				}

				//Top right
				else if (m_tilePosition.x == (m_neighbourPosition.x + m_tileSize.x) && m_tilePosition.y == m_neighbourPosition.y - m_tileSize.y)
				{
					if (m_tiles[i]->getGoalDistance() == NULL)
					{
						m_neighbours.push_back(m_tiles[i]); 
						m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));
					}
				}

				//Right
				else if (m_tilePosition.x == (m_neighbourPosition.x + m_tileSize.x) && m_tilePosition.y == m_neighbourPosition.y)
				{
					if (m_tiles[i]->getGoalDistance() == NULL)
					{
						m_neighbours.push_back(m_tiles[i]);
						m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));
					}
				}

				//Bottom right
				else if (m_tilePosition.x == (m_neighbourPosition.x + m_tileSize.x) && m_tilePosition.y == m_neighbourPosition.y + m_tileSize.y)
				{
					if (m_tiles[i]->getGoalDistance() == NULL)
					{
						m_neighbours.push_back(m_tiles[i]);
						m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));

					}
				}

				//Bottom left
				else if (m_tilePosition.x == (m_neighbourPosition.x - m_tileSize.x) && m_tilePosition.y == m_neighbourPosition.y + m_tileSize.y)
				{
					if (m_tiles[i]->getGoalDistance() == NULL)
					{
						m_neighbours.push_back(m_tiles[i]);
						m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));

					}
				}

				//UP
				if (m_tilePosition.x == (m_neighbourPosition.x) && m_tilePosition.y == m_neighbourPosition.y + m_tileSize.y)
				{
					if (m_tiles[i]->getGoalDistance() == NULL)
					{
						m_neighbours.push_back(m_tiles[i]);
						m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));

					}
				}

				//Down
				if (m_tilePosition.x == (m_neighbourPosition.x) && m_tilePosition.y == m_neighbourPosition.y - m_tileSize.y)
				{
					if (m_tiles[i]->getGoalDistance() == NULL)
					{
						m_neighbours.push_back(m_tiles[i]);
						m_tiles[i]->setGoalDistance(sqrt(pow((m_tilePosition.x + m_tileSize.x) - (m_goalPosition.x + m_tileSize.x), 2) + pow((m_tilePosition.y - m_tileSize.y) - (m_goalPosition.y - m_tileSize.y), 2)));
					}
				}
			}
		}
		m_neighbours.erase(m_neighbours.begin());
		m_neighbours.erase(std::unique(m_neighbours.begin(), m_neighbours.end()), m_neighbours.end());

	}

	/*
	implament the checking of direction  to make path 
	*/


	SetPath();
}

void Game::SetPath()
{

	int m_startID = m_startRow + (m_startRow * 50);
	sf::Vector2f m_startPosition = m_tiles[m_startID]->getTilePosition();
	sf::Vector2f m_tileSize = m_tiles[0]->getTileSize();

	std::vector<Tile*> m_neighbours;


	//initial loop to get neighbours of the goal
	for (int i = 0; i < (m_totalTiles); i++)
	{
		sf::Vector2f m_tilePosition = m_tiles[i]->getTilePosition();

		//Left tile
		if (m_tilePosition.x == (m_startPosition.x - m_tileSize.x) && m_tilePosition.y == m_startPosition.y)
		{
			m_neighbours.push_back(m_tiles[i]);
		}
		//Top left
		else if (m_tilePosition.x == (m_startPosition.x - m_tileSize.x) && m_tilePosition.y == m_startPosition.y - m_tileSize.y)
		{
			m_neighbours.push_back(m_tiles[i]); 
		}
		//Top right
		else if (m_tilePosition.x == (m_startPosition.x + m_tileSize.x) && m_tilePosition.y == m_startPosition.y - m_tileSize.y)
		{
			m_neighbours.push_back(m_tiles[i]);
		}
		//Right
		else if (m_tilePosition.x == (m_startPosition.x + m_tileSize.x) && m_tilePosition.y == m_startPosition.y)
		{
			m_neighbours.push_back(m_tiles[i]); 
		}
		//Bottom right
		else if (m_tilePosition.x == (m_startPosition.x + m_tileSize.x) && m_tilePosition.y == m_startPosition.y + m_tileSize.y)
		{
			m_neighbours.push_back(m_tiles[i]); 
		}
		//Bottom left
		else if (m_tilePosition.x == (m_startPosition.x - m_tileSize.x) && m_tilePosition.y == m_startPosition.y + m_tileSize.y)
		{
			m_neighbours.push_back(m_tiles[i]);
		}
		//UP
		if (m_tilePosition.x == (m_startPosition.x) && m_tilePosition.y == m_startPosition.y + m_tileSize.y)
		{
			m_neighbours.push_back(m_tiles[i]); 
		}
		//Down
		if (m_tilePosition.x == (m_startPosition.x) && m_tilePosition.y == m_startPosition.y - m_tileSize.y)
		{
			m_neighbours.push_back(m_tiles[i]); 
		}


	

	}






}




