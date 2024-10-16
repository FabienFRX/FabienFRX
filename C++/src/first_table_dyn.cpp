#include "iostream"
#include "vector"

using namespace std;

int first_table_dyn()
{
   vector<double> notes; //Un tableau vide

   notes.push_back(12.5);  //On ajoute des cases avec les notes
   notes.push_back(19.5);
   notes.push_back(6);
   notes.push_back(12);
   notes.push_back(14.5);
   notes.push_back(15);
   
   double moyenne(0);
   for(int i(0); i<notes.size(); ++i)
   //On utilise notes.size() pour la limite de notre boucle
   {
      moyenne += notes[i];
   }
   moyenne /= notes.size();
   cout << "Votre moyenne est : " << moyenne << endl;
   return 0;
}