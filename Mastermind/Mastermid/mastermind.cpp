#include <algorithm>
#include <iostream>
#include <random>

#include "mastermind.h"

using namespace mastermind;

namespace
{
    std::string GenerateResolutionCode(const std::string &color_pegs)
    {
        // Generating a sequence of 4 unique pseudo-random numbers between 0 and 5
        std::random_device rd;  // Seed 
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(0, 5);

        std::list<int> idx_list; 
        while (idx_list.size() < 4)
        {
            int idx = distrib(gen);

            auto it = std::find(idx_list.begin(), idx_list.end(), idx);
            if (it == idx_list.end())  // element not found in list
                idx_list.push_back(idx);
        }

        // Converting to string of colors
        std::string out = "";

        for (auto ii : idx_list)
            out.push_back(color_pegs[ii]);

        return out;
    }

    bool AllCharsInPegsList(const std::string& input_string, const std::string &color_pegs)
    {
        for (const char ch : input_string)
            if ((color_pegs.find(ch) == std::string::npos))
                return false;

        return true;
    }
}

namespace mastermind
{
    std::pair<std::string, std::string> Mastermind::PlayRound(std::string input_str)
    {
        // Returns a feedback string of 4 characters, where "k" corresponds to black dot, i.e. right color, right position,
        // "w" corresponds to white dot, i.e. right colow, wrong position.
        std::string feedback_string = "";

        // 1. Finding positional overlaps (black dots)
        for (size_t idx=0; idx<input_str.size(); idx++)
        {
            auto input_ch = input_str[idx];
            auto resolution_ch = _resolution_code[idx];

            if (input_ch == resolution_ch) 
                feedback_string.push_back('k');
                
            else if ((_resolution_code.find(input_ch) != std::string::npos))
                feedback_string.push_back('w');
        }

        std::sort(feedback_string.begin(), feedback_string.end());
            
        _round_counter++;

        if (feedback_string == "kkkk")
            _player_wins = true;

        if (_round_counter >= _max_rounds)
            _game_over = true;

        feedback_string.push_back('\n');  // For formatting purposes
            
        // The idea here is that the feedback string is made up of the 4 characters (black or white dots in the game),
        // While output string is a user-friendly statement that keeps track of the game
        int remaining_attempts = _max_rounds - _round_counter;

        std::string output_string = "Try again! " + std::to_string(remaining_attempts) + " attempts remaining.\n";

        if (_player_wins)
            output_string = "YOU WON!!\n";

        else if (_game_over)
            output_string = "GAME OVER: You exceeded the allowed attempts.\n";

        return std::make_pair(feedback_string, output_string);
    }

    Mastermind::Mastermind(const int &max_rounds) :
        _color_pegs("rgobyp"),
        _max_rounds(max_rounds),
        _round_counter(0),
        _game_over(false),
        _player_wins(false)
    {
        _resolution_code = ::GenerateResolutionCode(_color_pegs);
    }

    void Mastermind::operator()()
    {
        // Main execution loop
        while (true)
        {
            // User inputs a string made up of 4 characters, out of the 6 available. Each corresponds to a color. 
            // With this implementation, some sanity checks are of course needed to restrain the user from wrong inputs,
            // but they would be redundant in an implementation with a proper interface, i.e. a board, colored pegs, etc.
            std::string input_string;
            std::cout << "Insert a string of 4 colors.\n";
            std::cout << "r->red, g->green, o->orange, b->blue, y->yellow, p->purple.\n";
            std::cin >> input_string;

            // Transforming to lowercase
            std::transform(input_string.begin(), input_string.end(), input_string.begin(),
                [](char c) { return std::tolower(c); });

            // Some sanity checks - they would not be needed with a proper app interface, with pegs and rows that the user can choose from.
            if (input_string.size() != 4)
            {
                std::cout << "String must be 4-characters long, try again!";
                continue;
            }

            if (! ::AllCharsInPegsList(input_string, _color_pegs))
            {
                std::cout << "Input colors must be in the specified list!";
                continue;
            }

            bool unique_chars = std::unique(input_string.begin(), input_string.end()) == input_string.end();
            if (!unique_chars)
            {
                std::cout << "Characters in the string must be unique!";
                continue;
            }

            std::pair<std::string, std::string> output_str_pair = PlayRound(input_string);

            std::string feedback_string = output_str_pair.first;
            std::string output_string = output_str_pair.second;

            std::cout << feedback_string;
            std::cout << output_string;

            if (_game_over || _player_wins)
                break;
        }
    }
}