#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <sstream>
#include <chrono>

class Gracz{
private:
    int saldo=0;
    int liczba_gier=0;
    int liczba_wygranych=0;
    int najwieksza_wygrana=0;
public:
    bool czy_mozesz_odjac(int kwota){
        if(saldo>=kwota)
            return true;
        else
            return false;
    }
    void odejmij(int kwota){
        if(czy_mozesz_odjac(kwota))
        {
            saldo-=kwota;
        }
    }
    void dodaj(int kwota){
        saldo+=kwota;
    }
    int getSaldo(){
        return saldo;
    }
    void updateStatystyk(int wygrana){
        liczba_gier++;
        if(wygrana>0){
            liczba_wygranych++;
        }
        if(wygrana>najwieksza_wygrana){
            najwieksza_wygrana=wygrana;
        }
    }
    int getLiczbaGier(){return liczba_gier;}
    int getLiczbaWygranychGier(){return liczba_wygranych;}
    int getNajwiekszaWygrana(){return najwieksza_wygrana;}

    void zapiszDoPliku(){
        std::ofstream plik("kasyno_dane.txt");
        if(plik.is_open()){
            plik<<"Saldo: "<<saldo<<std::endl;
            plik<<"Gry: "<<liczba_gier<<std::endl;
            plik<<"Wygrane: "<<liczba_wygranych<<std::endl;
            plik<<"Rekord "<<najwieksza_wygrana<<std::endl;
            plik.close();
        }
    }

    void wczytajZPliku(){
        std::ifstream plik("kasyno_dane.txt");
        if(plik.is_open()){
            std::string etykieta;
            plik>>etykieta>>saldo;
            plik>>etykieta>>liczba_gier;
            plik>>etykieta>>liczba_wygranych;
            plik>>etykieta>>najwieksza_wygrana;
            plik.close();
        }
    }

};

