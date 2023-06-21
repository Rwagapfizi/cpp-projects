#include <iostream>
#include <fstream>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// Get current time
string getCurrentTimestamp()
{
    time_t now = time(0);
    tm *currentTime = localtime(&now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", currentTime);

    return string(buffer);
}

// Function to convert a string to lowercase
string toLowerCase(const string &str)
{
    string lowerStr = str;
    for (std::string::size_type i = 0; i < lowerStr.length(); ++i)
    {
        lowerStr[i] = std::tolower(lowerStr[i]);
    }
    return lowerStr;
}

// Function to convert a string to uppercase
string toUpperCase(const string &str)
{
    string upperStr = str;
    for (std::string::size_type i = 0; i < upperStr.length(); ++i)
    {
        upperStr[i] = std::toupper(upperStr[i]);
    }
    return upperStr;
}

int stringToInt(const string &str)
{
    stringstream ss(str);
    int value = 0;
    ss >> value;
    return value;
}

// Check if location exists in file
bool locationExists(const string &location)
{
    ifstream inputFile("output.csv");
    if (inputFile.is_open())
    {
        string line;
        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string fileLocation;
            ss >> fileLocation;
            if (fileLocation == location)
            {
                inputFile.close();
                return true;
            }
        }
        inputFile.close();
    }
    return false;
}

// Save location to file
void addLocation(const string &location)
{
    if (locationExists(location))
    {
        cout << "\nLocation " << location << " already exists!" << endl;
        return;
    }

    ofstream outputFile("output.csv", ios::app);
    // transform(location.begin(), location.end(), location.begin(), ::toupper);
    string upperLocation = toUpperCase(location);
    if (outputFile.is_open())
    {
        outputFile << upperLocation << endl;
        cout << "\tLocation " << location << " added successfully!" << endl;
        outputFile.close();
    }
    else
    {
        cout << "\tError: Failed to open file for writing!" << endl;
    }
}

void deleteLocation(const string &location)
{
    ifstream inputFile("output.csv");
    ofstream tempFile("temp.csv");
    // transform(location.begin(), location.end(), location.begin(), ::toupper);
    string upperLocation = toUpperCase(location);
    if (inputFile.is_open() && tempFile.is_open())
    {
        string line;
        bool deleted = false;
        while (getline(inputFile, line))
        {
            if (line != upperLocation)
            {
                tempFile << line << endl;
            }
            else
            {
                deleted = true;
            }
        }
        inputFile.close();
        tempFile.close();

        if (deleted)
        {
            remove("output.csv");
            rename("temp.csv", "output.csv");
            cout << "\tLocation " << location << " deleted successfully!" << endl;
        }
        else
        {
            remove("temp.csv");
            cout << "\tLocation not found!" << endl;
        }
    }
    else
    {
        cout << "\tError: Failed to open files for reading/writing!" << endl;
    }
}

void listLocations() {
    ifstream inputFile("output.csv");
    if (inputFile.is_open()) {
        vector<string> locations;

        string line;
        while (getline(inputFile, line)) {
            istringstream iss(line);
            string location;
            iss >> location;
            locations.push_back(location);
        }

        inputFile.close();

        sort(locations.begin(), locations.end());

        cout << "\tLocations:" << endl;
        for (vector<string>::iterator it = locations.begin(); it != locations.end(); ++it) {
            cout << "\t" << *it << endl;
        }
    } else {
        cout << "\tError: Failed to open file for reading!" << endl;
    }
}

void listDiseases()
{
    ifstream inputFile("output.csv");
    if (inputFile.is_open())
    {
        string line;
        while (getline(inputFile, line))
        {
            string location;
            string disease;
            stringstream ss(line);
            ss >> location;
            ss >> disease;
            // cout << "\t" << line << endl;
            cout << "\t" << disease << endl;
        }
        inputFile.close();
    }
    else
    {
        cout << "\tError: Failed to open file for reading!" << endl;
    }
}

