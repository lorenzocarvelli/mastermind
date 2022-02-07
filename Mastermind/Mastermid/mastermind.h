#pragma once
#include <string> 
#include <list> 


namespace mastermind
{
    class Mastermind
    {
    public:
        Mastermind(const int &max_rounds);
        void operator()();

    private:
        const int _max_rounds;
        int _round_counter;
        std::string _resolution_code;
        bool _game_over;
        bool _player_wins;
        std::string _color_pegs;

        std::pair<std::string, std::string> PlayRound(std::string input_str);
    };
}