class blackJack{
private:
    struct karta{
        std::string nazwa;
        int wartosc;
    };
    std::vector<karta> talia;
public:
    void przygotujTalie(){
        std::string kolory[]={"Kier","Karo","Trefl","Pik"};
        std::string figury[]={"2","3","4","5","6","7","8","9","10","Walet","Dama","Krol","As"};
        int wartosci[]={2,3,4,5,6,7,8,9,10,10,10,10,11};
        for(int i=0;i<4;i++){
            for(int j=0;j<13;j++){
                karta nowaKarta;
                nowaKarta.nazwa=figury[j]+" "+kolory[i];
                nowaKarta.wartosc=wartosci[j];

                talia.push_back(nowaKarta);
            }
        }
    }
    void przetastujTalie(){
        unsigned int ziarno=std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine g(ziarno);
        std::shuffle(talia.begin(),talia.end(),g);
    }
    void graj(Gracz &gracz){
        while(true){
            przygotujTalie();
            przetastujTalie();
            bool ok;
            int stawka;
            std::string inputStawka;
            do{
                ok=true;
                std::cout<<"Podaj stawke za jaka chcesz zagrac: (lub 'q' aby wyjsc):";
                std::cin>>inputStawka;
                if(inputStawka=="q"||inputStawka=="Q")
                    return;
                std::stringstream ss(inputStawka);
                if(!(ss>>stawka)){
                    std::cout<<"Podaj liczbe a nie tekst!\n";
                    ok=false;
                    Sleep(750);
                    std::system("cls");
                    continue;
                }
                if(stawka<=0){
                    std::cout<<"Za tyle to sobie nie pogracz ^-^\n";
                    ok=false;
                    Sleep(750);
                    std::system("cls");
                    continue;
                }
                if(gracz.czy_mozesz_odjac(stawka))
                    gracz.odejmij(stawka);
                else{
                    std::cout<<"Za malo srodkow\n";
                    ok=false;
                    Sleep(750);
                    std::system("cls");
                    continue;
                }
            }while(!ok);
            std::vector<karta> rekaGracza;
            std::vector<karta> rekaKrupiera;

            //rozdawanie kart

            rekaGracza.push_back(talia.back());
            talia.pop_back();
            rekaKrupiera.push_back(talia.back());
            talia.pop_back();
            rekaGracza.push_back(talia.back());
            talia.pop_back();
            rekaKrupiera.push_back(talia.back());
            talia.pop_back();

            int sumaPoczatkowaGracza=rekaGracza[0].wartosc+rekaGracza[1].wartosc;
            if(rekaGracza.size()==2&&sumaPoczatkowaGracza==21){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                std::cout<<"\n=== BlACKJACK! Masz 21 z dwoch kart!===\n";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                int sumaPoczatkowaKrupiera=rekaKrupiera[0].wartosc+rekaKrupiera[1].wartosc;
                if(sumaPoczatkowaKrupiera==21){
                    std::cout<<"Krupier rowniez ma BlackJacka! Remis. Zwrot stawki.\n";
                    gracz.dodaj(stawka);
                }else{
                    std::cout<<"Wygrywasz: "<<stawka*2.5<<std::endl;
                    getch();
                    system("cls");
                    gracz.dodaj(stawka*2.5);
                }
            }
            else{
            char decyzja;
            bool turaGracza=true;
            int wartosc_kart;
            while(turaGracza){
                wartosc_kart=0;
                std::system("cls");
                std::cout<<"Twoje karty"<<std::endl;
                for(int i=0;i<rekaGracza.size();i++){
                    std::cout<< "- "<<rekaGracza[i].nazwa<<"(Wartosc: "<<rekaGracza[i].wartosc<<")\n";
                    wartosc_kart+=rekaGracza[i].wartosc;
                }
                std::cout<<"Aktualna suma puntkow: "<<wartosc_kart<<"\n";
                if(wartosc_kart>21){
                    std::cout<<"Przekroczyles/as 21puntkow!\n";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    std::cout<<"Przegrywasz.\n";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    getch();
                    system("cls");
                    break;
                }
                std::cout<<"- " <<rekaKrupiera[0].nazwa << std::endl;
                std::cout<<"- [Karta zakryta]\n";
                std::cout<<"\n[1] Dobierz karte (Hit)\n";
                std::cout<< "[2] Pasuj (Stand)\n";
                std::cout<<"Twoj wybor: ";
                std::cin>>decyzja;
                switch(decyzja){
                case '1':
                    rekaGracza.push_back(talia.back());
                    talia.pop_back();
                    break;
                case '2':
                    turaGracza=false;
                    break;
                default:
                    std::cout<< "Nie ma takiej opcji, wybierz 1 lub 2.\n";
                    break;
                }
            }

            if(wartosc_kart<=21){
                std::cout<<"\nTura krupiera...\n";

                std::cout<<"Karty krupiera: \n";
                std::cout<<rekaKrupiera[0].nazwa <<" (Wartosc: "<<rekaKrupiera[0].wartosc<<")\n";
                std::cout<<rekaKrupiera[1].nazwa <<" (Wartosc: "<<rekaKrupiera[1].wartosc<<")\n";
                int wartoscKrupiera=0;
                wartoscKrupiera=rekaKrupiera[0].wartosc+rekaKrupiera[1].wartosc;
                std::cout<<"Poczatkowa suma krupiera: "<<wartoscKrupiera<<"\n\n";
                while(wartoscKrupiera<17){
                    rekaKrupiera.push_back(talia.back());
                    talia.pop_back();
                    karta dobralKrupier=rekaKrupiera[rekaKrupiera.size()-1];
                    std::cout<<"Krupier dobiera karte: "<<dobralKrupier.nazwa<<" (Wartosc: "<< dobralKrupier.wartosc << ")\n";
                    wartoscKrupiera+=rekaKrupiera[rekaKrupiera.size()-1].wartosc;
                    std::cout<<"Suma krupiera po dobraniu: "<<wartoscKrupiera<< std::endl;
                }
                if(wartoscKrupiera>21){
                    gracz.dodaj(2*stawka);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                    std::cout <<"Wygrales "<<2*stawka<<std::endl;
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                }
                else if(wartosc_kart>wartoscKrupiera){
                    gracz.dodaj(2*stawka);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                    std::cout<<"\nWygrales"<<2*stawka<<std::endl;
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                }
                else if(wartoscKrupiera>wartosc_kart){
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    std::cout<<"\nPrzegrales...\n";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                }
                else if(wartoscKrupiera==wartosc_kart){
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14 );
                    std::cout<<"\nRemis!\n";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    gracz.dodaj(stawka);
                }
                getch();
                std::system("cls");

            }


        }//koniec else po sprawdzaniu czy byl blackjack
        }//koniec petly gry

    }//koniec void graj
};//koniec klasy


