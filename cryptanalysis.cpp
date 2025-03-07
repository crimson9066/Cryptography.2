#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

// English letter frequency (approximate percentage in normal text)
const string ENGLISH_FREQ = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
const vector<string> COMMON_TWO_LETTER_WORDS = {"AN", "AS", "AT", "BE", "BY", "DO", "GO", "HE", "IF", "IN", "IS", "IT", "ME", "MY", "NO", "OF", "ON", "OR", "SO", "TO", "UP", "WE"};
const vector<string> COMMON_THREE_LETTER_WORDS = {"AND", "ARE", "BUT", "CAN", "DID", "FOR", "GET", "HAD", "HER", "HIM", "HIS", "HOW", "ITS", "LET", "MAN", "NOT", "NOW", "OFF", "OUR", "OUT", "SAY", "SEE", "SHE", "THE", "USE", "WAS", "WAY", "WHO", "WHY", "YES", "YOU"};

// Function to analyze letter frequency in encrypted text
map<char, int> analyzeFrequency(const string& text) {
    map<char, int> freq;
    for (char c : text) {
        if (isalpha(c)) {
            freq[toupper(c)]++;
        }
    }
    return freq;
}

// Function to identify spaces and separate words
vector<string> extractWords(const string& text) {
    vector<string> words;
    istringstream stream(text);
    string word;
    while (stream >> word) {
        words.push_back(word);
    }
    return words;
}

// Function to generate an initial substitution mapping based on frequency analysis
map<char, char> generateMapping(map<char, int> freq) {
    vector<pair<char, int>> sortedFreq(freq.begin(), freq.end());
    sort(sortedFreq.begin(), sortedFreq.end(), [](auto& a, auto& b) { return a.second > b.second; });
    
    map<char, char> substitution;
    for (size_t i = 0; i < sortedFreq.size() && i < ENGLISH_FREQ.size(); i++) {
        substitution[sortedFreq[i].first] = ENGLISH_FREQ[i];
    }
    return substitution;
}

// Allow manual adjustments to the mapping
void adjustMapping(map<char, char>& substitution) {
    char original, newLetter;
    string choice;
    
    while (true) {
        cout << "\nDo you want to adjust any mappings? (yes/no): ";
        cin >> choice;
        if (choice != "yes") break;
        
        cout << "Enter the encrypted letter to change: ";
        cin >> original;
        cout << "Enter the new decrypted letter: ";
        cin >> newLetter;
        
        original = toupper(original);
        newLetter = toupper(newLetter);
        
        substitution[original] = newLetter;
        
        cout << "Updated mapping: " << original << " -> " << newLetter << endl;
    }
}

// Function to decrypt text using the mapping
string decryptText(const string& text, const map<char, char>& substitution) {
    string decrypted;
    for (char c : text) {
        if (isalpha(c)) {
            decrypted += substitution.at(toupper(c));
        } else {
            decrypted += c; // Preserve spaces and punctuation
        }
    }
    return decrypted;
}

// Function to save the decrypted text to a file
void saveToFile(const string& decryptedText) {
    ofstream outFile("decrypted_output.txt");
    if (outFile.is_open()) {
        outFile << "Decryption Output:\n";
        outFile << decryptedText << "\n";
        outFile.close();
        cout << "\nDecrypted text saved to 'decrypted_output.txt'!\n";
    } else {
        cout << "\nError: Could not open file for writing.\n";
    }
}

int main() {
    string encryptedText;
    cout << "Enter the encrypted text: ";
    getline(cin, encryptedText);
    
    map<char, int> frequency = analyzeFrequency(encryptedText);
    map<char, char> substitution = generateMapping(frequency);
    
    cout << "\nInitial letter mappings based on frequency analysis:\n";
    for (const auto& pair : substitution) {
        cout << pair.first << " -> " << pair.second << endl;
    }
    
    adjustMapping(substitution);
    
    string decryptedText = decryptText(encryptedText, substitution);
    vector<string> words = extractWords(decryptedText);
    
    cout << "\nDecrypted Text:\n" << decryptedText << "\n";
    
    cout << "\nIdentified Words (for further analysis):\n";
    for (const string& word : words) {
        cout << word << " ";
    }
    cout << "\n";
    
    saveToFile(decryptedText);
    return 0;
}
