#include <iostream>
#include <functional>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "movie.h"
#include "curl_util.h"

// constructor
Movie::Movie(int id){
    this->ID = id;
    std::string urlSuffix = "movie/" + std::to_string(id);

    CURL *curl = curl_easy_init();
    std::string response = apiCall(curl, urlSuffix);

    // std::cout << response << std::endl;

    // parse JSON response
    try{
        json item = json::parse(response);

        if (item.contains("title") && item.contains("id") && item.contains("release_date")
            && item.contains("genres") && item.contains("vote_average") && item.contains("budget")
            && item.contains("revenue") && item.contains("runtime") && item.contains("popularity")
            && item.contains("spoken_languages")){ // found

            this->Title = item["title"].get<std::string>();
            //ID = item["id"].get<int>();
            this->ReleaseYear = std::stoi(item["release_date"].get<std::string>());
            this->Rating = item["vote_average"].get<double>();
            this->Budget = item["budget"].get<int>();
            this->Revenue = item["revenue"].get<int>();
            this->Runtime = item["runtime"].get<int>();
            this->Popularity = item["popularity"].get<double>();

            for (const auto &lang : item["spoken_languages"]){
                this->SpokenLanguages[lang["english_name"].get<std::string>()] = lang["name"].get<std::string>();
            }

            for (const auto &genre : item["genres"]){
                this->Genres.insert({genre["id"].get<int>(), genre["name"].get<std::string>()});
            } // for
        } // if
    } // try
    catch (const json::exception &e)
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }

    curl_easy_cleanup(curl);
}

void Movie::print (){
    std::cout << "Title: " << this->Title << std::endl;
    std::cout << "ID: " << this->ID << std::endl;
    std::cout << "Release year: " << this->ReleaseYear << std::endl;
    std::cout << "Rating: " << this->Rating << std::endl;
    std::cout << "Budget: " << this->Budget << std::endl;
    std::cout << "Revenue: " << this->Revenue << std::endl;
    std::cout << "Runtime: " << this->Runtime << std::endl;
    std::cout << "Popularity: " << this->Popularity << std::endl;

    std::cout << "Genres: ";

    bool first = true;
    for (const auto &genre : this->Genres){
        if (!first)
            std::cout << ", ";
        std::cout << genre.second << " (" << genre.first << ")";
        first = false;
    }
    std::cout << std::endl;

    std::cout << "Spoken languages: ";

    first = true;
    for (const auto &lang : this->SpokenLanguages){
        if (!first)
            std::cout << ", ";
        std::cout << lang.first << " (" << lang.second << ")";
        first = false;
    }

    std::cout << "\n" << std::endl;
}

std::string Movie::getTitle() const{
    return this->Title;
}

int Movie::getID() const{
    return this->ID;
}

int Movie::getReleaseYear() const{
    return this->ReleaseYear;
}

std::map<int, std::string> Movie::getGenres() const{
    return this->Genres;
}

double Movie::getRating() const{
    return this->Rating;
}

int Movie::getBudget() const{
    return this->Budget;
}

int Movie::getRevenue() const{
    return this->Revenue;
}

int Movie::getRuntime() const{
    return this->Runtime;
}

double Movie::getPopularity() const{
    return this->Popularity;
}

std::map<std::string, std::string> Movie::getSpokenLanguages() const{
    return this->SpokenLanguages;
}