class Slots{
private:
    const int kwota=5;
    const std::string znaki="7@*$";
    void spin(char &a, char&b, char &c){
        a=znaki[rand()%znaki.length()];
        b=znaki[rand()%znaki.length()];
        c=znaki[rand()%znaki.length()];
    }
    void show(char a,char b,char c){
        std::system("cls");
        std::cout<< "3 CYTRYNY" << std::endl;
        std::cout<<"-------------------"<<std::endl;
        std::cout<<"|  "<<a;
        std::cout<<"  |  "<<b;
        std::cout<<"  |  "<<c;
        std::cout <<"   |"<<std::endl;
        std::cout<<"-------------------" << std::endl;
    }
    int checkWin(char a, char b, char c){
        if(a==b&&b==c){
            if(a=='7') return 1000;
            if(a=='@') return 100;
            if(a=='*') return 50;
            if(a=='$') return 10;
        }
        return 0;
    }
public:
    void Gra(Gracz &gracz){
        while(true){
            std::system("cls");
            std::cout<< "1. Zakrec"<<std::endl;
            std::cout<<"q. Wyjscie"<<std::endl;
            std::cout<< " " <<std::endl;
            std::cout<< " " <<std::endl;
            std::cout<<"Traf 3 takie same symbole zeby wygrac!"<<std::endl;
            std::cout<<"Trzy 7= 1000PLN"<<std::endl;
            std::cout<<"Trzy @= 100PLN"<<std::endl;
            std::cout<<"Trzy *= 50PLN"<<std::endl;
            std::cout<<"Trzy $= 10PLN"<<std::endl;
            std::cout<<""<<std::endl;
            std::cout<<""<<std::endl;
            std::cout<<"Saldo: "<< gracz.getSaldo()<<"PLN"<<std::endl;
            char Swybor=getch();
            switch(Swybor)
            {
            case '1':
                {
                char a,b,c;
                if(gracz.czy_mozesz_odjac(kwota))
                {
                gracz.odejmij(kwota);
                spin(a,b,c);
                show(a,b,c);
                Sleep(400);
                int win=checkWin(a,b,c);
                if(win>0){
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
                    std::cout <<"\nWYGRANA: "<<win<<" PLN!";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                }
                else{
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
                    std::cout<<"\nBrak wygranej :(";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
                }
                gracz.dodaj(win);
                gracz.updateStatystyk(win);
                }
                else{
                    std::system("cls");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    std::cout<<"Brak srodkow!";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                }
                getch();
                }
                break;
            case 'q':
                return;
            }
        }
    }
};

class Lotto{
private:
    const int kwota=10;
    void los_wyg(int wygrane[6]){
        for(int i=0;i<6;i++){
            bool ok;
            do{
                ok=true;
                wygrane[i]=rand()%49+1;
                for(int j=0;j<i;j++){
                    if(wygrane[i]==wygrane[j])
                        ok=false;
                }
            }while(!ok);
        }
    }
    void pobierzLiczby(int liczby[6]){
        std::system("cls");
        for(int i=0;i<6;i++){
            bool ok;
            do{
            ok=true;
            std::cout<<"Podaj " << i+1 << " liczbe: ";
            if(!(std::cin>>liczby[i])){
                std::cout<< "\n[Blad] To nie jest liczba! Wpisz cyfre.\n\n";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                ok=false;
                continue;
            }
            std::cout<<std::endl;
            if(liczby[i]<1||liczby[i]>49){
                std::cout << "[Blad] Liczba musi byc z zakresu od 1 do 49!\n\n";
                ok=false;
            }
            else{
                for(int j=0;j<i;j++){
                    if(liczby[i]==liczby[j]){
                        std::cout<<"[Blad] Ta liczba juz zostala podana!~\n\n";
                        ok=false;
                        break;
                        }
                    }
                }
            }while(!ok);
        }
    }
    int sprawdz(int wygrane[6],int liczby[6]){
        int trafienia=0;
        for(int i=0;i<6;i++){
            for(int j=0;j<6;j++){
                if(liczby[i]==wygrane[j]){
                    trafienia++;
                    break;
                }
            }
        }
        return trafienia;
    }
public:
    void gra(Gracz &gracz){
        if(gracz.getSaldo()<kwota){
            std::cout<<"Brak srodkow!\n";
            Sleep(1000);
            return;
        }
        gracz.odejmij(kwota);
        int wygrane[6];
        int liczby[6];

        los_wyg(wygrane);
        pobierzLiczby(liczby);
        int trafienia=sprawdz(wygrane,liczby);
        std::cout<<"\nWylosowane liczby: ";
        for(int i=0;i<6;i++)
            std::cout<<wygrane[i]<< " ";
        std::cout<<"\nTrafienia: "<<trafienia<<std::endl;
        int nagroda=0;
        switch(trafienia){
            case 6:
                nagroda=500000;
                break;
            case 5:
                nagroda=50000;
                break;
            case 4:
                nagroda=100;
                break;
            case 3:
                nagroda=50;
                break;
            default:
                std::cout<<"Brak wygranej\n";
        }
        if(nagroda>0){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
            std::cout<<"\nWYGRANA: "<<nagroda<<" PLN!!!\n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
            gracz.dodaj(nagroda);
        }
        gracz.updateStatystyk(nagroda);
        getch();
    }
};

