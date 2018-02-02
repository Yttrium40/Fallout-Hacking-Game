#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>

std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

int getWordLength()
{
    int wordLength;
    while (true)
    {
        std::cin >> wordLength;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(100, '\n');
        }
        std::cin.ignore(100, '\n');
        return wordLength;
    }
}

std::string getWordGuess(const std::vector<std::string> &options)
{
    std::string guess;
    std::cin >> guess;
    for (char &c : guess)
    {
        c = std::toupper(c);
    }
    bool goodInput = false;
    for (const std::string &str : options)
    {
        if (str == guess) { goodInput = true; break; }
    }
    if (!goodInput) { return ""; }
    return guess;
}

int getNumberCorrect(std::string word, std::string guess)
{
    int correctCount { 0 };
    for (int i = 0; i < word.length(); ++i)
    {
        if (word[i] == guess[i]) { ++correctCount; }
    }
    return correctCount;
}

int populateOptions(std::vector<std::string> &options, int numberOptions, int wordLength)
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

    std::uniform_int_distribution<> dis(0, randomChoices.size() - 1);
    std::uniform_int_distribution<> disGuess(0, numberOptions - 1);

    int answerIndex { disGuess(rng) };
    while (true)
    {
        std::cerr << "LOOP ENTERED\n";
        while (options.size() != numberOptions)
        {
            int randomNumber{ dis(rng) };
            if (randomChoices[randomNumber] != "")
            {
                options.push_back(randomChoices[randomNumber]);
                randomChoices[randomNumber] = "";
            }
        }
        int sumCorrect { 0 };
        int sumLetters { 0 };

        for (std::string &str : options)
        {
            sumCorrect += getNumberCorrect(str, options[answerIndex]);
            sumLetters += str.length();
        }

        if (static_cast<double>(sumCorrect) / sumLetters <= 0.2)
        {
            options.clear();
            continue;
        }
        else { return answerIndex; }
    }
}

void playGame(int wordLength)
{
    std::cout << "Difficulty is at: " << wordLength <<  " words" << '\n';

    int numberOptions { 10 };
    std::vector<std::string> options;
    options.reserve(numberOptions);

    int answerIndex { populateOptions(options, numberOptions, wordLength) };
    std::cerr << "LOOP DONE\n";

    for (std::string &word : options)
    {
        for (char &c : word)
        {
            c = std::toupper(c);
        }
        std::cout << word << '\n';
    }

    std::string answer { options[answerIndex] };
    int guessCount { 4 };
    while (guessCount != 0)
    {
        std::string guess{""};
        while (guess.empty())
        {
            std::cout << "Enter your guess (" << guessCount << " left): ";
            guess = getWordGuess(options);
        }
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
    bool continueGame { true };
    while (continueGame)
    {
        std::cout << "Enter word length: ";
        playGame(getWordLength());
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
