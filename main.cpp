#include <iostream>
#include <cstring>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <conio.h>
#include <cstdlib>
#define N 100
#define UNOS_REZULTATA 1
#define ISPIS_TABELE 2
#define UPIS_U_FAJL 3
#define PRONALAZAK_MECA 4
#define ISPIS_RASPOREDA 5
#define ISPIS_IGRACA 6
#define IZLAZ 7
#define BROJ_IZBORA 7
using namespace std;
int bc_10(int n)
{
    int tmp=n,p=1;
    while (tmp>0) { tmp/=10; p*=10; }
    return (p/10);
}
struct rezultat
{
    int h,a;
    rezultat() {}
    rezultat (int H, int A) { h=H; a=A; }
};
struct igrac
{
    int id;
    string nick;
    int w,l,d,pts,pld,gf,ga,rank;

    bool operator < (const igrac& I) const
    {
        if (pts > I.pts) return true;
        else if (pts == I.pts)
        {
            if ((gf-ga) > (I.gf-I.ga)) return true;
            else if ((gf-ga)==(I.gf-I.ga))
            {
                if (gf > I.gf) return true;
                else if (gf == I.gf)
                {
                    if (w > I.w) return true;
                    else if (w == I.w)
                    {
                        if (pld > I.pld) return true;
                        else return false;
                    }
                    return false;
                }
                return false;
            }
            return false;
        }
        return false;
    }
};
class tabela
{
    public:
    int brojIgraca;
    igrac igraci[N];
    rezultat res[N][N];
    int kolo[N][N];
    int rank[N];
    bool igrali[N][N];

