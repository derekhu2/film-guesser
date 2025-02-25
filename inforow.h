#pragma once

#include <string>

#include "movie.h"

class InfoRow {
public:
    std::string Title;
    int ID;
    int ReleaseYear; // -1: answer < guess
    int Genres; // -1: none, 0: some, 1: all
    int Rating;
    int Budget;
    int Revenue;
    int Runtime;
    int Popularity;
    int SpokenLanguages;

    // constructor
    InfoRow(Movie answer, Movie guess);

    // print
    void higherLowerPrint(int var, std::string nameVar);
    void matchesPrint(int var, std::string nameVar);
    void print();

    template <typename T>
    int getHigherLower(const Movie& answer, const Movie& guess, T (Movie::*getter)() const){
        auto answerVal = (answer.*getter)();
        auto guessVal = (guess.*getter)();

        if (answerVal < guessVal)
            return -1;
        else if (answerVal > guessVal)
            return 1;
        else
            return 0;
    }

    template <typename K, typename V>
    int getMatches(const Movie& answer, const Movie& guess, std::map<K, V> (Movie::*getter)() const) {
        std::map<K, V> answerMap = (answer.*getter)();
        std::map<K, V> guessMap = (guess.*getter)();

        int numInAnswer = answerMap.size();
        int numInGuess = guessMap.size();
        int matchingCount = 0;

        for (const auto& item : guessMap) {
            if (answerMap.find(item.first) != answerMap.end())
                matchingCount++;
        }

        if (numInAnswer == numInGuess && matchingCount == numInAnswer)
            return 1;  // all entries match
        else if (matchingCount > 0)
            return 0;  // some entries match
        else
            return -1; // no entries match
    }
};
