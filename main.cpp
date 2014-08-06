//#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <map>

enum inputType {
	quit,
	nothing,
};



int main(int argc, char* argv[])
{
	bool running = true;
	std::string inputString;
	inputType input = inputType::nothing;
	std::map<std::string,inputType> inputMap;
	inputMap["quit"] = quit;

	while (running) {
		getline(std::cin,inputString);
		try {
			input = inputMap.at(inputString);
		}
		catch (std::out_of_range e) {
			std::cout<<"Invalid input"<<std::endl;
		}

		switch (input) {
		case quit:
			running = false;
			break;
		default:
			break;
		}
	}





	/* SFML STUFF

	=============================================================================================

	//Create Window
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1000, 600), "Backup Utility");
    
	while (window->isOpen()) {
		//Pull events, deal with them
        sf::Event event;
        while (window->pollEvent(event)) {
            switch (event.type) {
				//On window closed. Don't think we have to do anything special here
				case sf::Event::Closed:
					window->close();
					break;
				//On mouse pressed down
				case sf::Event::MouseButtonPressed:
					{
					break;
					}
				//On click
				case sf::Event::MouseButtonReleased:
					{
					break;
					}
				//On scroll
				case sf::Event::MouseWheelMoved:
					{
					break;
					}
				case sf::Event::KeyReleased:
					{
					break;
					}
				case sf::Event::TextEntered:
					{
					break;
					}
				default:
					break;
			}
        }
    }
	*/

	//Success!
	return 0;
}