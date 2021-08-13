#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Bus;

class renderer {
public:
	sf::RenderWindow window;
	std::vector<std::vector<sf::RectangleShape>> ScrShapes;
	Bus* bus;

public:
	renderer();
	renderer(Bus*);
	void render_loop();
	void ConnectBus(Bus*);

	// Creates and opens the window
	void close_window();
};
