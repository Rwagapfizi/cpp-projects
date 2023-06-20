#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// Structure to represent a disease case
struct DiseaseCase {
    string location;
    string disease;
    int cases;
};

// Function to convert string to lowercase
string toLowercase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Function to split a string by delimiter
vector<string> splitString(string str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to add a new location
void addLocation(map<string, vector<DiseaseCase> >& data, string location) {
    data[toLowercase(location)] = vector<DiseaseCase>();
}

// Function to record a new disease case
void recordCase(map<string, vector<DiseaseCase> >& data, string location, string disease, int cases) {
    string lowercaseLocation = toLowercase(location);
    for (auto & entry:data[lowercaseLocation]) {
        if (entry.disease == disease) {
            entry.cases += cases;
            return;
        }
    }
    data[lowercaseLocation].push_back({location, disease, cases});
}

// Function to delete a location
void deleteLocation(map<string, vector<DiseaseCase> >& data, string location) {
    string lowercaseLocation = toLowercase(location);
    data.erase(lowercaseLocation);
}

// Function to list all existing locations
void listLocations(const map<string, vector<DiseaseCase> >& data) {
    for (const auto& entry : data) {
        cout << entry.first << endl;
    }
}

// Function to list all existing diseases
void listDiseases(const map<string, vector<DiseaseCase> >& data) {
    vector<string> diseases;
    for (const auto& entry : data) {
        for (const auto& caseEntry : entry.second) {
            diseases.push_back(caseEntry.disease);
        }
    }
    sort(diseases.begin(), diseases.end());
    diseases.erase(unique(diseases.begin(), diseases.end()), diseases.end());
    for (const auto& disease : diseases) {
        cout << disease << endl;
    }
}

// Function to list all locations with a given disease
void listLocationsWithDisease(const map<string, vector<DiseaseCase> >& data, string disease) {
    string lowercaseDisease = toLowercase(disease);
    for (const auto& entry : data) {
        for (const auto& caseEntry : entry.second) {
            if (toLowercase(caseEntry.disease) == lowercaseDisease) {
                cout << caseEntry.location << endl;
                break;
            }
        }
    }
}

// Function to display total disease cases from all locations
void displayTotalCases(const map<string, vector<DiseaseCase> >& data, string disease) {
    int totalCases = 0;
    string lowercaseDisease = toLowercase(disease);
    for (const auto& entry : data) {
        for (const auto& caseEntry : entry.second) {
            if (toLowercase(caseEntry.disease) == lowercaseDisease) {
                totalCases += caseEntry.cases;
            }
        }
    }
    cout << totalCases << endl;
}

// Function to display disease cases from a given location
void displayCasesFromLocation(const map<string, vector<DiseaseCase> >& data, string location, string disease) {
    string lowercaseLocation = toLowercase(location);
    string lowercaseDisease = toLowercase(disease);
    for (const auto& caseEntry : data[lowercaseLocation]) {
        if (toLowercase(caseEntry.disease) == lowercaseDisease) {
            cout << caseEntry.cases << endl;
            return;
        }
    }
    cout << "0" << endl;
}

// Function to display help for all commands
void displayHelp() {
    cout << "Command List:\n";
    cout << "  Add <location>\n";
    cout << "  Record <location> <disease> <cases>\n";
    cout << "  List\n";
    cout << "  Delete <location>\n";
    cout << "  List\n";
    cout << "  Where <disease>\n";
    cout << "  Cases <disease>\n";
    cout << "  Cases <location> <disease>\n";
    cout << "  Help\n";
    cout << "  Exit\n";
}

// Function to save data to a file
void saveData(const map<string, vector<DiseaseCase> >& data, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& entry : data) {
            for (const auto& caseEntry : entry.second) {
                file << caseEntry.location << "," << caseEntry.disease << "," << caseEntry.cases << endl;
            }
        }
        file.close();
        cout << "Data saved to " << filename << endl;
    } else {
        cout << "Unable to save data to " << filename << endl;
    }
}

// Function to load data from a file
void loadData(map<string, vector<DiseaseCase> >& data, const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            vector<string> tokens = splitString(line, ',');
            if (tokens.size() == 3) {
                string location = tokens[0];
                string disease = tokens[1];
                int cases = stoi(tokens[2]);
                recordCase(data, location, disease, cases);
            }
        }
        file.close();
        cout << "Data loaded from " << filename << endl;
    } else {
        cout << "Unable to load data from " << filename << endl;
    }
}

int main() {
    map<string, vector<DiseaseCase> > data;
    string filename = "data.csv";  // Change the filename if needed

    loadData(data, filename);

    string command;
    while (true) {
        cout << "Command: ";
        getline(cin, command);
        vector<string> tokens = splitString(command, ' ');

        if (tokens.empty()) {
            continue;
        }

        string mainCommand = toLowercase(tokens[0]);

        if (mainCommand == "add" && tokens.size() == 2) {
            string location = tokens[1];
            addLocation(data, location);
        } else if (mainCommand == "record" && tokens.size() == 4) {
            string location = tokens[1];
            string disease = tokens[2];
            int cases = stoi(tokens[3]);
            recordCase(data, location, disease, cases);
        } else if (mainCommand == "list" && tokens.size() == 1) {
            listLocations(data);
        } else if (mainCommand == "delete" && tokens.size() == 2) {
            string location = tokens[1];
            deleteLocation(data, location);
        } else if (mainCommand == "list" && tokens.size() == 2) {
            string disease = tokens[1];
            listLocationsWithDisease(data, disease);
        } else if (mainCommand == "cases" && tokens.size() == 2) {
            string disease = tokens[1];
            displayTotalCases(data, disease);
        } else if (mainCommand == "cases" && tokens.size() == 3) {
            string location = tokens[1];
            string disease = tokens[2];
            displayCasesFromLocation(data, location, disease);
        } else if (mainCommand == "help") {
            displayHelp();
        } else if (mainCommand == "exit") {
            saveData(data, filename);
            break;
        } else {
            cout << "Invalid command. Please try again.\n";
        }
    }

    return 0;
}

