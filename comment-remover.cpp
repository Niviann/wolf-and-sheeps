#include <string>
#include <iostream>
#include <fstream>
using namespace std;

void clear() // funkcja czyszcząca konsole
{
    for (int i = 0; i < 30; i++)
        cout << endl;
}

void spaceCreate(bool *type, char *active_char, bool value, bool plusOne) // funkcja wstawiająca spacje i podtrzymująca dalsze usuwanie
{
    *type = value;
    *active_char = ' ';  // zamiana na spacje
    if (plusOne == true) // dla */
        *(active_char + 1) = ' ';
}

void logo(bool option = false)
{
    clear();
    if (option == false)
        cout << R"(
  _  __                          _                        _   _                         _             
 | |/ /___  _ __ ___   ___ _ __ | |_ __ _ _ __ _______   | | | |___ _   ___      ____ _| |_ ___  _ __ 
 | ' // _ \| '_ ` _ \ / _ \ '_ \| __/ _` | '__|_  / _ \  | | | / __| | | \ \ /\ / / _` | __/ _ \| '__|
 | . \ (_) | | | | | |  __/ | | | || (_| | |   / / (_) | | |_| \__ \ |_| |\ V  V / (_| | || (_) | |   
 |_|\_\___/|_| |_| |_|\___|_| |_|\__\__,_|_|  /___\___/   \___/|___/\__,_| \_/\_/ \__,_|\__\___/|_| 
        )";
    else
    {
        logo();
        cout << R"(
   _,-='"-.__               /\_/\
   `-.}       `=._,.-==-._.,  @ @._,
      `-.__   _,-.   )       _,.-'   Chytry lis zwinal twoj komentarz
           `"     G..m-"^m`m'                                                   
        )";
    }
}

void generateFile(bool functionCase, string fileName)
{
    bool exist = false; //czy plik z dopiską new już istnieje
    while (true)
    {
        fstream file(fileName);
        ofstream buffer("buffer");
        string newFileName = fileName.substr(0, fileName.size() - 4);
        newFileName.append("_New.cpp");
        if (ifstream(newFileName)) //sprawdzam czy istnieje
            exist = true;

        if (file.is_open() && fileName.find(".cpp") != string::npos)
        {
            ofstream newFile(newFileName);
            bool type[3] = {false, false, false}; // 1 dla komentarza typu "//" || 2 dla komentarza typu "/*..*/" || 3 dla komentarza w apostrofie ""
            string line;
            while (getline(file, line))
            {
                string withoutSpaces = "";
                for (int i = 0; i < line.size(); i++)
                {
                    if (line[i] == '"' && type[0] == false && type[1] == false && line[i - 1] != char(39) && line[+1] != char(39)) //wykrywanie apostrofu
                        type[2] = !type[2];                                                                                        // ustawianie wejścia/wyjścia z apostrofu
                    else if (type[2] == false)
                    {
                        if ((line[i] == '/' && line[i + 1] == '/') || type[0] == true) //wykrywanie // i wstawianie spacji do końca linii
                            spaceCreate(&type[0], &line[i], true, false);
                        else if ((line[i] == '/' && line[i + 1] == '*') || type[1] == true)                       //wykrywanie początku /*
                            if ((line[i] == '/' && line[i + 1] == '*' && line[i + 2] == '/') && type[1] == false) //wykrycie /*/
                                spaceCreate(&type[1], &line[i], true, true);
                            else if (line[i] == '*' && line[i + 1] == '/') //wykrycie końca sekwencji
                                spaceCreate(&type[1], &line[i], false, true);
                            else if (line[i] != ' ') // wstawianie spacji zamiast znaków w komentarzu typu /*..*/
                                spaceCreate(&type[1], &line[i], true, false);
                    }
                    if (type[0] == false && type[1] == false)
                        withoutSpaces.append(string(1, line[i])); //przepisuje do stringa znaki które nie są komentarzem
                }
                type[0] = false;
                buffer << line << endl;

                if (type[0] == false && type[1] == false)
                    newFile << withoutSpaces << endl;
            }
            file.close();
            newFile.close();
            buffer.close();

            if (functionCase == false)
            {
                remove(fileName.c_str());
                rename("buffer", fileName.c_str());
                if (exist == false)
                    remove(newFileName.c_str());
            }

            remove("buffer");
            break;
        }
        else
        {
            buffer.close();
            remove("buffer");
            logo();
            cout << endl
                 << "Podales zla nazwe pliku podaj jeszcze raz:" << endl;
            cin >> fileName;
        }
    }
}

string fileSelect() //wybór pliku
{
    string filename;
    logo();
    cout << endl
         << "Podaj nazwe pliku:" << endl;
    cin >> filename;
    return (filename);
}

int main()
{
    logo();
    while (true)
    {
        int menu_option;
        cout << endl
             << "1: Modyfikacja wybranego pliku i zastapienie komentarza na spacje" << endl;
        cout << "2: Utworzenie nowego pliku bez komentarza" << endl;
        cout << "3: Instrukcja" << endl;
        cout << "4: Wyjscie" << endl
             << endl;
        cout << "Podaj opcje:" << endl;
        cin >> menu_option;
        if (cin.fail())//wykrywam złe wejście 
        {
            cin.clear();
            cin.ignore();
            menu_option = 0;
        }
        switch (menu_option)
        {
        case 1: //wybór sposobu
        case 2:
            generateFile(menu_option - 1, fileSelect());
            logo(true);
            break;
        case 3: // instrukcja
            logo();
            cout << endl
                 << "Instrukcja :" << endl
                 << "Wybierz opcje z menu i podaj nazwe pliku cpp z koncowka lub sciezke (bez spacji) do niego, a program sam usunie komentarz."
                 << endl;
            break;
        case 4: //wyjście
            return (0);
        default:
            logo();
            cout << endl
                 << "Wpisales zla opcje, wybierz ponownie:";
        }
    }
}