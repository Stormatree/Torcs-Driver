#include <TorcsClient.hpp>
#include "Renderer.hpp"

#include <thread>
#include <list>
#include <string>
#include <map>

#include <SimpleDriver.h>
#include "MyDriver.hpp"
#include "OldDriver.hpp"

typedef std::map<unsigned int, BaseDriver*> DriverMap;

void makeDrivers(DriverMap& drivers){
	drivers[3001] = new MyDriver(true);

	drivers[3002] = new MyDriver;
	drivers[3003] = new MyDriver;
	drivers[3004] = new MyDriver;
}

void parseArgs(int argc, char* argv[], std::string& hostName, bool& rendering){
	// Default argument values
	rendering = true;
	hostName = "localhost";
	
	if (argc == 3){
		if ((std::string)argv[1] == "silent"){
			rendering = false;
			hostName = argv[2];
		}
		else if ((std::string)argv[2] == "silent"){
			rendering = false;
			hostName = argv[1];
		}
	}
	else if (argc == 2){
		if ((std::string)argv[1] == "silent")
			rendering = false;
		else
			hostName = argv[1];
	}
}

int main(int argc, char* argv[]){
	srand((time_t)time(0));

	// Set hostName and startPort using args, or default "localhost" and 3001
	std::string hostName;
	bool rendering;

	parseArgs(argc, argv, hostName, rendering);

	// Creation of new driver pointers derivded from BaseDriver
	DriverMap drivers;
	makeDrivers(drivers);
	
	// Creates individual threads for each driver to connect
	std::list<std::thread> threads;

	for (auto pair : drivers)
		threads.push_back(std::thread(race_thread, hostName, pair.first, pair.second));

	// Now the race has potentially started, initiate the debug renderer if rendering is on 
	if (rendering){
		Renderer::get().setWindowSize({ 640, 640 });
		Renderer::get().init();

		while (Renderer::get().running()){
			Renderer::get().update();

			std::this_thread::sleep_for(std::chrono::milliseconds(25));
		}

		std::cout << "Renderer thread stopped...\n";
	}		

	// Wait for each driver to finish
	for (std::thread& thread : threads)
		thread.join();

	// Kill everything
	for (auto pair : drivers)
		delete pair.second;

	threads.clear();
	drivers.clear();

	return 0;
}