    tabela() { brojIgraca=0; }
    friend void UCITAJ(tabela &T);
    friend void UPISI(tabela T, const char file_podaci[]="podaci.txt", const char file_tabela[]="tabela.txt");
    friend void NAPRAVI_BACKUP(tabela T);
    void Sortiraj();
    void unesiRezultat();
    void napraviRaspored();
    void ispisiRaspored();
    void ispisiTabelu();
    void ispisiIgrace();
    void nadjiMec();
};
void UCITAJ(tabela &T)
{
    fstream zayim,zayim2;
    int kraj=0,poc_a=0,h=0,a=0,p=1,j,i,k;
    string s;

    zayim.open("podaci.txt",ios::in);
    zayim >> T.brojIgraca;
    for (i=0; i<T.brojIgraca; i++)
    for (j=0; j<T.brojIgraca; j++) zayim >> T.kolo[i][j];

    for (i=0; i<T.brojIgraca; i++)
    for (j=0; j<T.brojIgraca; j++)
    {
        zayim >> s;

        int kraj=0,poc_a=0,p=1,k;
        T.res[i][j].h=T.res[i][j].a=0;

        while(s[kraj++]!=':') p*=10; kraj--; p/=10;
        for (k=0; k<kraj; k++)
        {
            T.res[i][j].h+=p*int(s[k]-48);
            p/=10;
        }
        p=1; poc_a=++kraj;

        while (s[kraj++]) p*=10; kraj--; p/=10;
        for (k=poc_a; k<kraj; k++)
        {
            T.res[i][j].a+=p*int(s[k]-48);
            p/=10;
        }
    }

    for (i=0; i<T.brojIgraca; i++)
    for (j=0; j<T.brojIgraca; j++) zayim >> T.igrali[i][j];

    zayim.close();

    zayim2.open("tabela.txt",ios::in);
    for (i=0; i<9; i++) zayim2 >> s;

    for (i=0; i<T.brojIgraca; i++)
    {
        zayim2 >> T.igraci[i].id >> T.igraci[i].nick >> T.igraci[i].pld >> T.igraci[i].w;
        zayim2 >> T.igraci[i].l >> T.igraci[i].d >> s >> T.igraci[i].pts >> T.igraci[i].rank;
        T.rank[i]=T.igraci[i].rank-1;

        int kraj=0,poc_a=0,p=1,k;
        T.igraci[i].gf=T.igraci[i].ga=0;

        while(s[kraj++]!=':') p*=10; kraj--; p/=10;
        for (k=0; k<kraj; k++)
        {
            T.igraci[i].gf+=p*int(s[k]-48);
            p/=10;
        }
        p=1; poc_a=++kraj;

        while (s[kraj++]) p*=10; kraj--; p/=10;
        for (k=poc_a; k<kraj; k++)
        {
            T.igraci[i].ga+=p*int(s[k]-48);
            p/=10;
        }
    }

    T.Sortiraj();

    zayim2.close();

}
void UPISI(tabela T, const char file_podaci[], const char file_tabela[])
{
    fstream zayim, zayim2;
    int i,j,tmp,p=1;
    string s1;

    zayim.open(file_podaci,ios::out);
    zayim << T.brojIgraca << endl;
    for (i=0; i<T.brojIgraca; i++) {
    for (j=0; j<T.brojIgraca; j++) zayim << T.kolo[i][j] << " "; zayim << endl; } zayim << endl;

    for (i=0; i<T.brojIgraca; i++) {
    for (j=0; j<T.brojIgraca; j++) zayim << T.res[i][j].h << ":" << T.res[i][j].a << " ";  zayim << endl; } zayim << endl;

    for (i=0; i<T.brojIgraca; i++) {
    for (j=0; j<T.brojIgraca; j++) zayim << T.igrali[i][j] << " "; zayim << endl; } zayim << endl;
    zayim.close();

    zayim2.open(file_tabela,ios::out);

    zayim2 << "ID NICK             PLD    W    L    D     GD  PTS RANK" << endl;
    for (i=0; i<T.brojIgraca; i++)
    {
        zayim2 << T.igraci[i].id << "  " << T.igraci[i].nick;

        for (j=0; j<15-T.igraci[i].nick.size(); j++) zayim2 << " ";

        s1="";

        tmp=T.igraci[i].gf;
        if(!tmp) s1+="0";
        else
        {
            p=bc_10(tmp);
            while (p>0) { s1+=char((tmp/p)+48); tmp%=p; p/=10; }
        }
        s1+=':';
        tmp=T.igraci[i].ga;
        if(!tmp) s1+="0";
        else
        {
            p=bc_10(tmp);
            while (p>0) { s1+=char((tmp/p)+48); tmp%=p; p/=10; }
        }

        zayim2 << setw(5) << T.igraci[i].pld << setw(5) << T.igraci[i].w << setw(5) << T.igraci[i].l << setw(5) << T.igraci[i].d;
        zayim2 << setw(7) << s1 << setw(5) << T.igraci[i].pts << setw(5) << T.igraci[i].rank << endl;
    }

    zayim2.close();
}
void NAPRAVI_BACKUP(tabela T) { UPISI(T,"podaci_BACKUP.txt","tabela_BACKUP.txt"); }
void tabela::ispisiIgrace()
{
    for (int i=0; i<brojIgraca; i++) cout << "ID:" << igraci[i].id << " NICK: " << igraci[i].nick << endl;
}
void tabela::Sortiraj()
{
    igrac tmp[N];
    int i,j;

    for (i=0; i<brojIgraca; i++) tmp[i]=igraci[i];

    sort(tmp,tmp+brojIgraca);

    /*for (i=0; i<brojIgraca; i++)
    {
        cout << tmp[i].id << " " << tmp[i].nick;
        cout << " " << tmp[i].pld << " " << tmp[i].w << " " << tmp[i].l << " " << tmp[i].d;
        cout << " " << tmp[i].gf << ":" << tmp[i].ga << " " << tmp[i].pts << endl;
    }*/

    for (i=0; i<brojIgraca; i++) { igraci[tmp[i].id].rank=i+1; rank[i]=tmp[i].id; }
}
void tabela::ispisiTabelu()
{
    string s1;
    int i,j,tmp,p=1;
    cout << "RANK ID NICK             PLD    W    L    D     GD  PTS" << endl;
    for (i=0; i<brojIgraca; i++)
    {
        cout << igraci[rank[i]].rank << ".   " << igraci[rank[i]].id << "  " << igraci[rank[i]].nick;
        for (j=0; j<15-igraci[rank[i]].nick.size(); j++) cout << " ";

        s1="";

        tmp=igraci[rank[i]].gf;
        if(!tmp) s1+="0";
        else
        {
            p=bc_10(tmp);
            while (p>0) { s1+=char((tmp/p)+48); tmp%=p; p/=10; }
        }
        s1+=':';
        tmp=igraci[rank[i]].ga;
        if(!tmp) s1+="0";
        else
        {
            p=bc_10(tmp);
            while (p>0) { s1+=char((tmp/p)+48); tmp%=p; p/=10; }
        }

        cout << setw(5) << igraci[rank[i]].pld << setw(5) << igraci[rank[i]].w << setw(5) << igraci[rank[i]].l << setw(5) << igraci[rank[i]].d;
        cout << setw(7) << s1 << setw(5) << igraci[rank[i]].pts << endl;
    }
}
void tabela::napraviRaspored()
{
    int i,j;

    for (i=0; i<brojIgraca; i++) kolo[0][i]=i;
    random_shuffle(kolo[0],kolo[0]+brojIgraca);

    for (i=1; i<brojIgraca; i++)
    {
        copy(kolo[i-1],kolo[i-1]+brojIgraca,kolo[i]);
        next_permutation(kolo[i],kolo[i]+brojIgraca);
    }

    for (i=0; i<brojIgraca; i++) kolo[i][i]=-1;
}
void tabela::ispisiRaspored()
{
    int i,j,k,*index,ind;
    for (i=1; i<brojIgraca + brojIgraca%2; i++)
    {
        cout << i << ". KOLO\n=======\n";

        for (j=0; j<brojIgraca; j++)
        {
            index=find(&kolo[j][0],&kolo[j][brojIgraca],i);
            //cout << "kolo:" << i << " j:" << j << " index:" << *index << endl; system("PAUSE");
            if (index != &kolo[j][brojIgraca])
            {
                ind=index-&kolo[j][0];
                cout << igraci[j].nick << " ";
                if (igrali[j][ind]) cout << res[j][ind].h << ":" << res[j][ind].a; else cout << ":";
                cout << " " << igraci[ind].nick << endl;
            }
        }
        cout << endl;
    }

}
void tabela::unesiRezultat()
{
    string igrac1,igrac2;
    int bg1,bg2,i1,i2,i,j;
    bool ima1=false,ima2=false;

    cout << "Unesi rezultat (PRIMJER: zayim nadin 0 0): "; cin >> igrac1 >> igrac2 >> bg1 >> bg2;
    for (i=0; i<brojIgraca; i++)
        if (igraci[i].nick == igrac1) { i1=i; ima1=true; break; }
    for (i=0; i<brojIgraca; i++)
        if (igraci[i].nick == igrac2) { i2=i; ima2=true; break; }

   // cout << "i1:" << i1 << " igrac[i1]:" << igraci[i1].nick << " i2:" << i2 << " igrac[i2]:" << igraci[i2].nick << endl << endl;

    if (!ima1 || !ima2) { cout << "Odabran je igrac koji nije u ligi!" << endl; getch(); }
    else if (igrali[i1][i2] || igrali[i2][i1]) { cout << "Igrali su vec!" << endl; getch(); }
    else{

    igrali[i1][i2]=igrali[i2][i1]=true;
    igraci[i1].pld++; igraci[i2].pld++;
    igraci[i1].gf+=bg1; igraci[i1].ga+=bg2; igraci[i2].gf+=bg2; igraci[i2].ga+=bg1;
    res[i1][i2].h=bg1; res[i1][i2].a=bg2; res[i2][i1].h=bg2; res[i2][i1].a=bg1;

    if (bg1 > bg2) { igraci[i1].w++; igraci[i1].pts+=3; igraci[i2].l++; }
    else if (bg1 < bg2) { igraci[i2].w++; igraci[i2].pts+=3; igraci[i1].l++; }
    else { igraci[i1].d++; igraci[i2].d++; igraci[i1].pts++; igraci[i2].pts++; }

    /*for (i=0; i<brojIgraca; i++)
    {
        cout << igraci[i].id << " " << igraci[i].nick;
        cout << " " << igraci[i].pld << " " << igraci[i].w << " " << igraci[i].l << " " << igraci[i].d;
        cout << " " << igraci[i].gf << ":" << igraci[i].ga << " " << igraci[i].pts << endl;
    }*/

    this->Sortiraj();
}}
void tabela::nadjiMec()
{
    int i,j,k,*index,ind;

    for (i=1; i<brojIgraca; i++)
    {
        for (j=0; j<brojIgraca; j++)
        {
            index=find(&kolo[j][0],&kolo[j][brojIgraca],i);
            if (index != &kolo[j][brojIgraca])
            {
                ind=index-&kolo[j][0];
                if (!igrali[j][ind] && !igrali[j][ind]) { cout << igraci[j].nick << " : " << igraci[ind].nick << endl; return; }
            }
        }
        cout << endl;
    }

    cout << "Odigrana je cijela sezona!" << endl;
}
int upit()
{
    char unos;
    int izbor=1;

    system("CLS");
    cout << "IZABERITE OPCIJU\n";
    cout << char(16) << " Unos rezultata\n  Ispis tabele\n  Upis u fajl\n  Pronalazak meca\n  Ispis rasporeda\n  Ispis igraca\n  Izlaz\n";
    unos=getch();

    while (unos != char(32))
    {
        if (unos=='W' || unos=='w') if (izbor>1) izbor--;
        if (unos=='S' || unos=='s') if (izbor < BROJ_IZBORA) izbor++;

        system("CLS");
        if (izbor==1)
        {
            cout << "IZABERITE OPCIJU\n";
            cout << char(16) << " Unos rezultata\n  Ispis tabele\n  Upis u fajl\n  Pronalazak meca\n  Ispis rasporeda\n  Ispis igraca\n  Izlaz\n";
        }
        else if (izbor==2)
        {
            cout << "IZABERITE OPCIJU\n";
            cout << "  Unos rezultata\n" << char(16) << " Ispis tabele\n  Upis u fajl\n  Pronalazak meca\n  Ispis rasporeda\n  Ispis igraca\n  Izlaz\n";
        }
        else if (izbor==3)
        {
            cout << "IZABERITE OPCIJU\n";
            cout << "  Unos rezultata\n  Ispis tabele\n" << char(16) << " Upis u fajl\n  Pronalazak meca\n  Ispis rasporeda\n  Ispis igraca\n  Izlaz\n";
        }
        else if (izbor==4)
        {
            cout << "IZABERITE OPCIJU\n";
            cout << "  Unos rezultata\n  Ispis tabele\n  Upis u fajl\n" << char(16) << " Pronalazak meca\n  Ispis rasporeda\n  Ispis igraca\n  Izlaz\n";
        }
        else if (izbor==5)
        {
            cout << "IZABERITE OPCIJU\n";
            cout << "  Unos rezultata\n  Ispis tabele\n  Upis u fajl\n  Pronalazak meca\n" << char(16) << " Ispis rasporeda\n  Ispis igraca\n  Izlaz\n";
        }
        else if (izbor==6)
        {
            cout << "IZABERITE OPCIJU\n";
            cout << "  Unos rezultata\n  Ispis tabele\n  Upis u fajl\n  Pronalazak meca\n  Ispis rasporeda\n" << char(16) << " Ispis igraca\n  Izlaz\n";
        }
        else if (izbor==7)
        {
            cout << "IZABERITE OPCIJU\n";
            cout << "  Unos rezultata\n  Ispis tabele\n  Upis u fajl\n  Pronalazak meca\n  Ispis rasporeda\n  Ispis igraca\n" << char(16) << " Izlaz\n";
        }
        unos=getch();
    }
    system("CLS");
    return izbor;
}
int main()
{
    tabela Tabela;
    int izbor;

    cout << "DOBRODOSLI U PROGRAM ZA VODJENJE TABELE !!!" << endl << endl;
    cout << "Par korisnih savjeta:\n1. Krecete se tipkama W (gore) i S (dole)\n2. Kada izaberete zeljenu opciju pritisnite SPACE\n";
    cout << "3. Nemojte predugo drzati program, nema garancije da ce raditi... :S\n";
    getch();

    UCITAJ(Tabela);
    NAPRAVI_BACKUP(Tabela);

    while (izbor != IZLAZ)
    {
        izbor=upit();

        if (izbor==UNOS_REZULTATA) Tabela.unesiRezultat();
        else if (izbor==ISPIS_TABELE) { Tabela.ispisiTabelu(); cout << endl; getch(); }
        else if (izbor==UPIS_U_FAJL) UPISI(Tabela);
        else if (izbor==PRONALAZAK_MECA) { Tabela.nadjiMec(); cout << endl; getch(); }
        else if (izbor==ISPIS_RASPOREDA) { Tabela.ispisiRaspored(); cout << endl; getch(); }
        else if (izbor==ISPIS_IGRACA) { Tabela.ispisiIgrace(); cout << endl; getch(); }
    }

    UPISI(Tabela);

    return 0;
}
