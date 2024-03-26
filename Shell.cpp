#include <iostream>
#include <string>
#include <map>
#include <cstdlib>

// Function prototypes
void help();
void echo(const std::string& input);
void exitCLI();
void date();
void PWD();

// Command map associating command strings with their corresponding functions
std::map<std::string, void (*)(const std::string&)> commands = {
    {"help", [](const std::string&){ help(); }},
    {"echo", echo},
    {"exit", [](const std::string&){ exitCLI(); }},
    {"date", [](const std::string&){ date(); }},
    {"pwd", [](const std::string&){ PWD(); }}
    
    // Add more commands here
};

int main() {
    std::string inputLine;
    std::cout << "Simple CLI. Type 'help' for a list of commands.\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, inputLine);

        // Find the first space to separate the command from its arguments
        size_t spaceIndex = inputLine.find(' ');
        std::string command = inputLine.substr(0, spaceIndex);
        std::string argument = spaceIndex != std::string::npos ? inputLine.substr(spaceIndex + 1) : "";
        

        // Execute the command if it is found
        auto commandIt = commands.find(command);
        std::cout << ""<<"\n";
        if (commandIt != commands.end()) {
            commandIt->second(argument);
        } else {
            std::cout << "Unknown command. Type 'help' for a list of commands.\n";
        }
    }

    return 0;
}

void help() {
    std::cout << "Available commands:\n"
              << "help          - Show this help message\n"
              << "echo [text]   - Echo the input text\n"
              << "exit          - Exit the CLI\n";
}

void echo(const std::string& input) {
    std::cout << input << std::endl;
}

void exitCLI() {
    std::cout << "Exiting CLI.\n";
    exit(0);
}

void date(){
    system("date");
}

void PWD(){
    system("pwd");
}