int main()
{
    Gracz gracz;
    gracz.wczytajZPliku();
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info={1,FALSE};
    SetConsoleCursorInfo(hConsole, &info);

    srand(time(NULL));
    bool kontynuuj=true;
    Slots slots;
    Lotto lotto;
    blackJack stolBlackjacka;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
    std::cout << "------------------" << std::endl;
    //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    std::cout << "|";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    std::cout<<"     KASYNO      ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    std::cout<<"|" << std::endl;
    std::cout << "------------------" << std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    std::cout << "Wczytywanie"; Sleep(500);std::cout<<".";Sleep(500);std::cout<<".";Sleep(500);std::cout<<".";Sleep(250);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    while(kontynuuj)
    {

        std::system("cls");
        char wybor;
        std::cout<<"Witamy w kasynie!"<<std::endl;
        std::cout<<"W co dzisiaj zagrasz?"<< std::endl;
        std::cout<<"1. Slots"<< std::endl;
        std::cout<<"2. Lotto" << std::endl;
        std::cout<<"3. Doladuj saldo" << std::endl;
        std::cout<<"4. Statystyki" << std::endl;
        std::cout<<"5. Blackjack" << std::endl;
        std::cout<<"6. Exit" << std::endl;
        std::cout<<""<<std::endl;
        std::cout<<""<<std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
        std::cout<<"Saldo: "<< gracz.getSaldo()<<"PLN"<<std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);

        wybor=getch();
        switch(wybor)
        {
        case '1':
            slots.Gra(gracz);
            break;
        case '2':
            lotto.gra(gracz);
            break;
        case '3':
            {

            int kasa;
            std::system("cls");
            std::cout<<"Ile chcesz doladowac PLN?: ";
            std::cin>>kasa;
            gracz.dodaj(kasa);
            std::cout<<"\nDodano "<<kasa<<"PLN";
            std::cout<< "\nTwoje saldo: ";
            std::cout<<gracz.getSaldo();
            Sleep(1500);
            }
            break;
        case '4':
            std::system("cls");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),16);
            std::cout<<"Statystyki gracza:\n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),3);
            std::cout<<"Liczba gier:" << gracz.getLiczbaGier()<<std::endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
            std::cout<<"Liczba wygranych: "<<gracz.getLiczbaWygranychGier()<<std::endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),5);
            std::cout<<"Najwyzsza wygrana: "<<gracz.getNajwiekszaWygrana()<<std::endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
            std::cout<<""<<std::endl;
            std::cout<<""<<std::endl;
            std::cout<<""<<std::endl;
            std::cout<<""<<std::endl;
            std::cout<<"Zeby wyjsc do menu wcisnij 'q'.";
            while(true){
                char wyb1=getch();
                if(wyb1=='q'|| wyb1=='Q')
                    break;
            }
            break;
        case '5':
            std::system("cls");
            stolBlackjacka.graj(gracz);
            break;

        case '6':
            {
                gracz.zapiszDoPliku();
            kontynuuj=false;
            break;
            }
        }
    }

    return 0;
}
