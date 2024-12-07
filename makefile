all: clean compile link

link:
	g++ -o simulation bin/main.o bin/Action.o bin/Auxiliary.o bin/Facility.o bin/Plan.o bin/SelectionPolicy.o bin/Settlement.o bin/Simulation.o

compile: main Action Auxiliary Facility Plan SelectionPolicy Settlement Simulation

main:
	g++ -g -Weffc++ -Wall -std=c++11 -Iinclude -c -o bin/main.o src/main.cpp

Action:
	g++ -g -Weffc++ -Wall -std=c++11 -Iinclude -c -o bin/Action.o src/Action.cpp

Auxiliary:
	g++ -g -Weffc++ -Wall -std=c++11 -Iinclude -c -o bin/Auxiliary.o src/Auxiliary.cpp

Facility:
	g++ -g -Weffc++ -Wall -std=c++11 -Iinclude -c -o bin/Facility.o src/Facility.cpp

Plan:
	g++ -g -Weffc++ -Wall -std=c++11 -Iinclude -c -o bin/Plan.o src/Plan.cpp

SelectionPolicy:
	g++ -g -Weffc++ -Wall -std=c++11 -Iinclude -c -o bin/SelectionPolicy.o src/SelectionPolicy.cpp

Settlement:
	g++ -g -Weffc++ -Wall -std=c++11 -Iinclude -c -o bin/Settlement.o src/Settlement.cpp

Simulation:
	g++ -g -Weffc++ -Wall -std=c++11 -Iinclude -c -o bin/Simulation.o src/Simulation.cpp

clean:
	rm -f bin/*.o simulation

valgrind: clean compile link
	valgrind --leak-check=full --show-reachable=yes ./simulation config_file.txt