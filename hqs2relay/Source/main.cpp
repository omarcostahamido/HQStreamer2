/*
* HQStreamer2 - Stream audio between DAWs across the internet
* Copyright (C) 2020 Sauraen, <sauraen@gmail.com>
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
* 
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <JuceHeader.h>
#include "relay.hpp"

#include <iostream>
#include <string>

int main(int argc, char* argv[]){
	//Parse command line options
	bool interactive = false;
	for(int i=1; i<argc; ++i){
		String command(argv[i]);
		if(command == "-i"){
			interactive = true;
		}else if(command == "--setpassphrase"){
			if(i < argc-1){
				std::cout << "Incorrect usage, you just leaked the passphrase \"" << argv[i+1] << "\" to your Bash history\n";
				return 1;
			}
			std::cout << "Enter the new passphrase: ";
			std::string in_s;
			if(std::getline(std::cin, in_s).eof()) return 0;
			String pass(in_s);
			HQS2Relay::StorePassphrase(pass);
			std::cout << "Passphrase set\n";
			return 0;
		}else if(command == "-h" || command == "-?" || command == "--help"){
			std::cout << "Usage: hqs2relay [-i] [--setpassphrase]\n";
			std::cout << "    -i: interactive mode\n";
			std::cout << "    --setpassphrase [no args]: Interactively set the passphrase and exit\n";
			return 0;
		}else{
			std::cout << "Invalid command-line argument: " << command << "\n";
			std::cout << "Try hqs2relay --help\n";
			return 1;
		}
	}
    HQS2Relay relay;
	if(interactive){
		//Input loop
		while(true){
			std::cout << "> ";
			std::string in_s;
			if(std::getline(std::cin, in_s).eof()) break;
			String input(in_s);
			String command = input.upToFirstOccurrenceOf(" ", false, true).toLowerCase();
			String args = input.fromFirstOccurrenceOf(" ", false, true);
			if(command.isEmpty()){
				std::cout << "(No command entered.)\n";
			}else if(command == "exit" || command == "quit"){
				break;
			}else if(command == "setpassphrase"){
				relay.SetPassphrase(args);
			}else if(command == "sessions"){
				relay.PrintSessionsInfo();
			}else if(command == "status"){
				if(args.isEmpty()){
					std::cout << "Usage: status sessionname\n";
				}else{
					relay.PrintSessionInfo(args);
				}
			}else if(command == "kick"){
				if(args.isEmpty()){
					std::cout << "Usage: kick sessionname\n";
				}else{
					relay.KickSession(args);
				}
			}else if(command == "help"){
				std::cout << "Commands: exit/quit, setpassphrase, sessions, status, kick\n";
			}else{
				std::cout << "Invalid command \"" << command << "\", please type \"help\"\n";
			}
		}
	}else{
		std::cout << "Non-interactive mode, the only thing you can do is kill the process\n";
		while(true){
			Thread::yield();
			Thread::sleep(100);
		}
	}
}
