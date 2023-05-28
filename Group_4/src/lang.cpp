#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cmath>

using namespace std;

struct WordData {
    int hits;
    int fails;
    double prob;
    char nextSymb;
    int num_consecutive_fails;
};

double calculateCompressionBits(const string& riFile, const string& tFile, int wordLength, double alpha, double minProb, int maxNumConsFails, double literalCost) {
    
    ifstream riInput(riFile);
    if (!riInput.is_open()) {
        cerr << "Error opening ri file: " << riFile << endl;
        return -1;
    }

    ifstream tInput(tFile);
    if (!tInput.is_open()) {
        cerr << "Error opening t file: " << tFile << endl;
        return -1;
    }

    string riText((istreambuf_iterator<char>(riInput)), (istreambuf_iterator<char>()));
    string tText((istreambuf_iterator<char>(tInput)), (istreambuf_iterator<char>()));

    unordered_map<string, WordData> wordMap;

    int len = riText.length();

    for (int i = 0; i <= len - wordLength; i++) {
        string word = riText.substr(i, wordLength);

        if (wordMap.find(word) == wordMap.end()) {
            wordMap[word] = {0, 0, 0.5, riText[i + wordLength]};
        } else {
            if (riText[i + wordLength] == wordMap[word].nextSymb) {
                wordMap[word].hits++;
                wordMap[word].num_consecutive_fails = 0;
            } else {
                wordMap[word].fails++;
                wordMap[word].num_consecutive_fails++;
            }

            if (wordMap[word].num_consecutive_fails >= maxNumConsFails || wordMap[word].prob < minProb) {
                wordMap[word].nextSymb = riText[i + wordLength];
                if (wordMap[word].num_consecutive_fails >= maxNumConsFails) {
                    wordMap[word].num_consecutive_fails = 0;
                }
            } else {
                wordMap[word].prob = static_cast<double>(wordMap[word].hits + alpha) / (wordMap[word].hits + wordMap[word].fails + 2 * alpha);
                wordMap[word].nextSymb = riText[i + wordLength];
            }
        }
    }

    double totalBits = 0.0;

    for (auto it = wordMap.begin(); it != wordMap.end(); it++) {
        totalBits += -log2(it->second.prob);
    }

    for (char c : tText) {
        string word = tText.substr(0, wordLength);
        tText.erase(0, 1);

        if (wordMap.find(word) != wordMap.end()) {
            totalBits += -log2(wordMap[word].prob);
        } else {
            totalBits += literalCost;
        }
    }

    return totalBits;
}

int main(int argc, char* argv[]) {

    string help_text =
      "Usage:\n"
      "  ./lang filename_ri filename_t word_length alpha min_prob max_num_cons_fails literal_cost\n"
      "Required:\n"
      "  filename_ri    The name of the file with a text representing the class ri\n"
      "  filename_t     The name of the file with the text under analysis\n"
      "  word_length    The length of a word to consider\n"
      "  alpha          The threshold value used to calculate the probability associated to each word\n"
      "  min_prob       The minimum probability value\n"
      "  max_num_cons_fails     The maximum number of consecutive failures\n"   
      "  literal_cost   Cost of encoding a symbol literally\n"          
      "Example:\n"
      "  ./lang filename1.txt filename2.txt 4 0.5 0.4 4 8.0\n";

    if (argc != 8) {
        printf("ERR: Incorrect number of arguments\n\n%s", help_text.c_str());
        exit(1);
    }

    string riFile = argv[1];
    string tFile = argv[2];
    int wordLength = stoi(argv[3]);
    double alpha = stod(argv[4]);
    double minProb = stod(argv[5]);
    int maxNumConsFails = stoi(argv[6]);
    double literalCost = stod(argv[7]);

    // Calculate the estimated number of bits required to compress t using the model computed from ri
    double compressionBits = calculateCompressionBits(riFile, tFile, wordLength, alpha, minProb, maxNumConsFails, literalCost);

    if (compressionBits >= 0) {
        cout << "Estimated number of bits required to compress t: " << compressionBits << " bits" << endl;
    }

    return 0;
}