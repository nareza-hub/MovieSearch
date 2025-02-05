/* Name:            Neoma Reza
   Email:           nareza@crimson.ua.edu
   Course Section:  Fall 2024 CS 201 - 001
   Homework#:       3 */
// To compile:      g++ -std=c++20 HW3Sample.cpp -o HW3Sample.exe
// To run:          ./HW3Sample.exe dbfile1.txt query.txt

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_map>
#include <vector>
#include <chrono> //for timing

int main(int argc, char *argv[]) {
    //check for correct command-line arguments
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <db file> <query file>" << std::endl;
        return -1;
    }

    std::string dbFileName = argv[1];
    std::string queryFileName = argv[2];

    //define data structures for storing movies and actors
    std::unordered_map<std::string, std::vector<std::string>> movieToActors;
    std::unordered_map<std::string, std::vector<std::string>> actorToMovies;
    
    std::string line;
    std::regex delim("/");
    int i = 0; //record counter

    //open dbfile
    std::ifstream dbFile(dbFileName);
    if (!dbFile.is_open()) {
        std::cerr << "Unable to open file: " << dbFileName << std::endl;
        return -1;
    }
    
    auto start = std::chrono::high_resolution_clock::now(); //start timer for creation
    //read and process each line in the database file
    while (std::getline(dbFile, line)) {
        //use regex to split movie and actors
        auto begin = std::sregex_token_iterator(line.begin(), line.end(), delim, -1);
        auto end = std::sregex_token_iterator();

        if (begin == end) continue; // Skip if no tokens found

        //first token is the movie name
        std::string movie = *begin++;
        std::vector<std::string> actors;

        //remaining tokens are actor names
        while (begin != end) {
            std::string actor = *begin++;
            actors.push_back(actor);

            //map actor to movie
            actorToMovies[actor].push_back(movie);
        }

        //map movie to actors
        movieToActors[movie] = actors;
        i++; //increment record count for each movie
    }
    dbFile.close(); //close dbfile
    auto end = std::chrono::high_resolution_clock::now(); //end timer for create data structure

    //open query file
    std::ifstream queryFile(queryFileName);
    if (!queryFile.is_open()) {
        std::cerr << "Unable to open file: " << queryFileName << std::endl;
        return -1;
    }
    int lineNum = 0; //initialize line number for query processing
    //process each query
    std::string query;
    auto searchStart = std::chrono::high_resolution_clock::now();
    while (std::getline(queryFile, query)) {
        lineNum++; //increment line number for each query
        std::cout << "Line " << lineNum << ": "; //output line number
        if (movieToActors.find(query) != movieToActors.end()) { 
            //query is a movie name
            std::cout << "Actors in '" << query << "':" << std::endl;
            for (const auto& actor : movieToActors[query]) {
                std::cout << "  " << actor << std::endl;
            }
        } else if (actorToMovies.find(query) != actorToMovies.end()) {
            //query is an actor name
            std::cout << "Movies starring '" << query << "':" << std::endl;
            for (const auto& movie : actorToMovies[query]) {
                std::cout << "  " << movie << std::endl;
            }
        } else {
            std::cout << query << " - Not Found" << std::endl; //no match found (actor and/or movie)
        }
    }
    queryFile.close(); //close query file
    auto searchEnd = std::chrono::high_resolution_clock::now(); //end search timer

    //calculate timing
    std::chrono::duration<double> creationTime = end - start; //calculate time taken for create data structure
    std::chrono::duration<double> searchTime = searchEnd - searchStart; //calculate time taken for search
    std::chrono::duration<double> totalTime = creationTime + searchTime; //calculate time taken total

    //print out the results
    std::cout << "\nNumber of records (movies): " << i << std::endl; //# of records
    std::cout << "Time taken (create data structure): " << creationTime.count() << " seconds\n"; //time taken for create data structure
    std::cout << "Time taken for search: " << searchTime.count() << " seconds\n"; //time taken for search
    std::cout << "Total time: " << totalTime.count() << " seconds\n"; //time taken total

    return 0;
}