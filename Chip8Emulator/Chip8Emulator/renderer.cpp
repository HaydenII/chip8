#include "renderer.h"
#include "bus.h"

renderer::renderer(Bus* Inref) : renderer()
{
	ConnectBus(Inref);
}

renderer::renderer() : bus(nullptr), ScrShapes(64, std::vector<sf::RectangleShape>(32))
{
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 32; j++) {
			ScrShapes[i][j] = sf::RectangleShape(sf::Vector2f(16.f, 16.f));
			ScrShapes[i][j].setPosition(sf::Vector2f((i * 16), (j * 16)));
			ScrShapes[i][j].setFillColor(sf::Color::Black);
		}
	}
	window.setFramerateLimit(60);
}

void renderer::render_loop()
{
	if (bus == nullptr) { return; }

	// Create window and open window
	window.create(sf::VideoMode(1024, 512), "Chip 8 Emulator", sf::Style::Default);

	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);

		for (int x = 0; x < 32; x++) {
			uint64_t TempDisplay = (bus)->display[x];
			for (int y = 0; y < 64; y++) {

				sf::Color b;
				if ((TempDisplay >> (63 - y)) & 0x1) {
					b = ScrShapes[y][x].getFillColor();
				}

				// Offset by 63 becuase by 64 would shift the last pixel off screen
				if (((TempDisplay >> (63 - y)) & 0x1) && (ScrShapes[y][x].getFillColor() == sf::Color::Black)) {
					ScrShapes[y][x].setFillColor(sf::Color::White);
				}
				else if (!((TempDisplay >> (63 - y)) & 0x1) && (ScrShapes[y][x].getFillColor() == sf::Color::White)) {
					ScrShapes[y][x].setFillColor(sf::Color::Black);
				}
				window.draw(ScrShapes[y][x]);
			}
		}

		window.display();
	}
}

void renderer::ConnectBus(Bus* InBus)
{
	bus = InBus;
}
