#include <iostream>
#include <string>
#include <map>
#include <cstdlib>

#include <filesystem>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>     // For execvp, fork, chdir
#include <vector>
#include <sys/wait.h>   // For waitpid



// Function prototypes
void execute_command(const std::string& inputLine);
void help();
void echo(const std::string& input);
void exitCLI();
void date();
void PWD();
void clear();
void LS();
void CD(const std::string& input);
void cat(const std::string& input);

// Command map associating command strings with their corresponding functions
std::map<std::string, void (*)(const std::string&)> commands = {
    {"help", [](const std::string&){ help(); }},
    {"echo", echo},
    {"exit", [](const std::string&){ exitCLI(); }},
    {"date", [](const std::string&){ date(); }},
    {"pwd", [](const std::string&){ PWD(); }},
    {"clear", [](const std::string&){ clear(); }},
    {"ls", [](const std::string&){ LS(); }},
    {"cd", CD},
    {"cat", cat}

    
    // Add more commands here
};

int main() {

    system("clear"); // to clear the main shell 

    std::string inputLine;
    std::cout << "Simple CLI. Type 'help' for a list of commands.\n";
   
    
    while (true) {
        std::filesystem::path cwd = std::filesystem::current_path();
        std::cout <<cwd << ">> ";
        std::getline(std::cin, inputLine);

        // Find the first space to separate the command from its arguments
        execute_command(inputLine);
        
    }

    return 0;
}

void execute_command(const std::string& inputLine){
    size_t spaceIndex = inputLine.find(' ');
    std::string command = inputLine.substr(0, spaceIndex);
    std::string argument = spaceIndex != std::string::npos ? inputLine.substr(spaceIndex + 1) : "";
        

        // Execute the command if it is found
    auto commandIt = commands.find(command);
        
    if (commandIt != commands.end()) {
            commandIt->second(argument);
    } else {
            std::cout << "Unknown command. Type 'help' for a list of commands.\n";
    }

}

void help() {
    std::cout << "Available commands:\n"
              << "help            - Show this help message\n"
              << "echo [text]     - Echo the input text\n"
              << "date            - Show the current working directory\n"
              << "pwd             - Show the current path\n"
              << "ls              - Show all the files in the current directory\n"
              << "cat             - Show the content of the file\n"
              << "clear           - clear the command line\n"
              << "cd [directory]  - Move to the desired directory\n"
              << "exit            - Exit the CLI\n";

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

void clear(){
    system("clear");
}

void LS(){
    DIR *dp;    // holds current directory
    dirent *d;  // holds the directory and file info inside current directory 
    std::filesystem::path cwd = std::filesystem::current_path();
    std::string cwdString = cwd.string(); // Convert path to string


    if(dp = opendir(cwdString.c_str())){
        // read directory dp and send info to d
        while(d = readdir(dp)){
            if (d->d_name[0] != '.' ){
                std::cout<<d->d_name<<"\n";
            } 
            
        }
    }

}

void CD(const std::string& input){
    
    chdir(input.c_str());
    
}

void cat(const std::string& input){
    std::vector<const char*> command = {"cat", input.c_str(), nullptr};

    pid_t pid = fork(); // Create a new process

    if (pid == -1) {
        // If fork() returns -1, an error occurred
        std::cerr << "Fork failed" << std::endl;
        exit(1);
    } else if (pid == 0) {
        // Child process
        // Execute the command using execvp
        execvp(command[0], const_cast<char* const*>(command.data()));

        // If execvp returns, it means an error occurred
        std::cerr << "Error executing execvp" << std::endl;
        exit(EXIT_FAILURE); // Exit the child process with an error code
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for the child process to finish

        if (WIFEXITED(status)) {
            std::cout << "Child process exited with code " << WEXITSTATUS(status) << std::endl;
        }
    }
}