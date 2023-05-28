#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

struct WordData {
    int hits;
    int fails;
    double prob;
    char nextSymb;
    int num_consecutive_fails;
};

struct LanguageModel {
    string language;
    unordered_map<string, WordData> wordMap;
    double totalBits;
};

LanguageModel buildLanguageModel(const string& riFile, const string& language, int wordLength, double alpha, double minProb, int maxNumConsFails) {

    ifstream riInput(riFile);
    if (!riInput.is_open()) {
        cerr << "Error opening ri file: " << riFile << endl;
        return LanguageModel();
    }

    string riText((istreambuf_iterator<char>(riInput)), (istreambuf_iterator<char>()));

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

    for (const auto& entry : wordMap) {
        totalBits += -log2(entry.second.prob);
    }

    return {language, wordMap, totalBits};
}

string recognizeLanguage(const string& tFile, const vector<LanguageModel>& languageModels, int wordLength, double literalCost) {

    ifstream tInput(tFile);
    if (!tInput.is_open()) {
        cerr << "Error opening t file: " << tFile << endl;
        return "";
    }

    string tText((istreambuf_iterator<char>(tInput)), (istreambuf_iterator<char>()));

    vector<pair<string, double>> languageScores;

    for (const auto& model : languageModels) {
        double totalBits = model.totalBits;
        string recognizedLanguage = model.language;

        string tCopy = tText;
        transform(tCopy.begin(), tCopy.end(), tCopy.begin(), ::tolower);

        for (size_t i = 0; i <= tCopy.length() - wordLength; i++) {
            string word = tCopy.substr(i, wordLength);

            auto it = model.wordMap.find(word);
            if (it != model.wordMap.end()) {
                totalBits += -log2(it->second.prob);
            } else {
                totalBits += literalCost;
            }
        }

        languageScores.emplace_back(recognizedLanguage, totalBits);
    }

    auto minScore = min_element(languageScores.begin(), languageScores.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    });

    return minScore != languageScores.end() ? minScore->first : "";
}

int main(int argc, char *argv[]) {

    string help_text =
      "Usage:\n"
      "  ./findlang ri_directory filename_t word_length alpha min_prob max_num_cons_fails literal_cost\n"
      "Required:\n"
      "  ri_directory   The path to the directory containing the language examples\n"
      "  filename_t     The name of the file with the text under analysis\n"
      "  word_length    The length of a word to consider\n"
      "  alpha          The threshold value used to calculate the probability associated to each word\n"
      "  min_prob       The minimum probability value\n"
      "  max_num_cons_fails     The maximum number of consecutive failures\n"    
      "  literal_cost   Cost of encoding a symbol literally\n"        
      "Example:\n"
      "  ./findlang /examples filename2.txt 4 0.5 0.4 4 8.0\n";

    if (argc != 8) {
        printf("ERR: Incorrect number of arguments\n\n%s", help_text.c_str());
        exit(1);
    }

    string riDirectory = argv[1];
    string tFile = argv[2];
    int wordLength = stoi(argv[3]);
    double alpha = stod(argv[4]);
    double minProb = stod(argv[5]);
    int maxNumConsFails = stoi(argv[6]);
    double literalCost = stod(argv[7]);

    vector<LanguageModel> languageModels;

    ifstream languageList("language_list.txt");
    if (!languageList.is_open()) {
        cerr << "Error opening language list file." << endl;
        return 1;
    }

    string language;
    while (getline(languageList, language)) {
        string riFile = riDirectory + "/" + language + ".txt";
        LanguageModel model = buildLanguageModel(riFile, language, wordLength, alpha, minProb, maxNumConsFails);
        languageModels.push_back(model);
    }

    string recognizedLanguage = recognizeLanguage(tFile, languageModels, wordLength, literalCost);

    if (!recognizedLanguage.empty()) {
        cout << "The text is most likely written in " << recognizedLanguage << " language." << endl;
    } else {
        cout << "Language recognition failed." << endl;
    }

    return 0;
}