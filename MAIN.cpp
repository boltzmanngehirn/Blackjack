#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

class Card {
public:
    int value;
    string suit;

    Card(int val, string st) : value(val), suit(st) {}
};

class Deck {
    vector<Card> cards;
public:
    Deck() {
        string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
        for (const string &suit : suits) {
            for (int i = 1; i <= 13; ++i) {
                cards.emplace_back(i, suit);
            }
        }
        // Use std::shuffle with a random engine
        random_device rd;
        mt19937 g(rd());
        shuffle(cards.begin(), cards.end(), g);
    }

    Card drawCard() {
        Card drawn = cards.back();
        cards.pop_back();
        return drawn;
    }
};

int getCardValue(const Card &card) {
    if (card.value > 10) return 10; // Face cards are 10
    if (card.value == 1) return 11; // Ace is initially treated as 11
    return card.value;
}

void printHand(const vector<Card>& hand, const string& who) {
    cout << who << " hand: ";
    for (const Card &card : hand) {
        cout << card.value << " of " << card.suit << ", ";
    }
    cout << "\n";
}

int calculateHandTotal(const vector<Card>& hand) {
    int total = 0;
    int aceCount = 0;

    for (const Card &card : hand) {
        total += getCardValue(card);
        if (card.value == 1) aceCount++;
    }

    while (total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }

    return total;
}

int main() {
    double playerMoney = 500;
    char continuePlaying = 'y';

    while (continuePlaying == 'y' && playerMoney > 0) {
        Deck deck;
        vector<Card> playerHand;
        vector<Card> dealerHand;
        double bet;

        cout << "You have $" << playerMoney << ". Enter your bet: ";
        cin >> bet;

        if (bet > playerMoney) {
            cout << "You don't have enough money to bet that amount.\n";
            continue;
        }

        playerHand.push_back(deck.drawCard());
        playerHand.push_back(deck.drawCard());
        dealerHand.push_back(deck.drawCard());
        dealerHand.push_back(deck.drawCard());

        printHand(playerHand, "Your");
        cout << "Dealer shows: " << dealerHand[0].value << " of " << dealerHand[0].suit << "\n";

        // Player's turn
        char action;
        while (true) {
            int playerTotal = calculateHandTotal(playerHand);
            cout << "Your total: " << playerTotal << "\n";

            if (playerTotal > 21) {
                cout << "You busted!\n";
                playerMoney -= bet;
                break;
            }

            cout << "(h)it or (s)tand? ";
            cin >> action;

            if (action == 'h') {
                playerHand.push_back(deck.drawCard());
                printHand(playerHand, "Your");
            } else if (action == 's') {
                break;
            } else {
                cout << "Invalid action. Please enter 'h' or 's'.\n";
            }
        }

        // Dealer's turn
        if (action == 's') {
            int dealerTotal = calculateHandTotal(dealerHand);
            while (dealerTotal < 17) {
                dealerHand.push_back(deck.drawCard());
                dealerTotal = calculateHandTotal(dealerHand);
            }

            printHand(dealerHand, "Dealer's");
            cout << "Dealer's total: " << dealerTotal << "\n";

            int playerTotal = calculateHandTotal(playerHand);
            if (dealerTotal > 21 || dealerTotal < playerTotal) {
                cout << "You win!\n";
                playerMoney += bet;
            } else if (dealerTotal == playerTotal) {
                cout << "It's a tie!\n";
            } else {
                cout << "You lose!\n";
                playerMoney -= bet;
            }
        }

        if (playerMoney <= 0) {
            cout << "You are out of money. Game over.\n";
            break;
        }

        cout << "Play again? (y/n): ";
        cin >> continuePlaying;
    }

    cout << "You leave with $" << playerMoney << ".\n";
    return 0;
}
