/*
=============================================
Comments policy:
-Jason will (attempt to) comment everything, even intuitive stuff
-If something has already been explained, it may optionally not be re-explained
-When in doubt, add a comment
-Functions' arguments should be explained by their names. 
	If you are confused by a function call, try hovering over it, or right-click>Go to declaration
=============================================
*/

//Standard io
#include <iostream>
//Strings
#include <string>
//Class for mapping keys to values in an unordered array
#include <map>
//For binary operations, tedious things
#include <algorithm>
//To check for a file
#include <sys/stat.h>
//To read/write from files
#include <fstream>
//Essentially a dynamic array
#include <vector>
//To iterate!
#include <iterator>
//For doing bit operations rather than byte
#include <bitset>

//Listing different possible commands
enum cmdType {
	quit,
	encrypt,
	decrypt,
	nothing,
};

//Check if a file exists
bool fileExists (std::string name) {
	//Not really sure what this does, but it works if you put in the right args
	struct stat buffer;   
	return (stat(name.c_str(), &buffer) == 0); 
}
//Read a file as a string
std::string getFileContentsString(std::string filename) {
	//Open the file in input mode
	std::ifstream file(filename);
	//String to save file's contents
	std::string contents;
	//Send file reader (g) to end of file
    file.seekg(0, std::ios::end);
	//Make string exactly large enough to hold file. This is for a trick for efficiency
    contents.resize(file.tellg());
	//Send file reader back to beginning
    file.seekg(0, std::ios::beg);
	//Read file. Memory amount read is however large contents is, because it is the same size as the file
    file.read(&contents[0], contents.size());
	//Close the file (so we don't leave a lock on it)
    file.close();
	return contents;
}

//Read a file as binary
std::vector<char> getFileContentsBinary(std::string filename) {
	//Open file in binary input mode
	std::ifstream input(filename, std::ios::binary);
	//Creates vector called buffer, copying input to it
	std::vector<char> buffer((
		std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));
	return buffer;
}

//Write a file from a string with no encryption
void none(std::string inFile, std::string outFile, std::string cipher) {
	//Open file in output mode
	std::ofstream writeFile;
	//Create the file
	writeFile.open(outFile);
	//Save the contents of the input file to a string
	std::string contents = getFileContentsString(inFile);
	//This << operator is the same for any stream (eg. std::cout). It writes the string to the stream
	writeFile<<contents;
	writeFile.close();
}

//Write a file from a string with XOR encrpytion
void xor(std::string inFile, std::string outFile, std::string cipher) {
	//Open file in output mode
	std::ofstream writeFile;
	//Create the file
	writeFile.open(outFile);
	//Save the contents of the input file to a string
	std::string contents = getFileContentsString(inFile);
	
	//actual encryption goes here

	//This << operator is the same for any stream (eg. std::cout). It writes the string to the stream
	writeFile << contents;
	writeFile.close();
}

//Write a file from binary in whitespace
void eWhitespace(std::string inFile, std::string outFile, std::string cipher) {
	std::ofstream writeFile;
	writeFile.open(outFile);
	std::vector<char> buffer = getFileContentsBinary(inFile);
	std::string contents;
	std::bitset<8> byte;
	for (std::vector<char>::const_iterator i = buffer.begin(); i != buffer.end(); i++) {
		byte = (*i);
		for (int n = 0; n < 8; n++){
			contents += (byte[n] ? '.' : ','); //first is space, second is non-breaking space
		}
	}
	writeFile<<contents;
	writeFile.close();
}
//Decrypt a file from whitespace
void dWhitespace(std::string inFile, std::string outFile, std::string cipher) {
	std::ofstream writeFile;
	writeFile.open(outFile);
	std::string contents = getFileContentsString(inFile);
	std::string byte;
	std::string output;
	for (int i = 0; i != contents.length(); i++) {
		byte += (contents.at(i) == '.' ? 1 : 0);
		if (byte.length()==8) {
			output += strtol(byte.c_str(), NULL, 2);
			byte = "";
		}
	}
	writeFile << output;
	writeFile.close();
}

