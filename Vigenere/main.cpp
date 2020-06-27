/*
 * Author: Pablo Cortez
 * Description: This is a basic vigenere cipher that will encrypt and decrypt text files
 * based on the key entered by the user. The key must be longer that four characters. The
 * key must be all alpha-characters. The encryption process will remove all punctuation
 * marks and numbers. This is done because having punctuation marks and numbers increases
 * the likelihood of someone of guessing what the ciphertext may be, or using statistical
 * analysis to guess the period of the key.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include<limits>
#include <cstdlib>
#include <cstdio>

using namespace std;

#define SPACE " "
#define ASCII_TO_NUM 'a'
#define NEW_LINE '\n'


void vigenere_Encrypt(ifstream &inFile, ofstream &outFile, const string &key);
void vigenere_Decrypt(ifstream &inFile, ofstream &outFile, const string &key);
bool checkFileExtension(string fileName);
string toLower(string str);

int main()
{
    cout << "Hello world!" << endl;

    string const alphabet = "abcdefghijklmnopqrstuvwxyz";
    string fileName = "";
    string const EXIT_PROGRAM = "EXIT";
    string encryptedFile = "";
    string key = "";
    string userInput = "";
    string choice = "";
    ifstream inFile;
    ofstream outFile;
    bool propeerFileName = false;
    bool encryptFiles = true;



    // Check whether user wants to encrypt or decrypt file.
    while(true)
    {
        cout << "Enter [E] to Encrypt a file, [D] to Decrypt a file, [Q] to Quit: ";
        getline(cin, choice);


        // Check if user wants to encrypt data
        if(choice == "E")
        {
            cout << "Enter File to Encrypt: ";
            break;
        }

        // Check if user wants to decrypt data
        else if(choice == "D")
        {
            cout << "Enter File to Decrypt: ";
            encryptFiles = false;
            break;
        }

        // Check if user wants to exit program
        else if( choice == "Q")
        {
            return 0;
        }

         // Check if user entered correct input, non-number or a double
        cout << "Entered incorrect input: " << choice << ", please try again "  << "\n";

    }


    // Get the name of the file to be encrypted

    cin >> fileName;


    propeerFileName = checkFileExtension(fileName);
    inFile.open(fileName);

    // Check if file was properly opened and has a proper filename
    while(!inFile.is_open() || !(propeerFileName))
    {
        // Get user to reenter file name
        cout << "INVALID FILE NAME, REENTER FILE NAME OR \"EXIT\" TO ESCAPE: ";
        cin >> fileName;
        propeerFileName = checkFileExtension(fileName);

        // Check if user wants to exit the program
        if(fileName == EXIT_PROGRAM)
        {
            return 0;
        }

        inFile.open(fileName);
    }


    // Get the name of the encrypted file
    cout << "Enter File Name to Send data to: ";
    cin >> encryptedFile;

    propeerFileName = checkFileExtension(encryptedFile);

    // Check if the file has a proper file extension
    while(!propeerFileName)
    {
        // Get user to reenter file name
        cout << "INVALID FILE NAME, REENTER FILE NAME OR \"EXIT\" TO ESCAPE: ";
        cin >> encryptedFile;


        if(fileName == EXIT_PROGRAM)
        {
            return 0;
        }

        propeerFileName = checkFileExtension(encryptedFile);
    }


    outFile.open(encryptedFile);


    std::size_t index = -1;
    // Keep looping if user doesn't enter a non-alpha key of length less than 4
    do
    {
        cout << "Enter an Alpha-Character key with length greater than 3: ";
        cin >> key;

        index = key.find_first_not_of(alphabet);

    }while( (key.length() < 4) || (index != string::npos) );

    key = toLower(key);


    // Encrypt the file:
    if(encryptFiles)
    {
        vigenere_Encrypt(inFile, outFile, key);
    }

    // Decrypt the file
    else
    {
        vigenere_Decrypt(inFile, outFile, key);
    }


    inFile.close();
    outFile.close();
    return 0;
}



/*
 * This will check if the file has a proper extension of
 * .docx or .txt
 */
