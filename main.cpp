#include <iostream>
#include <string>
#include "color.hpp"
#include "list.h"
#include "Stack.h"
#include "Card.h"
#include "Game.h"
using namespace std;

void split(string comm, string arr[10])
{
    int i = 0;               // Index for traversing the input string
    int wordscount = 0;       // Number of words found so far
    string word = "";         // Temporary string to store current word

    while (comm[i] != '\0')
    {
        // If the current character is not a space, append it to 'word'
        if (comm[i] != ' ' && comm[i] != '\t' && comm[i] != '\n') // also consider tabs and newlines as whitespace
        {
            word += comm[i];  // Add current character to word
        }
        else
        {
            // If we hit a space and the word is non-empty, store it
            if (!word.empty())
            {
                arr[wordscount] = word;  // Store the word in the array
                wordscount++;            // Increment the word count
                word = "";               // Reset word for the next word
            }
        }
        i++;
    }

    // Add the last word if there is one (in case the string doesn't end with a space)
    if (!word.empty())
    {
        arr[wordscount] = word;
        wordscount++;
    }

    // Fill the rest of the array with empty strings, if necessary
    for (int j = wordscount; j < 10; j++)
    {
        arr[j] = "";  // Set unused slots in array to empty strings
    }
}

int main()
{
	system("color A0");
	cout << hue::bright_white<< endl;

	cout << "WELCOME TO SOLITAIRE\n";
    
    cout << hue::black;
	cout << hearts << diamonds << clubs << spades << "   ";
    cout << hue::red;
    cout << hearts << diamonds << clubs << spades << endl;
    cout << hue::black;

    Game game;

    string command;
    while (true) 
    {
        game.display();
        
        string command;
        cout << hue:: bright_white_on_purple << "Enter command: ";
        cout << hue::black_on_light_green;
        getline(cin, command);

        string arr[10];
        split(command, arr);
        char c = arr[0][0];
        char a = arr[3][0];

        int i = 0;
        
        if (a == 49)
            i = 1;
        if (a == 50)
            i = 2;
        if (a == 51)
            i = 3;
        if (a == 52)
            i = 4;
        if (a == 53)
            i = 5;
        if (a == 54)
            i = 6;
        if (a == 55)
            i = 7;
        if (a == 56)
            i = 8;
        if (a == 57)
            i = 9;


        if (!game.commands(c, arr[1], arr[2], i))
        {
            cout << "\n Invalid Command \n";
        }
        else
            game.IncrementMove();

        if (game.isGameWon()) 
        {
            cout << hue:: on_purple << "\n\nCongratulations! You won the game." << hue::black_on_light_green << endl << endl;
            break;
        }
    }

	return 0;
}