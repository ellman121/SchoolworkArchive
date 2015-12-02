#include <iostream>
#include <vector>
#include <utility>
#include <stdlib.h>
#include <string>
#include <fstream>

#define For(i,a,b) for(int (i)=(a);(i) < (b); ++(i))

using namespace std;

typedef pair<int,int> pii;

//all posibles for a and a inverse
const int par[12] = {1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25};
const int inverse[12] = {1, 9, 21,15,3, 19,  7, 23, 11,  5, 17, 25};

const char alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

int findLetter(char x)
{
    for (int i = 0; i < 26; ++i)
    {
        if (x == alphabet[i])
        {
            return i;
        }
    }
    return -1;
}

// Takes in a normalized lower case string
// returns out upper case string
string encryptionMessage(string msg, pii key)
{
    int temp;
    int alpha = key.first;
    int beta = key.second;
    char *ciphertext = new char[msg.length()];
    
    for (int i = 0; i < msg.length(); ++i)
    {
        temp = findLetter(msg[i]);
        temp = (temp * alpha + beta) % 26;
        ciphertext[i] = toupper(alphabet[temp]);
    }

    return (string) ciphertext;
}

string decryptionMessage(string ctxt, pii key)
{
    string msg = "";

    int temp;
    int a_inv = 0;
    for(int i = 0; i < 12; i++)
    {
        if ((inverse[i] * key.first) % 26 == 1)
        {
            a_inv = inverse[i];
        }
    }

    for(int i = 0; i < ctxt.length(); i++)
    {
        temp = a_inv * (ctxt[i] - key.second - 65);
        if(temp < 0)
        {
            temp = temp % 26 +26;
        }
        else
        {
            temp = temp % 26;
        }
        msg = msg + 
        (char) ( temp + 65);
    }
    return msg;
}

int main(int argc, char const *argv[])
{
    int length, j = 0;
    pii key;
    string message(argv[2]);
    string cipher;

    ifstream bigrams_input;

    // Assign alpha and beta
    while(j < 26)
    {
        if (argv[1][0] == alphabet[j])
        {
            key.first = j;
            break;
        }
        j++;
    }
    j = 0;
    while(j < 26)
    {
        if (argv[1][1] == alphabet[j])
        {
            key.second = j;
            break;
        }
        j++;
    }

    if (message != "ciphertext.txt")
    {
        for (int i = 0; i < message.length(); ++i)
        {
            if (ispunct(message[i]) || isspace(message[i]) )
            {
                message.erase(i,1);
                i--;
            }
        }

        cipher = encryptionMessage(message, key);

        ofstream fout;
        fout.open("ciphertext.txt");
        fout << cipher << endl;
    }
    else
    {
        ifstream fin;
        fin.open("ciphertext.txt");
        ofstream fout;
        fout.open("plaintext.txt");

        fin >> cipher;

        message = decryptionMessage(cipher, key);
        for (int i = 0; i < message.length(); ++i)
        {
            message[i] = tolower(message[i]);
        }

        fout << message << endl;
    }

    return 0;
}
