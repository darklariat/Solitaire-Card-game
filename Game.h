#pragma once
#include <iostream>
#include <random>
#include "Card.h"
#include "list.h"
#include "Stack.h"
using namespace std;

class Command {
public:
    char type;
    string source;
    string dest;
    int count;

    Command(char t = 's', string s = "", string d = "", int c = 0)
    {
        type = t;
        source = s;
        dest = d;
        count = c;
    }
};

class Game
{
private:
    Dlist <Card*> tableau[7];             // 7 tableau columns
    Stack <Card*> foundation[4];          // 4 foundation piles
    Stack <Card*> stockpile;             
    Stack <Card*> wastepile;              
    Stack <Command*> undo;                 // for undo operations
    Dlist <Card*> deck;                   // original deck of cards
    Stack <Card*> lastFoundation;         // keep track of cards moved to foundationpile from wastepile
    int moveCount;

public:
    int get_moveCount() { return moveCount; }

    void shuffleDeck()
    {
        const char* suits[] = { hearts, diamonds, clubs, spades };
        string ranks[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

        Card* arr[52];
        int k = 0;
        for (int i = 0; i < 4; i++) 
        {
            for (int j = 0; j < 13; j++) 
            {
                arr[k] = new Card(ranks[j], suits[i]);
                k++;
            }
        }

        std::srand(static_cast<unsigned>(std::time(0))); 

        for (int i = 0; i < 52; ++i) 
        {
            // Generate a random index
            int randomIndex = rand() % 52;

            // Swap the current card with the card at randomIndex
            swap(arr[i], arr[randomIndex]);
        }

        int i = 0;
        while (i < 52)
        {
            deck.InsertAtStart(arr[i]);
            i++;
        }
    }

    void dealCards()
    {
        for (int i = 0; i < 7; i++) 
        {
            for (int j = 0; j <= i; j++)
            {
                tableau[i].InsertAtEnd(move(deck.DeleteEnd()));
                if (j == 0)
                {
                    Dlist <Card*> ::Iterator temp = tableau[i].begin();
                    Card* temp1 = temp.curr->data;
                    temp1->hidden = false;
                }
            }
        }

        // Remaining cards go into the stockpile
        while (!deck.isEmpty()) 
        {
            stockpile.push(move(deck.DeleteEnd()));
            Dlist <Card*> ::Iterator temp = stockpile.list.begin();
            Card* temp1 = temp.curr->data;
        }

        //put placeholder cards in foundation piles
        foundation[0].push(new Card("?", hearts));
        foundation[1].push(new Card("?", diamonds));
        foundation[2].push(new Card("?", clubs));
        foundation[3].push(new Card("?", spades));
    }

    void printTableau()
    {
        // Find the maximum height of the columns
        int maxHeight = 0;
        for (int i = 0; i < 7; ++i) 
        {
            int columnHeight = tableau[i].size();
            if (columnHeight > maxHeight) 
            {
                maxHeight = columnHeight;
            }
        }

        cout << "  C1             C2             C3             C4             C5             C6             C7\n";
        // Print row by row
        for (int row = 0; row < maxHeight; ++row) 
        {
            for (int col = 0; col < 7; ++col) 
            {
                cout << "  ";
                Dlist<Card*>::Iterator it = tableau[col].end();
                it--;
                int columnHeight = tableau[col].size();

                if (row < columnHeight) 
                {
                    // Traverse to the 'row'-th card in this column
                    for (int i = 0; i < row; ++i) {
                        it--;  
                    }

                    Card* card = *it;
                    cout << *card << "       ";
                }
                else 
                {
                    cout << "             ";
                }
            }
            cout << endl;  
        }
    }

    void printInstructions()
    {
        cout << hue:: black_on_light_green << " \n " << hue::bright_white_on_light_blue << " Enter Z" << hue::black_on_light_green << " to undo a previous move\n";
        cout << hue::black_on_light_green << " " << hue::bright_white_on_light_blue << " Enter M" << hue::black_on_light_green << " followed by source, destination and number of ";
        cout << "cards to move that many cards \n to the destination. Possible moves: \n";
        cout << "\t\t\t\t" << hue::bright_white << "Source: w(wastepile), cn(column number n)" << hue:: black << "\n";
        cout << "\t\t\t\t" << hue::bright_white << "Destinations : f(foundation pile), cn" << hue:: black;
        cout << hue::black_on_light_green << " \n " << hue::bright_white_on_light_blue << " Enter S" << hue::black_on_light_green << " to move a card from stockpile to wastepile.\n\n";

    }

    //Display the game state
    void display()
    {
        for (int i = 0; i < 30; i++)
        {
            cout << hue::red << hearts << diamonds;
            cout << hue::black << spades << clubs;
        }
        cout << endl << endl;

        cout << "  Move Num: " << moveCount << "\t\t\t" << hue::bright_white_on_light_blue << "Possible moves: " << calculateMoves() << hue::black_on_light_green;
        cout << "\n  Stockpile[" << stockpile.countElem()<< "]" << "       Wastepile[" << wastepile.countElem() << "]" << "              Foundation piles" << endl; 
        cout << "  ";
        stockpile.display();
        cout << "               ";
        wastepile.display();
        cout << "               ";
        for (int i = 0; i < 4; i++)
        {
            foundation[i].display();
            cout << "  ";
        }
        cout << endl << endl;

        cout << "  ";
        printTableau();
        cout << endl << endl << endl;
    }

    Game() 
    {
        shuffleDeck();
        dealCards();
        moveCount = 1;
    }

    bool checkRankColor(int source, int dest, int count)
    {
        if (source == 48)
            source = 0;
        if (source == 49)
            source = 1;
        if (source == 50)
            source = 2;
        if (source == 51)
            source = 3;
        if (source == 52)
            source = 4;
        if (source == 53)
            source = 5;
        if (source == 54)
            source = 6;
        if (source == 55)
            source = 7;
        if (source == 56)
            source = 8;
        if (source == 57)
            source = 9;

        if (dest == 48)
            dest = 1;
        if (dest == 49)
            dest = 1;
        if (dest == 50)
            dest = 2;
        if (dest == 51)
            dest = 3;
        if (dest == 52)
            dest = 4;
        if (dest == 53)
            dest = 5;
        if (dest == 54)
            dest = 6;
        if (dest == 55)
            dest = 7;
        if (dest == 56)
            dest = 8;
        if (dest == 57)
            dest = 9;

        Dlist<Card*>::Iterator it1;
        Card* s;
        if (source != 0)
        {
            it1 = tableau[source - 1].begin();
            for (int i = 0; i < count - 1; i++)
            {
                if (it1.curr-> data != nullptr)
                {
                    it1++;
                }
                else
                {
                    return false;
                }
            }

            if (it1.curr != nullptr)
            {
                s = it1.curr->data;
                if (s == nullptr)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }

            if (s != nullptr)
            {
                if (s->hidden == true)
                {
                    return false;
                }
            }
        }
        else
        {
            it1 = wastepile.list.begin();
            if (it1.curr->data == nullptr)
            {
                return false;
            }
            s = it1.curr->data;
        }

        bool check = 1;
        Dlist<Card*>::Iterator it2;
        Card* d;
        if (dest != 0)
        {
            it2 = tableau[dest - 1].begin();
            d = it2.curr->data;
        }
        else
        {
            if (s == nullptr)
            {
                return false;
            }
            const char* suit = s->suit;
            if(suit == hearts)
                it2 = foundation[0].list.begin();
            else if (suit == diamonds)
                it2 = foundation[1].list.begin();
            else if (suit == clubs)
                it2 = foundation[2].list.begin();
            else if (suit == spades)
                it2 = foundation[3].list.begin();

            d = it2.curr->data;
        }

        // assigning value to ranks for comparison
        string ranks[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
        int srank = 0, drank = 0;
        for (int i = 0; i < 13; i++)
        {
            if (s->rank == ranks[i])
            {
                srank = i;
            }
            if (d != nullptr)
            {
                if (d->rank == ranks[i])
                {
                    drank = i;
                }
            }
            else
            {
                return true;
            }
            if (d->rank == "?")
            {
                drank = -1;
            }
        }

        //comparing ranks and colors
        if (dest != 0)
        {
            if (srank < drank && (srank + 1 == drank))
            {
                if (s->color == "red" && d->color == "black")
                {
                    return true;
                }
                else if (s->color == "black" && d->color == "red")
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        else
        {
            if (srank > drank && (drank + 1 == srank))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    bool movecheck(string source, string dest, int count)
    {
        if (source != "w")
        {
            if (source.length() != 2)
            {
                return false;
            }
            else
            {
                if (tolower(source[0]) != 'c')
                {
                    return false;
                }
                else
                {
                    if (source[1] < 1 && source[1] > 7)
                    {
                        return false;
                    }
                }
            }

            if (dest != "f")
            {
                if (dest.length() != 2)
                {
                    return false;
                }
                else
                {
                    if (tolower(dest[0]) != 'c')
                    {
                        return false;
                    }
                    else
                    {
                        if (dest[1] < 1 && dest[1] > 7)
                        {
                            return false;
                        }
                    }
                }

                //tableau to tableau
                return checkRankColor(source[1], dest[1], count);
            }
            else
            {
                //tableau to foundation
                return checkRankColor(source[1], 0, count);
            }
        }
        else   // source is wastepile
        {
            if (dest != "f")
            {
                if (dest.length() != 2)
                {
                    return false;
                }
                else
                {
                    if (tolower(dest[0]) != 'c')
                    {
                        return false;
                    }
                    else
                    {
                        if (dest[1] < 1 && dest[1] > 7)
                        {
                            return false;
                        }
                    }
                }

                //wastepile to tableau
                return checkRankColor(0, dest[1], count);
            }
            else
            {
                //wastepile to foundation
                return checkRankColor(0, 0, count);
            }
        }
    }

    void movecomm(string source, string dest, int count)
    {
        if (source[1] != '\0')
        {
            if (source[1] == 48)
                source[1] = 0;
            if (source[1] == 49)
                source[1] = 1;
            if (source[1] == 50)
                source[1] = 2;
            if (source[1] == 51)
                source[1] = 3;
            if (source[1] == 52)
                source[1] = 4;
            if (source[1] == 53)
                source[1] = 5;
            if (source[1] == 54)
                source[1] = 6;
            if (source[1] == 55)
                source[1] = 7;
            if (source[1] == 56)
                source[1] = 8;
            if (source[1] == 57)
                source[1] = 9;
        }
        if(dest[1] != '\0')
        {
            if (dest[1] == 48)
                dest[1] = 1;
            if (dest[1] == 49)
                dest[1] = 1;
            if (dest[1] == 50)
                dest[1] = 2;
            if (dest[1] == 51)
                dest[1] = 3;
            if (dest[1] == 52)
                dest[1] = 4;
            if (dest[1] == 53)
                dest[1] = 5;
            if (dest[1] == 54)
                dest[1] = 6;
            if (dest[1] == 55)
                dest[1] = 7;
            if (dest[1] == 56)
                dest[1] = 8;
            if (dest[1] == 57)
                dest[1] = 9;
        }

        if (tolower(source[0]) == 'c' && tolower(dest[0] == 'c')) // tableau to tableau
        {
            Stack <Card*> temp;
            for (int i = 0; i < count; i++)
            {
                temp.push(move(tableau[source[1] - 1].DeleteStart()));
            }

            for (int i = 0; i < count; i++)
            {
                tableau[dest[1] - 1].InsertAtStart(move(temp.pop()));
            }

            //revealing hidden card
            Dlist<Card*>::Iterator it1 = tableau[source[1] - 1].begin();
            if (it1.curr != nullptr)
            { 
                if (it1.curr->data != nullptr)
                {

                    if (it1.curr->data->hidden == true)
                    {
                        it1.curr->data->hidden = false;
                    }
                }
            }

            undo.push(new Command('m', source, dest, count));
        }
        else if(source[0] == 'c' && dest == "f")  // tableau to foundation
        {
            Dlist<Card*>::Iterator it1;
            Card* s;

            it1 = tableau[source[1] - 1].begin();
            s = it1.curr->data;
        
            lastFoundation.push(move(s));

            const char* suit = s->suit;
            if (suit == hearts)
                foundation[0].push(move(tableau[source[1] - 1].DeleteStart()));
            else if (suit == diamonds)
                foundation[1].push(move(tableau[source[1] - 1].DeleteStart()));
            else if (suit == clubs)
                foundation[2].push(move(tableau[source[1] - 1].DeleteStart()));
            else if (suit == spades)
                foundation[3].push(move(tableau[source[1] - 1].DeleteStart()));

            Dlist<Card*>::Iterator it2 = tableau[source[1] - 1].begin();
            if (it2.curr->data != nullptr)
            {
                if (it2.curr->data->hidden == true)
                {
                    it2.curr->data->hidden = false;
                }
            }

            undo.push(new Command('m', source, "f", 1));
        }
        else if(source == "w" && dest[0] == 'c') // wastepile to tableau
        {
            tableau[dest[1] - 1].InsertAtStart(move(wastepile.pop()));
            undo.push(new Command('m', "w", dest, 1));
        }
        else   // wastepile to foundation pile
        {
            Card* s;

            s = wastepile.revealTop();
            lastFoundation.push(move(s));
            
            const char* suit = s->suit;
            if (suit == hearts)
                foundation[0].push(move(wastepile.pop()));
            else if (suit == diamonds)
                foundation[1].push(move(wastepile.pop()));
            else if (suit == clubs)
                foundation[2].push(move(wastepile.pop()));
            else if (suit == spades)
                foundation[3].push(move(wastepile.pop()));

            undo.push(new Command('m', "w", "f", 1));
        }
    }

    // Draw a card from the stockpile to the wastepile
    void stockcomm()
    {
        if(stockpile.isEmpty())
        {
            while (!wastepile.isEmpty())
            {
                stockpile.push(move(wastepile.pop()));
                Dlist<Card*>::Iterator it = stockpile.list.begin();
                it.curr->data->hidden = true;
            }
        }
        wastepile.push(move(stockpile.pop()));
        Dlist<Card*>::Iterator it = wastepile.list.begin();
        it.curr->data->hidden = false;

        undo.push(move(new Command('s', "a", "a", 0)));
    }

    void revMovecomm(string source, string dest, int count)
    {
        if (source[1] != '\0')
        {
            if (source[1] == 48)
                source[1] = 0;
            if (source[1] == 49)
                source[1] = 1;
            if (source[1] == 50)
                source[1] = 2;
            if (source[1] == 51)
                source[1] = 3;
            if (source[1] == 52)
                source[1] = 4;
            if (source[1] == 53)
                source[1] = 5;
            if (source[1] == 54)
                source[1] = 6;
            if (source[1] == 55)
                source[1] = 7;
            if (source[1] == 56)
                source[1] = 8;
            if (source[1] == 57)
                source[1] = 9;
        }
        if (dest[1] != '\0')
        {
            if (dest[1] == 48)
                dest[1] = 1;
            if (dest[1] == 49)
                dest[1] = 1;
            if (dest[1] == 50)
                dest[1] = 2;
            if (dest[1] == 51)
                dest[1] = 3;
            if (dest[1] == 52)
                dest[1] = 4;
            if (dest[1] == 53)
                dest[1] = 5;
            if (dest[1] == 54)
                dest[1] = 6;
            if (dest[1] == 55)
                dest[1] = 7;
            if (dest[1] == 56)
                dest[1] = 8;
            if (dest[1] == 57)
                dest[1] = 9;
        }

        if (tolower(source[0]) == 'c' && tolower(dest[0] == 'c')) // tableau to tableau
        {
            Stack <Card*> temp;
            for (int i = 0; i < count; i++)
            {
                temp.push(move(tableau[source[1] - 1].DeleteStart()));
            }

            for (int i = 0; i < count; i++)
            {
                tableau[dest[1] - 1].InsertAtStart(move(temp.pop()));
            }
        }
        else if (source == "f" && dest[0] == 'c')  // foundation to tableau
        {
            Card* d;
            d = lastFoundation.pop();

            const char* suit = d->suit;
            if (suit == hearts)
                tableau[dest[1] - 1].InsertAtStart(move(foundation[0].pop()));
            else if (suit == diamonds)
                tableau[dest[1] - 1].InsertAtStart(move(foundation[1].pop()));
            else if (suit == clubs)
                tableau[dest[1] - 1].InsertAtStart(move(foundation[2].pop()));
            else if (suit == spades)
                tableau[dest[1] - 1].InsertAtStart(move(foundation[3].pop()));

        }
        else if (source[0] == 'c' && dest == "w") // tableau to wastepile
        {
            wastepile.push(move(tableau[source[1] - 1].DeleteStart()));
        }
        else   // foundation pile to wastepile
        {
            Card* s;
            s = lastFoundation.pop();

            const char* suit = s->suit;
            if (suit == hearts)
                wastepile.push(move(foundation[0].pop()));
            else if (suit == diamonds)
                wastepile.push(move(foundation[1].pop()));
            else if (suit == clubs)
                wastepile.push(move(foundation[2].pop()));
            else if (suit == spades)
                wastepile.push(move(foundation[3].pop()));
        }
    }

    // send card from wastepile back to stockpile
    void revStockcomm()
    {
        stockpile.push(move(wastepile.pop()));
    }

    // Undo the last move or draw
    void Undo()
    {
        if (!undo.isEmpty())
        {
            Command* lastCommand = undo.pop();
            // Implement undo logic based on the last command
            if (lastCommand->type == 'm')
            {
                revMovecomm(lastCommand->dest, lastCommand->source, lastCommand->count);
            }
            else if (lastCommand->type == 's')
            {
                revStockcomm();
            }
        }
    }

    void IncrementMove()
    {
        moveCount++;
    }

    bool commands(char type, string source, string dest, int count) 
    {
        char type1 = tolower(type);
        // checks:
        if (type1 != 'm' && type1 != 'z' && type1 != 's')
        {
            return false;
        }

        if (type1 == 'm')
        {
            if (movecheck(source, dest, count) == false)
                return false;
        }

        // Applying command
        if (type1 == 'm')
        {
            movecomm(source, dest, count);
        }
        else if (type1 == 's')
        {
            stockcomm();
        }
        else // undo
        {
            Undo();
        }

        return true;
    }

    // Check if the game is won
    bool isGameWon() 
    {
        for (int i = 0; i < 4; i++)
        {
            if (foundation[i].revealTop() != nullptr)
            {
                if (foundation[i].revealTop()->rank != "K")
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    int checkpossiblemws()
    {
        int moves = 0;
        Dlist<Card*>::Iterator it1;
        Card* s;
        Dlist<Card*>::Iterator it2;
        Card* w;

        it1 = stockpile.list.begin();
        s = *it1;
        it2 = wastepile.list.begin();
        w = *it2;

        //wastepile to tableau
        if (w != nullptr)
        {
            for (int i = 0; it2 != wastepile.list.end(); it2++)
            {
                for (int j = 0; j < 7; j++)
                {
                    int check = 0;
                    Card* s = it2.curr->data;
                    Card* d = tableau[j].head-> next -> data;
                    // assigning value to ranks for comparison
                    string ranks[13] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
                    int srank = 0, drank = 0;
                    for (int i = 0; i < 13; i++)
                    {
                        if (s->rank == ranks[i])
                        {
                            srank = i;
                        }
                        if (d != nullptr)
                        {
                            if (d->rank == ranks[i])
                            {
                                drank = i;
                            }
                        }
                        else
                        {
                            moves++;
                            check = 1;
                        }
                    }

                    if (srank < drank && (srank + 1 == drank) && check != 1)
                    {
                        moves++;
                    }
                }
            }
        }

        // checking stockpile to tableau
        if (s != nullptr)
        {
            for (int i = 0; it1 != stockpile.list.end(); it1++)
            {
                for (int j = 0; j < 7; j++)
                {
                    int check = 0;
                    Card* s = it1.curr->data;
                    Card* d = tableau[j].head-> next -> data;
                    // assigning value to ranks for comparison
                    string ranks[13] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
                    int srank = 0, drank = 0;
                    for (int i = 0; i < 13; i++)
                    {
                        if (s->rank == ranks[i])
                        {
                            srank = i;
                        }
                        if (d != nullptr)
                        {
                            if (d->rank == ranks[i])
                            {
                                drank = i;
                            }
                        }
                        else
                        {
                            moves++;
                            check = 1;
                        }
                    }

                    if (srank < drank && (srank + 1 == drank) && check != 1)
                    {
                        moves++;
                    }
                }
            }
        }

        // wastepile to foundation
        if (w != nullptr)
        {
            for (int i = 0; it2 != wastepile.list.end(); it2++)
            {
                Card* s = it2.curr->data;
                Card* d;
                const char* suit = s->suit;
                if (suit == hearts)
                    it2 = foundation[0].list.begin();
                else if (suit == diamonds)
                    it2 = foundation[1].list.begin();
                else if (suit == clubs)
                    it2 = foundation[2].list.begin();
                else if (suit == spades)
                    it2 = foundation[3].list.begin();
                d = it2.curr->data;
                    
                // assigning value to ranks for comparison
                string ranks[13] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
                int srank = 0, drank = 0;
                for (int i = 0; i < 13; i++)
                {
                    if (s->rank == ranks[i])
                    {
                        srank = i;
                    }
                    if (d != nullptr)
                    {
                        if (d->rank == ranks[i])
                        {
                            drank = i;
                        }
                    }
                    if (d->rank == "?")
                    {
                        drank = -1;
                    }
                }

                if (srank > drank && (drank + 1 == srank))
                {
                    moves++;
                }
            }
        }

        // stockpile to foundation
        if (s != nullptr)
        {
            for (int i = 0; it1 != stockpile.list.end(); it1++)
            {
                Card* s = it1.curr->data;
                Card* d;
                const char* suit = s->suit;
                if (suit == hearts)
                    it1 = foundation[0].list.begin();
                else if (suit == diamonds)
                    it1 = foundation[1].list.begin();
                else if (suit == clubs)
                    it1 = foundation[2].list.begin();
                else if (suit == spades)
                    it1 = foundation[3].list.begin();
                d = it1.curr->data;

                // assigning value to ranks for comparison
                string ranks[13] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
                int srank = 0, drank = 0;
                for (int i = 0; i < 13; i++)
                {
                    if (s->rank == ranks[i])
                    {
                        srank = i;
                    }
                    if (d != nullptr)
                    {
                        if (d->rank == ranks[i])
                        {
                            drank = i;
                        }
                    }
                    if (d->rank == "?")
                    {
                        drank = -1;
                    }
                }

                if (srank > drank && (drank + 1 == srank))
                {
                    moves++;
                }
            }
        }
        return moves;
    }

    int calculateMoves()
    {
        int moveCount = 0;
        // tableau to tableau moves
        for (int i = 1; i <= 7; i++)
        {
            for (int j = 1; j <= 7; j++)
            {
                for (int k = 1; k < 10; k++)
                {
                    if (checkRankColor(i, j, k) && j != i)
                    {
                        moveCount++;
                    }
                }
            }
        }
        
        //waste & stockpile to foundation and tableau moves
        moveCount += checkpossiblemws();
        
        return moveCount;   
    }
};