#pragma once
#include <iostream>
#include <string>
using namespace std;

const char* hearts = "\x03";  //♥
const char* diamonds = "\x04";  //♦
const char* clubs = "\x05";  //♣
const char* spades = "\x06";  //♠

class Card {
public:
    string rank;
    const char* suit;
    string color;
    bool hidden;

    Card(string r = "A", const char* s = spades)
    {
        rank = r;
        suit = s;
        hidden = true;
        if (suit == hearts || suit == diamonds)
            color = "red";
        else
            color = "black";
    }

    // move constructor
    Card(Card&& other) 
    {
        rank = move(other.rank);
        suit = other.suit;
        color = move(other.color);
        hidden = other.hidden;

        other.suit = nullptr;  // Nullify the pointer of the object that had member variables moved from 
    }

    //move assignment operator 
    Card& operator = (Card&& other)
    {
        if (this != &other) {
            rank = move(other.rank);     
            suit = other.suit;               
            color = move(other.color);   
            hidden = other.hidden;            

            other.suit = nullptr;  // Nullify the moved-from object's suit
        }
        return *this;
    }

    //void display()
    friend ostream& operator << (ostream& out, const Card& obj)
    {
        if (obj.rank != "?")
        {
            if (obj.hidden == false)
            {
                if (obj.rank != "10")
                {
                    if (obj.color == "red")
                    {
                        cout << hue::red_on_bright_white << "|" << obj.suit << "  " << obj.rank << "|";
                        cout << hue::on_light_green;
                    }

                    else
                    {
                        cout << hue::black_on_bright_white << "|" << obj.suit << "  " << obj.rank << "|";
                        cout << hue::on_light_green;
                    }
                }

                else
                {
                    if (obj.color == "red")
                    {
                        cout << hue::red_on_bright_white << "|" << obj.suit << " " << obj.rank << "|";
                        cout << hue::on_light_green;
                    }

                    else
                    {
                        cout << hue::black_on_bright_white << "|" << obj.suit << " " << obj.rank << "|";
                        cout << hue::on_light_green;
                    }
                }
            }
            else
            {
                cout << hue::bright_white_on_blue << "|" << "  " << diamonds << " " << "|";
                cout << hue::on_light_green;
            }
        }
        else
        {
            cout << hue::black << "|" << "  " << hue:: bright_white << obj.suit << " " << hue::black << "|";
        }
        return out;
    }
};