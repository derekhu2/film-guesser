#include <iostream>
#include <map>

#include "inforow.h"
#include "movie.h"

// constructor
InfoRow::InfoRow(Movie answer, Movie guess){
    this->ID = guess.getID();
    this->Title = guess.getTitle();

    this->ReleaseYear = getHigherLower(answer, guess, &Movie::getReleaseYear);
    this->Rating = getHigherLower(answer, guess, &Movie::getRating);
    this->Budget = getHigherLower(answer, guess, &Movie::getBudget);
    this->Revenue = getHigherLower(answer, guess, &Movie::getRevenue);
    this->Runtime = getHigherLower(answer, guess, &Movie::getRuntime);
    this->Popularity = getHigherLower(answer, guess, &Movie::getPopularity);

    this->Genres = getMatches(answer, guess, &Movie::getGenres);
    this->SpokenLanguages = getMatches(answer, guess, &Movie::getSpokenLanguages);
}

void InfoRow::higherLowerPrint(int var, std::string nameVar){
    if (var == -1)
        std::cout << nameVar << ": " << "v" << std::endl;
    else if (var == 1)
        std::cout << nameVar << ": " << "^" << std::endl;
    else
        std::cout << nameVar << ": " << "CORRECT" << std::endl;
}

void InfoRow::matchesPrint(int var, std::string nameVar){
    if (var == -1)
        std::cout << nameVar << ": "<< "no matches" << std::endl;
    else if (var == 0)
        std::cout << nameVar << ": " << "some matches" << std::endl;
    else
        std::cout << nameVar << ": " << "CORRECT" << std::endl;
}

void InfoRow::print(){
    // std::cout << "ID: " << this->ID << std::endl;
    std::cout << "Title: " << this->Title << std::endl;

    this->higherLowerPrint(this->ReleaseYear, "Release year");
    this->higherLowerPrint(this->Rating, "Rating");
    this->higherLowerPrint(this->Budget, "Budget");
    this->higherLowerPrint(this->Revenue, "Revenue");
    this->higherLowerPrint(this->Runtime, "Runtime");
    this->higherLowerPrint(this->Popularity, "Popularity");

    this->matchesPrint(this->Genres, "Genres");
    this->matchesPrint(this->SpokenLanguages, "Spoken languages");

    std::cout << std::endl;
}