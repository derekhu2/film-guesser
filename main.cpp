#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <cstdlib>
#include <ctime>

#include <curl/curl.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "movie.h"
#include "inforow.h"
#include "curl_util.h"

bool isValidInt(const std::string &str, int &outValue) {
    try {
        outValue = std::stoi(str);
        return true;
    } catch (const std::invalid_argument &) {
        return false;
    } catch (const std::out_of_range &) {
        return false;
    }
}

std::vector<std::vector<std::string>> csvParse(std::string filename)
{
    std::ifstream input(filename);

    if (!input.is_open())
    {
        std::cerr << "Error opening file " << filename << std::endl;
    }

    std::vector<std::vector<std::string>> csvRows;

    for (std::string line; std::getline(input, line);) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;
        bool inQuotes = false;
        std::string field;

        while (ss.good()) {
            char c = ss.get();  // Read character by character

            if (c == '"') {
                inQuotes = !inQuotes;  // Toggle inQuotes mode
            } else if (c == ',' && !inQuotes) {
                // When encountering a comma **outside** of quotes, push the field to the row
                row.push_back(field);
                field.clear();
            } else if (c != EOF) {
                field += c;  // Append characters to the field
            }
        }
        row.push_back(field);  // Add last field in row

        csvRows.push_back(row);
    }

    return csvRows;
}

std::map<std::string, int> letterboxdCsvToMap(std::string filename){
    std::vector<std::vector<std::string>> csv = csvParse(filename);
    std::map<std::string, int> result;

    bool first = true;
    // iterate through each row
    for (const auto &row : csv) {
        if (!first){
            // visit 2nd col: title, 3rd col: year
            std::cout << row[1] << ", " << row[2] << std::endl;

            int releaseYear = 0;

            if (isValidInt(row[2], releaseYear))
                result[row[1]] = releaseYear;
        }
        first = false;
    }
    std::cout << std::endl;

    return result;
}

std::pair<int, std::string> searchResults(std::string callResponse, int targetYear){
    std::pair<int, std::string> result(-1, "");
    
    // parse JSON response
    try{
        json jsonResponse = json::parse(callResponse);

        // enter "results"
        if (jsonResponse.contains("results") && jsonResponse["results"].is_array()){
            int count = 0;

            bool first = true;

            // iterate through results
            for (const auto &item : jsonResponse["results"]){
                if (item.contains("title") && item.contains("release_date") && item.contains("id")){ // found

                    int releaseYear = 0;
                    isValidInt(item["release_date"].get<std::string>(), releaseYear);

                    if (targetYear == -1){
                        std::cout << count + 1 << ". "
                            << item["title"]
                            << " (" << releaseYear
                            << "), ID: " << item["id"] << std::endl;
                    }

                    if (targetYear == releaseYear && first){
                        result = std::make_pair(item["id"].get<int>(), item["title"].get<std::string>());
                        first = false;
                    }

                    count++;

                    if (count >= 10)
                        break; // stop after 10 movies
                }
            } // for
        } // if
        else{
            std::cerr << "No movie results found." << std::endl;
        }
    } // try
    catch (const json::exception &e)
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }

    return result;
}

std::map<int, std::string> searchMap(std::map<std::string, int> titleToYear){
    std::map<int, std::string> result;

    for (const auto &film : titleToYear){
        std::string title = film.first;
        title = urlEncode(title);
        std::string urlSuffix = "search/multi?include_adult=false&language=en-US&page=1&query=" + title;

        CURL *curl = curl_easy_init();
        std::string callResponse = apiCall(curl, urlSuffix);

        curl_easy_cleanup(curl);

        std::pair<int, std::string> idAndTitle = searchResults(callResponse, film.second);
        result[idAndTitle.first] = idAndTitle.second;
    }

    return result;
}

int getRandomID(std::map<int, std::string> idToTitle) {
    if (idToTitle.empty())
        return -1;

    std::srand(std::time(nullptr));
    int randomIndex = std::rand() % idToTitle.size();

    auto it = idToTitle.begin();
    std::advance(it, randomIndex);

    return it->first;
}

int main()
{
    std::string input;

    // prompt for CSV
    std::cout << "Enter Letterboxd film list csv: ";
    std::getline(std::cin, input);
    std::map<std::string, int> titleToYear = letterboxdCsvToMap(input);

    std::cout << "Calling TMDB API to make film list... \n" << std::endl;
    std::map<int, std::string> idToTitle = searchMap(titleToYear);

    int answerID = getRandomID(idToTitle);

    if (answerID == -1)
        std::cout << "Film list is empty." << std::endl;

    Movie answer(answerID);
//    std::cout << "Answer: " << std::endl;
//    answer.print();

    std::cout << "How many lives do you want? (input -1 for infinite)" << std::endl;
    std::getline(std::cin, input);
    std::cout << std::endl;

    int lives = -1;
    isValidInt(input, lives);

    // loop until dead
    while (true) {
        if (lives == 0){ // dead
            std::cout << "YOU DIED!\n" << std::endl;
            break;
        }

        // prompt for movie title
        std::cout << "Search for guess: ";
        std::getline(std::cin, input);
        input = urlEncode(input);
        std::string urlSuffix = "search/multi?include_adult=false&language=en-US&page=1&query=" + input;

        CURL *hnd = curl_easy_init();
        std::string callResponse = apiCall(hnd, urlSuffix);

        // Cleanup
        curl_easy_cleanup(hnd);

        searchResults(callResponse, -1);

        std::cout << std::endl;
        std::cout << "Enter guess ID: ";
        std::getline(std::cin, input);

        int guessID = 0;
        isValidInt(input, guessID);

        Movie guess(guessID);
        guess.print();

        InfoRow row(answer, guess);
        row.print();

        if (guess.getID() == answerID){ // win
            std::cout << "GREAT JOB BRO!" << std::endl;
            break;
        }

        lives--;
    }

    std::cout << "Answer: " << std::endl;
    answer.print();
}