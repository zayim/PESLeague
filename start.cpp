#include <iostream>
#include <cstring>
#include <iomanip>
#include <fstream>
using namespace std;
int main()
{
    fstream zayim,zayim2;
    int i,j,n,id=0;
    string nick;
    int home[100],away[100],kolo[100][100];

    zayim.open("tabela.txt",ios::out);

    cout << "Koliko igraca? "; cin >> n;
   // zayim << n << endl;
    zayim << "ID NICK             PLD    W    L    D     GD  PTS RANK" << endl;

    cout << "UNOS IGRACA\n===========\n";
    for (i=0; i<n; i++)
    {
        cout << "ID:" << id << "  NICK? "; cin >> nick;
        zayim << id++ << "  " << nick;
        for (j=0; j<15-nick.size(); j++) zayim << " ";
        zayim << setw(5) << 0 << setw(5) << 0 << setw(5) << 0 << setw(5) << 0 << setw(7) << "0:0" << setw(5) << 0 << setw(5) << (i+1) << endl;
    }
    zayim.close();

    zayim2.open("podaci.txt",ios::out);
    zayim2 << n << endl;

    //// kolo[i][j] -> u kojem kolu igraju i vs j

    for (i=0; i<(n+1)/2; i++) { home[i]=i; away[i]=(n+1)/2 + i; }

    for (i=1; i<n; i++)
    {
        for (j=0; j<(n+1)/2; j++)
        {
            if (home[j]==n) kolo[home[j]][away[j]] = kolo[away[j]][home[j]] = 0;
            else
            {
                kolo[home[j]][away[j]]=i;
                kolo[away[j]][home[j]]=n-1+i;
            }
        }
        int pomocna=home[1];
        for (j=1; j<(n+1)/2-1; j++) home[j]=home[j+1];
        home[(n+1)/2-1]=away[(n+1)/2-1];
        for (j=(n+1)/2-1; j>0; j--) away[j]=away[j-1];
        away[0]=pomocna;
    }

    for (i=0; i<n; i++)
    {
        for (j=0; j<n; j++) zayim2 << kolo[i][j] << " ";
        zayim2 << endl;
    }


    /*for (i=0; i<n; i++) {
    for (j=0; j<n; j++)
    zayim2 << "0 "; zayim2 << endl; }*/
    zayim2 << endl;

    for (i=0; i<n; i++) {
    for (j=0; j<n; j++)
    zayim2 << "0:0 "; zayim2 << endl; }
    zayim2 << endl;

    for (i=0; i<n; i++) {
    for (j=0; j<n; j++)
    zayim2 << "0 "; zayim2 << endl; }
    zayim2 << endl;

    zayim2.close();

    return 0;
}
