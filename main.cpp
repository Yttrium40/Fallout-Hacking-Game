#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>

int getDifficulty()
{
    int difficulty;
    while (true)
    {
        std::cin >> difficulty;
        if(std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(100, '\n');
        }
        std::cin.ignore(100, '\n');
        return difficulty;
    }
}

std::string getWordGuess()
{
    std::string guess;
    std::cin >> guess;
    for (char &c : guess)
    {
        c = std::toupper(c);
    }
    return guess;
}

int getNumberCorrect(std::string word, std::string guess)
{
    int correctCount{0};
    for (int i = 0; i < word.length(); ++i)
    {
        if (word[i] == guess[i]) { ++correctCount; }
    }
    return correctCount;
}

void populateOptions(std::vector<std::string> &options, int numberOptions, int wordLength)
{
    std::ifstream ifs;
    ifs.open("dictionary.txt", std::ifstream::in);

    std::vector<std::string> randomChoices;
    randomChoices.reserve(100);

    std::string line;
    while (!ifs.eof())
    {
        std::getline(ifs, line);
        if (line.length() == wordLength)
        {
            randomChoices.push_back(line);
        }
    }

    std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dis(0, randomChoices.size() - 1);
    while (options.size() != numberOptions)
    {
        int randomNumber{dis(rng)}; // TODO make this work
        if (randomChoices[randomNumber] != "")
        {
            options.push_back(randomChoices[randomNumber]);
            randomChoices[randomNumber] = "";
        }
    }
}

void playGame(int difficulty)
{
    std::cout << "Difficulty is at: " << difficulty << '\n';
    int wordLength;
    switch (difficulty)
    {
        case 1: wordLength = 3; break;
        case 2: wordLength = 4; break;
        case 3: wordLength = 5; break;
        case 4: wordLength = 6; break;
        case 5: wordLength = 7; break;
    }

    int numberOptions{10};
    std::vector<std::string> options;
    options.reserve(numberOptions);
    populateOptions(options, numberOptions, wordLength);

    std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dis(0, options.size() - 1);
    int randomNumber{dis(rng)};

    for (std::string &word : options)
    {
        for (char &c : word)
        {
            c = std::toupper(c);
        }
        std::cout << word << '\n';
    }

    std::string answer{options[randomNumber]};
    int guessCount{4};
    while (guessCount != 0)
    {
        std::cout << "Enter your guess (" << guessCount << " left): ";
        std::string guess{getWordGuess()};
        std::cout << guess << '\n';
        --guessCount;

        int numberCorrect{getNumberCorrect(answer, guess)};
        std::cout << numberCorrect << '/' << wordLength << " correct\n";

        if (numberCorrect == wordLength)
        {
            std::cout << "You win!\n";
            return;
        }
    }
    std::cout << "You lose!\n";
    std::cout << "The answer was: " << answer << "!\n";
}

int main()
{
    bool continueGame{true};
    while (continueGame)
    {
        std::cout << "Enter difficulty 1-5 (1 easiest, 5 hardest): ";
        playGame(getDifficulty());
        while (true)
        {
            std::cout << "Do you want to continue (y/n): ";
            char ynInput;
            std::cin >> ynInput;
            if (ynInput == 'y') { break; }
            if (ynInput == 'n') { continueGame = false; break; }
        }
    }

    return 0;
}