int main(int argc, char* argv[])
{
	//To determine when to stop the program
	bool running = true;
	//The key for the command map
	std::string cmdString;
	//The value of the command
	cmdType cmd = cmdType::nothing;
	//Map of command strings to commands
	std::map<std::string,cmdType> cmdMap;
	cmdMap["quit"] = quit;
	cmdMap["exit"] = quit;
	cmdMap["encrypt"] = encrypt;
	cmdMap["decrypt"] = decrypt;
	//The key for the encryption type map
	std::string encryptString;
	//The value of the encryption type (a function)
	void(*encryptFunc)(std::string, std::string, std::string) = none;
	//map of encryption strings to encryption types
	std::map<std::string,void(*)(std::string, std::string, std::string)> encryptionMap;
	encryptionMap["none"] = none;
	encryptionMap["whitespace"] = eWhitespace;
	encryptionMap["ws"] = eWhitespace;
	//Equivalent for decryption
	std::string decryptString;
	void(*decryptFunc)(std::string, std::string, std::string) = none;
	std::map<std::string, void(*)(std::string, std::string, std::string)> decryptionMap;
	decryptionMap["none"] = none;
	decryptionMap["whitespace"] = dWhitespace;
	decryptionMap["ws"] = dWhitespace;
	//Different input strings
	std::string inFile;
	std::string outFile;
	std::string cipher;

	std::cout<<"JAM Backup and Encryption Utility"<<std::endl;

	//Stops program after one loop if running is set to false
	while (running) {
		std::cout<<"Enter a command: encrypt, decrypt, quit"<<std::endl;
		//Gets the next input line from std::cin, stores it in cmdString
		getline(std::cin,cmdString);
		//Makes cmdString lowercase
		std::transform(cmdString.begin(), cmdString.end(), cmdString.begin(), ::tolower);
		//Making sure we aren't accessing an place in the map that doesn't exist
		try {
			cmd = cmdMap.at(cmdString);
		}
		catch (std::out_of_range e) {
			std::cout<<"Invalid command"<<std::endl;
		}

		//Do something different depending on command type
		switch (cmd) {
		//Shuts down the program
		case quit:
			running = false;
			break;
		//Encrypt a file
		case encrypt:
			std::cout<<"Encryption type (choose one): none, whitespace"<<std::endl;
			getline(std::cin,encryptString);
			std::transform(encryptString.begin(), encryptString.end(), encryptString.begin(), ::tolower);
			//Set the encryption function to the one corresponding to encryptString
			try {
				encryptFunc = encryptionMap.at(encryptString);
			}
			catch (std::out_of_range e) {
				std::cout<<"Invalid encryption type"<<std::endl;
				break;
			}
			std::cout<<"File name (with extension):"<<std::endl;
			getline(std::cin,inFile);
			//Make sure file exists
			if (!fileExists(inFile)) {
				std::cout<<"File does not appear to exist"<<std::endl;
				break;
			}
			std::cout<<"Output file name:"<<std::endl;
			getline(std::cin, outFile);
			std::cout<<"Cipher:"<<std::endl;
			getline(std::cin, cipher);
			//Send all of this information to the encryption function - it will do the rest of the work
			encryptFunc(inFile,outFile,cipher);
			break;
		//Decrypt a file. Essentially the same as encrypt
		case decrypt:
			std::cout << "Encryption type (choose one): none, whitespace" << std::endl;
			getline(std::cin, decryptString);
			std::transform(decryptString.begin(), decryptString.end(), decryptString.begin(), ::tolower);
			try {
				decryptFunc = decryptionMap.at(decryptString);
			}
			catch (std::out_of_range e) {
				std::cout << "Invalid decryption type" << std::endl;
				break;//test this
			}
			std::cout << "File name (with extension):" << std::endl;
			getline(std::cin, inFile);
			if (!fileExists(inFile)) {
				std::cout << "File does not appear to exist" << std::endl;
				break;
			}
			std::cout << "Output file name:" << std::endl;
			getline(std::cin, outFile);
			std::cout << "Cipher:" << std::endl;
			getline(std::cin, cipher);
			decryptFunc(inFile, outFile, cipher);
			break;
		default:
			break;
		}
	}





	/* SFML STUFF (ignore this)

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