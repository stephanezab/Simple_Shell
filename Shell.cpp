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

using namespace std;

// Function prototypes
void execute_command(const string& inputLine);
void help();
void echo(const string& input);
void exitCLI();
void date();
void PWD();
void clear();
void LS(const string &input);
void CD(const string& input);
void cat(const string& input);
void MK(const string& input);
void rm(const string& input);
void execpp(const string& input);

// Command map associating command strings with their corresponding functions
map<string, void (*)(const string&)> commands = {
    {"help", [](const string&){ help(); }},
    {"echo", echo},
    {"exit", [](const string&){ exitCLI(); }},
    {"date", [](const string&){ date(); }},
    {"pwd", [](const string&){ PWD(); }},
    {"clear", [](const string&){ clear(); }},
    {"ls", LS},
    {"cd", CD},
    {"cat", cat},
    {"mkdir", MK},
    {"rm", rm},
    {"g++", execpp}

};

int main() {

    system("clear"); // to clear the main shell 

    string inputLine;
    cout << "Simple CLI. Type 'help' for a list of commands.\n";
   
    
    while (true) {
        filesystem::path cwd = filesystem::current_path();
        cout << ">> ";
        getline(cin, inputLine);

        
        // Find the first space to separate the command from its arguments
        execute_command(inputLine);
        
        
    }

    return 0;
}

void execute_command(const string& inputLine){
    size_t index = inputLine.find("&&");


    if (index == string::npos)
    {
        size_t spaceIndex = inputLine.find(' ');
        string command = inputLine.substr(0, spaceIndex);
        string argument = spaceIndex != string::npos ? inputLine.substr(spaceIndex + 1) : "";

        // Execute the command if it is found
        auto commandIt = commands.find(command);

        if (commandIt != commands.end())
        {
            commandIt->second(argument);
        }
        else
        {
            cout << "Unknown command. Type 'help' for a list of commands.\n";
        }
    }
    else if (index != string::npos){ // fix this today
        string line1 = inputLine.substr(0, index - 1);
        string line2 = inputLine.substr(index + 3);

        execute_command(line1);
        execute_command(line2);

    }

    

}

void help() {
    cout << "Available commands:\n"
              << "help               - Show this help message\n"
              << "echo [text]        - Echo the input text\n"
              << "date               - Show the current working directory\n"
              << "pwd                - Show the current path\n"
              << "ls (-l)            - Show all the files in the current directory\n"
              << "cat                - Show the content of the file\n"
              << "clear              - clear the command line\n"
              << "cd [directory]     - Move to the desired directory\n"
              << "mkdir [directory]  - Create the desired directory\n"
              << "rm [file]          - Remove file from directory\n"
              << "rm -r [directory]  - Remove directory\n"
              << "exit               - Exit the CLI\n";

}

void echo(const string& input) {
    cout << input << endl;
}

void exitCLI() {
    cout << "Exiting CLI.\n";
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

void LS(const string &input){
    

    pid_t pid = fork(); // Create a new process

    if (pid == -1) {
        // If fork() returns -1, an error occurred
        cerr << "Fork failed" << endl;
        exit(1);
    } else if (pid == 0) {
        // Child process
        // Execute the command using execvp
        if (input == ""){
            char* argv[] = {const_cast<char*>("ls"), nullptr};
            execvp("ls", const_cast<char* const*>(argv));
        }
        else if (input == "-l"){
            char* argv[] = {const_cast<char*>("ls"), const_cast<char*>("-l"), nullptr};
            execvp("ls", const_cast<char* const*>(argv));
        }
        // If execvp returns, it means an error occurred
        cerr << "Error executing execvp" << endl;
        exit(EXIT_FAILURE); // Exit the child process with an error code
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for the child process to finish

        // if (WIFEXITED(status)) {
        //     cout << "Child process exited with code " << WEXITSTATUS(status) << endl;
        // }
    }
}

void CD(const string& input){
    
    chdir(input.c_str());
    
}

void cat(const string& input){
    vector<const char*> command = {"cat", input.c_str(), nullptr};

    pid_t pid = fork(); // Create a new process

    if (pid == -1) {
        // If fork() returns -1, an error occurred
        cerr << "Fork failed" << endl;
        exit(1);
    } else if (pid == 0) {
        // Child process
        // Execute the command using execvp
        execvp(command[0], const_cast<char* const*>(command.data()));

        // If execvp returns, it means an error occurred
        cerr << "Error executing execvp" << endl;
        exit(EXIT_FAILURE); // Exit the child process with an error code
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for the child process to finish

    }
}

void MK(const string& input){
    vector<const char*> command = {"mkdir", input.c_str(), nullptr};

    pid_t pid = fork(); // Create a new process

    if (pid == -1) {
        // If fork() returns -1, an error occurred
        cerr << "Fork failed" << endl;
        exit(1);
    } else if (pid == 0) {
        // Child process
        // Execute the command using execvp
        execvp(command[0], const_cast<char* const*>(command.data()));

        // If execvp returns, it means an error occurred
        cerr << "Error executing execvp" << endl;
        exit(EXIT_FAILURE); // Exit the child process with an error code
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for the child process to finish

    }
}

void rm(const string& input){ 
    size_t f = input.find("-r");
    

    pid_t pid = fork(); // Create a new process

    if (pid == -1) {
        // If fork() returns -1, an error occurred
        cerr << "Fork failed" << endl;
        exit(1);
    } else if (pid == 0) {
        // Child process
        // Execute the command using execvp
        if (f == string::npos){
            vector<const char*> command = {"rm", input.c_str(), nullptr};
            execvp(command[0], const_cast<char* const*>(command.data()));
        }
        else{
            string com2 = input.substr(f,f+2);
            string arg = input.substr(f+3);
            vector<const char*> command = {"rm", com2.c_str(), arg.c_str(), nullptr};
            execvp(command[0], const_cast<char* const*>(command.data()));

        }     

        // If execvp returns, it means an error occurred
        cerr << "Error executing execvp" << endl;
        exit(EXIT_FAILURE); // Exit the child process with an error code
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for the child process to finish

    }
}

void execpp(const string& input){

    size_t s = input.find(".");
    string file = input.substr(0,s);

    vector<const char*> command = {"g++", input.c_str(),"-o", file.c_str(), nullptr};

    pid_t pid = fork(); // Create a new process

    if (pid == -1) {
        // If fork() returns -1, an error occurred
        cerr << "Fork failed" << endl;
        exit(1);
    } else if (pid == 0) {
        // Child process
        // Execute the command using execvp
        execvp(command[0], const_cast<char* const*>(command.data()));

        // If execvp returns, it means an error occurred
        cerr << "Error executing execvp" << endl;
        exit(EXIT_FAILURE); // Exit the child process with an error code
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for the child process to finish

        string slash = "./";
        string execfile =  slash + file;

        vector<const char*> command2 = {execfile.c_str(), nullptr};

        pid_t pid2 = fork();

        if(pid2 == -1){
            cerr << "Second Fork failed" << endl;
            exit(1);

        }else if(pid2 == 0){
            
            execvp(command2[0], const_cast<char* const*>(command2.data()));

             cerr << "Error executing second execvp" << endl;
             exit(EXIT_FAILURE); 
        }else{
            int status2;
            waitpid(pid2, &status2, 0);
        }
    }
}