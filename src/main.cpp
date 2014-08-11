//#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <sys/stat.h>
#include <fstream>

enum cmdType {
	quit,
	encrypt,
	decrypt,
	nothing,
};

bool fileExists (std::string name) {
  struct stat buffer;   
  return (stat(name.c_str(), &buffer) == 0); 
}

std::string getFileContents(std::string filename) {
	std::ifstream file(filename);
	std::string contents;
    file.seekg(0, std::ios::end);
    contents.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&contents[0], contents.size());
    file.close();
	return contents;
}

void none(std::string inFile, std::string outFile, std::string cipher) {
	std::ofstream writeFile;
	writeFile.open(outFile);
	std::string contents = getFileContents(inFile);
	writeFile<<contents;
	writeFile.close();
}

int main(int argc, char* argv[])
{
	bool running = true;
	std::string cmdString;
	cmdType cmd = cmdType::nothing;
	std::map<std::string,cmdType> cmdMap;
	cmdMap["quit"] = quit;
	cmdMap["exit"] = quit;
	cmdMap["encrypt"] = encrypt;
	cmdMap["decrypt"] = decrypt;
	std::string encryptString;
	void(*encryptFunc)(std::string, std::string, std::string) = none;
	std::map<std::string,void(*)(std::string, std::string, std::string)> encryptionMap;
	encryptionMap["none"] = none;
	std::string inFile;
	std::string outFile;
	std::string cipher;

	std::cout<<"JAM Backup and Encryption Utility"<<std::endl;

	while (running) {
		std::cout<<"Enter a command: encrypt, decrypt, quit"<<std::endl;
		getline(std::cin,cmdString);
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
			std::cout<<"Encryption type (choose one): none"<<std::endl;
			getline(std::cin,encryptString);
			std::transform(encryptString.begin(), encryptString.end(), encryptString.begin(), ::tolower);
			try {
				encryptFunc = encryptionMap.at(encryptString);
			}
			catch (std::out_of_range e) {
				std::cout<<"Invalid encryption type"<<std::endl;
				break;//test this
			}
			std::cout<<"File name (with extension):"<<std::endl;
			getline(std::cin,inFile);
			if (!fileExists(inFile)) {
				std::cout<<"File does not appear to exist"<<std::endl;
				break;
			}
			std::cout<<"Output file name:"<<std::endl;
			getline(std::cin, outFile);
			std::cout<<"Cipher:"<<std::endl;
			getline(std::cin, cipher);
			encryptFunc(inFile,outFile,cipher);
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