void listDetails()
{
    ifstream inputFile("output.csv");
    if (inputFile.is_open())
    {
        string line;
        while (getline(inputFile, line))
        {
            string location;
            string disease;
            string cases;
            stringstream ss(line);
            ss >> location >> disease >> cases;
            // cout << "\t" << line << endl;
            cout << "\t" << location << " " << disease << " " << cases << endl;
        }
        inputFile.close();
    }
    else
    {
        cout << "\tError: Failed to open file for reading!" << endl;
    }
}

void whereDisease(const string &disease)
{
    ifstream inputFile("output.csv");
    string upperDisease = toUpperCase(disease);
    if (inputFile.is_open())
    {
        string line;
        bool diseaseFound = false;
        while (getline(inputFile, line))
        {
            string fileLocation;
            string fileDisease;
            stringstream ss(line);
            ss >> fileLocation;
            ss >> fileDisease;
            if (fileDisease == upperDisease)
            {
                cout << "\tDisease " << disease << " exist at " << fileLocation << endl;
                diseaseFound = true;
            }
            // cout << "\t" << line << endl;
        }
        inputFile.close();
        if (!diseaseFound)
        {
            cout << "\tDisease " << disease << " was not found";
        }
    }
    else
    {
        cout << "\tError: Failed to open file for reading!" << endl;
    }
}

void casesDisease(const string &disease)
{
    ifstream inputFile("output.csv");
    string upperDisease = toUpperCase(disease);
    if (inputFile.is_open())
    {
        string line;
        int cases = 0;
        bool diseaseFound = false;
        while (getline(inputFile, line))
        {
            string fileLocation;
            string fileDisease;
            string fileCases;
            stringstream ss(line);
            ss >> fileLocation >> fileDisease >> fileCases;
            if (fileDisease == upperDisease)
            {
                cases += stringToInt(fileCases);
                diseaseFound = true;
            }
            // cout << "\t" << line << endl;
        }
        inputFile.close();
        if (!diseaseFound)
        {
            cout << "\tDisease " << disease << " was not found\n";
        }
        else
        {
            cout << "\tTotal cases of " << disease << ": " << cases << "\n";
        }
    }
    else
    {
        cout << "\tError: Failed to open file for reading!" << endl;
    }
}

void casesLocation(const string &location, const string &disease)
{
    ifstream inputFile("output.csv");
    string upperLocation = toUpperCase(location);
    string upperDisease = toUpperCase(disease);
    if (inputFile.is_open())
    {
        string line;
        int cases = 0;
        bool locationFound = false;
        while (getline(inputFile, line))
        {
            string fileLocation;
            string fileDisease;
            string fileCases;
            stringstream ss(line);
            ss >> fileLocation >> fileDisease >> fileCases;
            if (fileLocation == upperLocation)
            {
                if (fileDisease == upperDisease)
                {
                    cases = stringToInt(fileCases);
                }
                else
                {
                    cout << "\tDisease " << disease << " is not recorded to " << location << endl;
                }
                locationFound = true;
            }
            // cout << "\t" << line << endl;
        }
        inputFile.close();
        if (!locationFound)
        {
            cout << "\tLocation " << location << " not found\n";
        }
        else
        {
            cout << "\tnCses of " << disease << " at "<< location << ": " << cases << "\n";
        }
    }
    else
    {
        cout << "\tError: Failed to open file for reading!" << endl;
    }
}

