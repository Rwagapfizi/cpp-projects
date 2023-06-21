#include <iostream>
#include <fstream>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <regex>
#include <map>

using namespace std;

// File name to char
string csvname = "items.csv";
const char *filename = csvname.c_str();

// Item Structure, to be used for comparison
struct Item
{
    string item_id;
    string item_name;
    string quantity;
    string registration_date;
};

// Get current time
string getCurrentTimestamp()
{
    time_t now = time(0);
    tm *currentTime = localtime(&now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", currentTime);

    return string(buffer);
}

// Get current date, for default Registration date
string getCurrentDate()
{
    time_t now = time(0);
    tm *currentTime = localtime(&now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", currentTime);

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

// Check if Item already exist in file
int itemExists(const char *filename, const string &item_id, const string &item_name)
{
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            string id, name, qty, date;
            getline(iss, id, ',');
            getline(iss, name, ',');
            getline(iss, qty, ',');
            getline(iss, date);

            // Values depending on what's found:
            // 0: Item doesn't exist
            // 1: Item ID exists
            // 2: Item name exists
            // 3: Item ID & name exists

            if (id == item_id)
            {
                file.close();
                return 1;
            }
            else if (name == item_name)
            {
                file.close();
                return 2;
            }
            else if (id == item_id || name == item_name)
            {
                file.close();
                return 3;
            }
        }
        file.close();
    }
    return 0;
}

// Compare item name, used for sorting
bool compareItems(const Item &item1, const Item &item2)
{
    return item1.item_name < item2.item_name;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
// Item Functions
// Record item from user input
void addItem(const string &item_id, const string &input_name, const string &quantity, string &registration_date)
{

    // See if there's no input
    if (item_id == "")
    {
        cout << "\tPlease input Item to be recorded" << endl;
        return;
    }

    // Set Registration Date to today in case of no input
    if (registration_date == "")
    {
        registration_date = getCurrentDate();
        cout << "\tRegistration Date set to today: " << registration_date << endl;
    }

    // Validate Registration Date
    regex dateRegex("\\d{4}-\\d{2}-\\d{2}");
    if (!regex_match(registration_date, dateRegex))
    {
        cout << "\tError: Invalid registration date format. Correct format: YYYY-MM-DD." << endl;
        return;
    }

    string item_name = toUpperCase(input_name);

    // Check if item already exist in file
    int itemExistStatus = itemExists(filename, item_id, item_name);
    if (itemExistStatus == 1)
    {
        cout << "\tError: Item ID " << item_id << " already exists." << endl;
        return;
    }
    else if (itemExistStatus == 2)
    {
        cout << "\tError: Item " << input_name << " already exists." << endl;
        return;
    }

    // Add item to file
    ofstream outputFile(filename, ios::app);
    if (outputFile.is_open())
    {
        outputFile << item_id << "," << item_name << "," << quantity << "," << registration_date << endl;
        outputFile.close();
        cout << "\tItem added successfully!" << endl;
    }
    else
    {
        cout << "\tError: Unable to open the file." << endl;
    }
}

// List existing items
void listItems()
{
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        vector<Item> items;
        while (getline(file, line))
        {
            istringstream iss(line);
            string id, name, qty, date;
            getline(iss, id, ',');
            getline(iss, name, ',');
            getline(iss, qty, ',');
            getline(iss, date);

            Item item;
            item.item_id = id;
            item.item_name = name;
            item.quantity = qty;
            item.registration_date = date;

            items.push_back(item);
        }

        file.close();

        sort(items.begin(), items.end(), compareItems);
        for (const auto &item : items)
        {
            int item_length = item.item_name.length();
            int blankSpace = 10 - item_length;
            cout << "\n\tItem ID: " << item.item_id << "\tItem Name: " << item.item_name;
            for (int i = 0; i < blankSpace; i++)
            {
                cout << " ";
            }
            cout << "\tQuantity: " << item.quantity << "\tReg Date: " << item.registration_date << endl;
            cout << "\t-----------------------------------------------------------------------------" << endl;
        }
    }
    else
    {
        cout << "\tError: Unable to open the file." << endl;
    }
}

// Delete by ID
void deleteById(const string &item_id)
{
    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cout << "Error: Unable to open the file." << endl;
        return;
    }

    ofstream outputFile("temp.csv");
    if (!outputFile.is_open())
    {
        cout << "Error: Unable to create temporary file." << endl;
        inputFile.close();
        return;
    }

    string line;
    bool found = false;
    while (getline(inputFile, line))
    {
        istringstream iss(line);
        string id, name, qty, date;
        getline(iss, id, ',');
        getline(iss, name, ',');
        getline(iss, qty, ',');
        getline(iss, date);

        if (id != item_id)
        {
            outputFile << line << endl;
        }
        else
        {
            found = true;
        }
    }

    inputFile.close();
    outputFile.close();

    if (found)
    {
        remove(filename);
        rename("temp.csv", filename);
        cout << "\tItem ID " << item_id << " deleted successfully!" << endl;
    }
    else
    {
        remove("temp.csv");
        cout << "\tError: Item of ID " << item_id << " not found." << endl;
    }
}

