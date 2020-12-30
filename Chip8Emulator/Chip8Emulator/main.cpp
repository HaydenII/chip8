#include "main.h"

std::vector<uint8_t> readFile(const char* filename)
{
	// open the file:
	std::basic_ifstream<uint8_t> file(filename, std::ios::binary);

	// read the data:
	return std::vector<uint8_t>((std::istreambuf_iterator<uint8_t>(file)), std::istreambuf_iterator<uint8_t>());
}

inline long long Get_Current_Time() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void run_system(Bus* bus) {
	long long lastTime = Get_Current_Time();
	long long CurrentTime;
	while (GetAsyncKeyState(VK_ESCAPE) == 0) {
		CurrentTime = Get_Current_Time();
		if (CurrentTime - lastTime >= 2) {
			bus->cpu.clock();
			lastTime = CurrentTime;
		}
	}
}

int main() {
	Bus bus;
	bus.cpu.reset();

	std::vector<uint8_t> program = readFile("C:\\Users\\hayde\\Downloads\\octojam6title.ch8");

	uint16_t WritePtr = 0x200;
	for (auto& instr : program) {
		bus.mem.write(WritePtr++, instr);
	}

	sf::RenderWindow window(sf::VideoMode(1024, 512), "Chip 8 Emulator", sf::Style::Default);

	std::vector<std::vector<sf::RectangleShape>> ScrShapes(64, std::vector<sf::RectangleShape>(32));
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 32; j++) {
			ScrShapes[i][j] = sf::RectangleShape(sf::Vector2f(16.f, 16.f));
			ScrShapes[i][j].setPosition(sf::Vector2f((i * 16), (j * 16)));
			ScrShapes[i][j].setFillColor(sf::Color::Black);
		}
	}

	window.setFramerateLimit(60);

	std::thread runCPU(run_system, &bus);

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
			uint64_t TempDisplay = (bus).display[x];
			for (int y = 0; y < 64; y++) {

				sf::Color b;
				if ((TempDisplay >> (63 - y)) & 0x1) {
					b = ScrShapes[y][x].getFillColor();
				}

				// Offset by 63 becuase by 64 would shift the last pixel off screen
				if (((TempDisplay >> (63 - y)) & 0x1) && (ScrShapes[y][x].getFillColor() == sf::Color::Black)) {
					ScrShapes[y][x].setFillColor(sf::Color::White);
				}
				else if (!((TempDisplay >> (63 - y)) & 0x1) && (ScrShapes[y][x].getFillColor() == sf::Color::White)){
					ScrShapes[y][x].setFillColor(sf::Color::Black);
				}
				window.draw(ScrShapes[y][x]);
			}
		}

		window.display();
	}

	//std::thread runCPU(run_system, &bus);

	//C8Display game = C8Display();
	//game.ConnectBus(&bus);
	//game.Construct(64, 32, 16, 16);
	//game.Start();

	return 1;
}
#
