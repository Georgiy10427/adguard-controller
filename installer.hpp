#include <iostream>
#include <string>
#include <filesystem>

using namespace std;

string filepath = "/opt/device-activation";
void first_setup();

void run_installer()
{
    bool exist = filesystem::exists(filepath);
    if(!exist)
    {
        first_setup();
    }
}

void first_setup() 
{
    system("sudo apt update");
    system("sudo apt upgrade -y");
    system("sudo apt install cron -y");
    system("sudo apt install curl -y");
    system("sudo chmod 777 run.sh");
    system("curl -s -S -L https://raw.githubusercontent.com/AdguardTeam/AdGuardHome/master/scripts/install.sh | sh -s -- -v");
    cout << "-------------------------\n";
    cout << "Please add the line to cron:\n";
    cout << "@reboot sudo nohup /home/dietpi/controller/run.sh &\n";
    cout << "Auto installation complete.\n";
    system(string("> " + filepath).c_str());
}