bool checkFileExtension(string fileName)
{
    int length = fileName.length();
    string fileExtension = "";

    // Find the last dot "." in the file name
    std::size_t index = fileName.rfind(".");

    if(index == string::npos)
    {
        cout << "dot (.) not found, FileName: " << fileName << endl ;
        return false;
    }

    fileExtension = fileName.substr(index, length);

    // File contains no ".", index = npos = -1, not a proper file name


    // Check if it has a proper file extension
    if((fileExtension != ".txt") && (fileExtension != ".docx"))
    {
        cout << "Doesn't have a proper extension, FileName: " << fileName <<
                ", file Extension: " << fileExtension <<endl;
        return false;
    }

    return true;
}



/*
 * This will write the encrypted characters to the outFile based on the key, and the
 * encrypted file will be comprised of f-grams. The encrypted file will be stripped
 * of all punctuation, to decrease the possibility of statistical analysis.
 */
void vigenere_Encrypt(ifstream &inFile, ofstream &outFile, const string &key)
{
    char element = 'a';
    int spaceCtr = 1;
    int keyIndex = 0;
    int length = key.length();

    char key_Array[length + 1];
    strcpy(key_Array, key.c_str());

    //cin.ignore(numeric_limits<streamsize>::max(),'\n');

    //outFile << element;

    // Iterate through the inFile and send encrypted data to outFile
    while(inFile.get(element))
    {
        // Check if char is a non-alpha or non-numeric, if it is skip
        if( !isalpha(element) && !isdigit(element))
        {
            //cout << "Skipping: " << element << endl;
            continue;
        }

        // Convert ascii value to alphabet position
        element = tolower(element) - ASCII_TO_NUM;


        // Caesar shift based on the the position of the key
        element = (char)((element + (key_Array[keyIndex%length] - ASCII_TO_NUM))%26) +
                   ASCII_TO_NUM;

        // Increment the index to the key character to use
        keyIndex++;

        // Check if the 5-gram isn't complete
        if(spaceCtr%6 != 0)
        {
            outFile << element;
            //outFile.write((char*)&element, sizeof(element));
        }

        // Adda a space to make it a 5-gram
        else
        {
            // Check if we only need to add a space
            if(spaceCtr%60 != 0)
            {
                outFile << SPACE;
                outFile << element;
            }

            // Need to add a newline character to have 10 columns
            else
            {
                outFile << SPACE;
                outFile << NEW_LINE;
                outFile << element;
            }
            spaceCtr++;
        }

        spaceCtr++;
    }
}



/*
 * This function will decrypt a inFile using Vigenere cipher, using the key given by the
 * user. It will send the decrypted message to the outFile.
 *
 * The key must have length greater than four, and must be all alpha characters.
 */
void vigenere_Decrypt(ifstream &inFile, ofstream &outFile, const string &key)
{
    int keyIndex = 0;
    int spaceCtr = 1;
    int length = key.length();
    char element;

    // Convert key into a char array
    char char_Array[key.length() + 1];
    strcpy(char_Array, key.c_str());

    // Iterate through the inFile
    while(inFile.get(element))
    {
        //cout << "element is: " << element << endl;
        // Check if element is an alpha character
        if(!isalpha(element))
        {
            continue;
        }

        element = tolower(element) - ASCII_TO_NUM;

        // Convert the char to an element of the message space
        element = element - (char_Array[keyIndex%length] - ASCII_TO_NUM);


        // Check if element is positive
        if(element > 0)
        {
            element = (element%26) + ASCII_TO_NUM;
        }

        // Element is negative, add 26 for modulo arithmetic to properly work
        else
        {
            element += 26;
            element = (element%26) + ASCII_TO_NUM;
        }

        // Increment the index to the key character to use
        keyIndex++;

        // Check if we still need to build the 5-gram
        if((spaceCtr%6) != 0)
        {
            outFile << element;
        }

        // Add space to complete the 5-gram
        else
        {
            // Check if we only need to add a space
            if((spaceCtr%60) != 0 )
            {
                outFile << SPACE;
                outFile << element;
            }

            // Need to add a newline character to have 10 columns
            else
            {
                outFile << SPACE;
                outFile << NEW_LINE;
                outFile << element;
            }

            spaceCtr++;
        }

        spaceCtr++;
    }
}

/*
 * This function will return the string entered as all lower case. It will be used for
 * the key, so that we can convert the character to its alphabetic position number.
 */
string toLower(string str)
{
    int length = str.length();
    string newStr = "";

    for(int i = 0; i < length; i++)
    {
        newStr += tolower(str[i]);
    }

    return newStr;
}
