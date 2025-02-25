#pragma once

#include <string>
#include <map>

class Movie {
public:
    std::string Title;
    int ID;
    int ReleaseYear;
    std::map<int, std::string> Genres;
    double Rating;
    int Budget;
    int Revenue;
    int Runtime;
    double Popularity;
    std::map<std::string, std::string> SpokenLanguages;

    // constructor
    Movie(int id);

    void print();

    std::string getTitle() const;
    int getID() const;
    int getReleaseYear() const;
    std::map<int, std::string> getGenres() const;
    double getRating() const;
    int getBudget() const;
    int getRevenue() const;
    int getRuntime() const;
    double getPopularity() const;
    std::map<std::string, std::string> getSpokenLanguages() const;

};
