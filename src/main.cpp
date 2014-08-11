//#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>

enum cmdType {
	quit,
	encrypt,
	decrypt,
	nothing,
};

enum encryptionType {
	none,
};

int main(int argc, char* argv[])
{
	bool running = true;
	std::string inputString;
	std::string cmdString;
	cmdType cmd = cmdType::nothing;
	std::map<std::string,cmdType> cmdMap;
	cmdMap["quit"] = quit;
	cmdMap["encrypt"] = encrypt;
	cmdMap["decrypt"] = decrypt;
	encryptionType encryption = encryptionType::none;
	std::map<std::string,encryptionType> encryptionMap;
	encryptionMap["none"] = none;

	std::cout<<"JAM Backup and Encryption Utility"<<std::endl;

	while (running) {
		getline(std::cin,inputString);
		cmdString = inputString.substr(0, inputString.find_first_of(" "));
		std::transform(cmdString.begin(), cmdString.end(), cmdString.begin(), ::tolower);
		try {
			cmd = cmdMap.at(cmdString);
		}
		catch (std::out_of_range e) {
			std::cout<<"Invalid command"<<std::endl;
		}

		switch (cmd) {
		case quit:
			running = false;
			break;
		case encrypt:

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