void recordDiseaseCases(const string& location, const string& disease, int cases) {
    string upperLocation = toUpperCase(location);
    string upperDisease = toUpperCase(disease);

    if (!locationExists(upperLocation))
    {
        cout << "\tLocation does not exist!" << endl;
        return;
    }
    ifstream inputFile("output.csv");
    ofstream tempFile("temp.csv");
    if (inputFile.is_open() && tempFile.is_open()) {
        string line;
        bool updated = false;
        while (getline(inputFile, line)) {
            istringstream iss(line);
            string firstWord;
            iss >> firstWord;
            if (firstWord == upperLocation) {
                tempFile << upperLocation << " " << upperDisease << " " << cases << endl;
                updated = true;
            } else {
                tempFile << line << endl;
            }
        }
        inputFile.close();
        tempFile.close();

        if (updated) {
            remove("output.csv");
            rename("temp.csv", "output.csv");
            cout << "\tDisease cases updated successfully!" << endl;
        } else {
            cout << "\tLocation not found!" << endl;
            remove("temp.csv");
        }
    } else {
        cout << "Error: Failed to open files for reading/writing!" << endl;
    }
}

void help()
{
    cout << "*\t\t===================================================\n";
    cout << "*\t\t=================== HELP MENU =====================\n";
    cout << "*\t\t===================================================\n";
    cout << "\tadd <location>                          :Add a new location\n";
    cout << "\tdelete <location>                       :Delete an existing location\n";
    cout << "\trecord <location> <diseases> <cases>    :Record a disease and its cases\n";
    cout << "\tlist locations                          :List all existing locations\n";
    cout << "\tlist diseases                           :List existing diseases in locations\n";
    cout << "\twhere <disease>                         :Find where disease exists\n";
    cout << "\tcases <location> <disease>              :Find cases of a disease in location\n";
    cout << "\tcases <disease>                         :Find total cases of a given disease\n";
    cout << "\thelp                                    :Prints user manual\n";
    cout << "\texit                                    :Exit the program\n";
    cout << "==============================================================================================\n";
}

int main()
{
    string timestamp = getCurrentTimestamp();
    cout << "*\t\t===================================================\n";
    cout << "*\t\t==== Welcome to Disease Cases Reporting System ====\n";
    cout << "*\t\t***************************************************\n*\n";
    cout << "*\t\t**** Developed by Rwagapfizi Igor as practical ****\n";
    cout << "*\t\t*********** Evaluated for end of Year 3 ***********\n";
    cout << "*\t\t===================================================\n*\n";
    cout << "*\t\tStarting time: " << timestamp << " \n";
    cout << "*\t\tNeed help? Type 'help' then press Enter key\n\n";

    while (true)
    {
        string commmand;
        string location = "";
        string disease = "";
        string casesString = "";

        cout << "\n\tConsole > ";
        getline(cin, commmand);

        stringstream ss(commmand);
        ss >> commmand;

        string command = toLowerCase(commmand);

        if (command == "add")
        {
            ss >> location;
            addLocation(location);
        }
        else if (command == "delete")
        {
            ss >> location;
            deleteLocation(location);
        }
        else if (command == "list")
        {
            string listCommand;
            ss >> listCommand;
            string lowerListCommand = toLowerCase(listCommand);
            if (lowerListCommand == "locations")
            {
                listLocations();
            }
            else if (lowerListCommand == "diseases")
            {
                listDiseases();
            }
            else if (lowerListCommand == "details")
            {
                listDetails();
            }
            else
            {
                cout << "\tList command unclear.";
            }
        }
        else if (command == "where")
        {
            ss >> disease;
            whereDisease(disease);
        }
        else if (command == "record")
        {
            ss >> location >> disease >> casesString;
            int cases = stringToInt(casesString);
            recordDiseaseCases(location, disease, cases);
        }
        else if (command == "cases")
        {
            string firstCommand;
            string secondCommand;
            ss >> firstCommand >> secondCommand;
            if (secondCommand == "")
            {
                casesDisease(firstCommand);
            }
            else
            {
                casesLocation(firstCommand, secondCommand);
            }
        }
        else if (command == "help")
        {
            help();
        }
        else if (command == "exit")
        {
            cout << "\tExiting the program." << endl;
            break; // Exit the loop and end the program
        }
        else
        {
            cout << "\tInvalid command!" << endl;
        }
    }
    return 0;
}
