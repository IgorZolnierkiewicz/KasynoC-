#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <time.h>
#include <conio.h>

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

};

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
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info={1,FALSE};
    SetConsoleCursorInfo(hConsole, &info);

    srand(time(NULL));
    bool kontynuuj=true;
    Gracz gracz;
    Slots slots;
    Lotto lotto;
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
        std::cout<<"5. Exit" << std::endl;
        std::cout<<""<<std::endl;
        std::cout<<""<<std::endl;
        std::cout<<"Saldo: "<< gracz.getSaldo()<<"PLN"<<std::endl;

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
            {
            kontynuuj=false;
            break;
            }
        }
    }

    return 0;
}
