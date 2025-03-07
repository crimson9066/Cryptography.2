#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

// English letter frequency (most to least common)
const string ENGLISH_FREQ = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

// Global trigram frequency map
map<string, double> TRIGRAM_FREQ;

// Function to load trigram frequencies from a file
void loadTrigramFrequencies(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open trigram file.\n";
        exit(1);
    }

    string trigram;
    double frequency;
    while (file >> trigram >> frequency) {
        TRIGRAM_FREQ[trigram] = frequency;
    }
}

// Function to generate a random substitution key
string generateRandomKey() {
    string key = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    random_shuffle(key.begin(), key.end());
    return key;
}

// Function to decrypt text using a substitution key
string decryptText(const string& text, const string& key) {
    string decrypted;
    for (char c : text) {
        if (isalpha(c)) {
            decrypted += key[toupper(c) - 'A'];
        } else {
            decrypted += c; // Preserve spaces and punctuation
        }
    }
    return decrypted;
}

// Function to score the decrypted text using trigram frequencies
double scoreText(const string& text) {
    double score = 0.0;
    for (size_t i = 0; i < text.length() - 2; i++) {
        string trigram = text.substr(i, 3);
        if (TRIGRAM_FREQ.find(trigram) != TRIGRAM_FREQ.end()) {
            score += TRIGRAM_FREQ[trigram];
        }
    }
    return score;
}

// Function to perform hill climbing with simulated annealing
string breakCipher(const string& encryptedText, int maxIterations = 10000, double initialTemp = 10.0) {
    string currentKey = generateRandomKey();
    string bestKey = currentKey;
    double currentScore = scoreText(decryptText(encryptedText, currentKey));
    double bestScore = currentScore;

    for (int i = 0; i < maxIterations; i++) {
        // Cooling schedule: reduce temperature over time
        double temperature = initialTemp / (i + 1);

        // Generate a new key by swapping two random letters
        string newKey = currentKey;
        int pos1 = rand() % 26;
        int pos2 = rand() % 26;
        swap(newKey[pos1], newKey[pos2]);

        // Score the new key
        double newScore = scoreText(decryptText(encryptedText, newKey));

        // Accept the new key if it's better or with a certain probability
        if (newScore > currentScore || (exp((newScore - currentScore) / temperature) > (double)rand() / RAND_MAX)) {
            currentKey = newKey;
            currentScore = newScore;
        }

        // Keep track of the best key found
        if (currentScore > bestScore) {
            bestKey = currentKey;
            bestScore = currentScore;
        }
    }

    return bestKey;
}

// Function to allow user to manually adjust the key
void adjustKey(string& key) {
    char encryptedChar, decryptedChar;
    cout << "Enter the encrypted letter to remap: ";
    cin >> encryptedChar;
    cout << "Enter the new decrypted letter: ";
    cin >> decryptedChar;

    encryptedChar = toupper(encryptedChar);
    decryptedChar = toupper(decryptedChar);

    // Find and update the mapping
    size_t pos = key.find(decryptedChar);
    if (pos != string::npos) {
        key[pos] = key[encryptedChar - 'A'];
        key[encryptedChar - 'A'] = decryptedChar;
    }

    cout << "Updated Key: " << key << "\n";
}

int main() {
    srand(time(0)); // Seed the random number generator

    // Load trigram frequencies
    loadTrigramFrequencies("trigrams.txt");

    string encryptedText;
    cout << "Enter the encrypted text: ";
    getline(cin, encryptedText);

    // Break the cipher using hill climbing with simulated annealing
    string bestKey = breakCipher(encryptedText);
    string decryptedText = decryptText(encryptedText, bestKey);

    cout << "\nBest Key Found: " << bestKey << "\n";
    cout << "Decrypted Text:\n" << decryptedText << "\n";

    // Allow user to adjust the key
    string choice;
    while (true) {
        cout << "\nDo you want to adjust the key? (yes/no): ";
        cin >> choice;
        if (choice != "yes") break;

        adjustKey(bestKey);
        decryptedText = decryptText(encryptedText, bestKey);
        cout << "\nUpdated Decrypted Text:\n" << decryptedText << "\n";
    }

    return 0;
}
