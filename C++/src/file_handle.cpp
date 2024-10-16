#include "iostream"
#include "fstream"
#include "string"

using namespace std;

int write_in_file()
{
    string const namefile("./test.txt");
    ofstream myFlow(namefile.c_str());

    if(myFlow)
    {
        myFlow << "Bonjour, je suis une phrase écrite dans un fichier." << endl;
        myFlow << 42.1337 << endl;
        int age(36);
        myFlow << "J'ai " << age << " ans." << endl;
    }
    else
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
    }
    return 0;
}

int read_file()
{
   ifstream file("./test.txt");

    if(file) {
        string ligne;

        while(getline(file, ligne)) { //Tant qu'on n'est pas à la fin, on lit
            cout << ligne << endl;
        }
    } else {
        cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
    }
   return 0;
}

int size_file()
{
    ifstream fichier("./test.txt");
    fichier.seekg(0, ios::end);
    int taille;
    taille = fichier.tellg();

    //On récupère la position qui correspond donc à la taille du fichier !
    cout << "Taille du fichier : " << taille << " octets." << endl;
    return 0;
}