// Delete by Name
void deleteByName(const string &input_name)
{
    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cout << "\tError: Unable to open the file." << endl;
        return;
    }

    ofstream outputFile("temp.csv");
    if (!outputFile.is_open())
    {
        cout << "\tError: Unable to create temporary file." << endl;
        inputFile.close();
        return;
    }

    string item_name = toUpperCase(input_name);
    string line;
    bool found = false;
    while (getline(inputFile, line))
    {
        istringstream iss(line);
        string id, name, qty, date;
        getline(iss, id, ',');
        getline(iss, name, ',');
        getline(iss, qty, ',');
        getline(iss, date);

        if (name != item_name)
        {
            outputFile << line << endl;
        }
        else
        {
            found = true;
        }
    }

    inputFile.close();
    outputFile.close();

    if (found)
    {
        remove(filename);
        rename("temp.csv", filename);
        cout << "\tItem " << input_name << " deleted successfully!" << endl;
    }
    else
    {
        remove("temp.csv");
        cout << "\tError: Item " << input_name << " not found." << endl;
    }
}

// Update Item by ID
void updateItem(const string &item_id, const string &input_name, const string &new_quantity, const string &new_registration_date)
{
    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cout << "\tError: Unable to open the file." << endl;
        return;
    }

    ofstream outputFile("temp.csv");
    if (!outputFile.is_open())
    {
        cout << "Error: Unable to create temporary file." << endl;
        inputFile.close();
        return;
    }

    string new_item_name = toUpperCase(input_name);
    string line;
    bool found = false;
    while (getline(inputFile, line))
    {
        istringstream iss(line);
        string id, name, qty, date;
        getline(iss, id, ',');
        getline(iss, name, ',');
        getline(iss, qty, ',');
        getline(iss, date);

        if (id == item_id)
        {
            //Check on different cases of missing inputs
            if (new_registration_date == "")
            {
                outputFile << id << "," << new_item_name << "," << new_quantity << "," << date << endl;
            }
            else if (new_quantity == "")
            {
                outputFile << id << "," << new_item_name << "," << qty << "," << date << endl;
            }
            else if (new_item_name == "")
            {
                cout << "\tPlease input data to be updated" << endl;
                return;
            } else {
                outputFile << id << "," << new_item_name << "," << new_quantity << "," << new_registration_date << endl;
            }
            found = true;
        }
        else
        {
            outputFile << line << endl;
        }
    }

    inputFile.close();
    outputFile.close();

    if (found)
    {
        remove(filename);
        rename("temp.csv", filename);
        cout << "\tItem " << item_id << " updated successfully!" << endl;
    }
    else
    {
        remove("temp.csv");
        cout << "\tError: Item not found." << endl;
    }
}

// Display Help manual
void help()
{
    cout << "*\t================================================================================================\n";
    cout << "*\t==================================== Commands syntaxes =========================================\n";
    cout << "*\t================================================================================================\n\n";
    cout << "\tadd <item_id> <item_name> <quantity> <registration_date>       :Record new item and its details\n";
    cout << "\tlist                                                           :List all recorded items\n";
    cout << "\tdelete id <item_id>                                            :Delete an item by given ID\n";
    cout << "\tdelete name <item_name>                                        :Delete an item by given name\n";
    cout << "\tupdate <item_id> <item_name> <quantity> <registration_date>    :Update an item by given ID\n";
    cout << "\thelp                                                           :Prints user manual\n";
    cout << "\texit                                                           :Exit the program\n";
    cout << "\n\t=================================================================================================\n";
}

int main()
{
    string timestamp = getCurrentTimestamp();
    cout << "*\t===========================================================================================\n";
    cout << "*\t=========================== Welcome to Inventory System Manager ===========================\n";
    cout << "*\t*******************************************************************************************\n*\n";
    cout << "*\t******************************* Developed by Rwagapfizi Igor ******************************\n";
    cout << "*\t==========================================================================================\n*\n";
    cout << "*\tStarting time: " << timestamp << " \n";
    cout << "*\tNeed help? Type 'help' then press Enter key\n\n";

    string input;

    while (true)
    {
        string item_id = "";
        string item_name = "";
        string item_quantity = "";
        string item_registration_date = "";
        string deleteCommand;

        cout << "\n\tConsole > ";
        getline(cin, input);

        stringstream ss(input);
        ss >> input;

        string command = toLowerCase(input);

        if (command == "itemadd" || command == "additem" || command == "add")
        {
            ss >> item_id >> item_name >> item_quantity >> item_registration_date;
            addItem(item_id, item_name, item_quantity, item_registration_date);
        }
        else if (command == "itemslist" || command == "listitems" || command == "list")
        {
            listItems();
        }
        else if (command == "delete" || command == "deleteitem")
        {
            ss >> deleteCommand;
            if (deleteCommand == "id")
            {
                ss >> item_id;
                deleteById(item_id);
            }
            else if (deleteCommand == "name")
            {
                ss >> item_name;
                deleteByName(item_name);
            }
            else
            {
                cout << "\tDelete command unclear. Use \"delete id <id>\" or \"delete name <name>\"" << endl;
            }
        }
        else if (command == "update")
        {
            ss >> item_id >> item_name >> item_quantity >> item_registration_date;
            updateItem(item_id, item_name, item_quantity, item_registration_date);
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

        command = "";
    }
    return 0;
}
