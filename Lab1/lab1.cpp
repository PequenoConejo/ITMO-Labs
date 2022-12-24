// lab1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string.h>
#include <ctime>
#include <fstream>

using namespace std;

struct City {
    int acresArea; // акры земли в целом
    int population; // население города
    int bushelsWheat; // бушели пшеницы в запасе
    int acresForWheat = 0; // акры земли под пшеницу в этом году - изначально 0
    string name; // название города
};

// сколько жителей умрет от голода
int dieStarving(int wheat, int population) {
    if (population*20 > wheat) // если пшеницы меньше чем требуется жителям
        return (population - (wheat / 20)); // вернем столько скольким не хватило
    return 0; // ну или ноль если хватило всем
}

// сколько приходит в город
int comeToCity(int dead, int wheatFromAnAcre, int wheat) {
    int res = dead / 2 + (5 - wheatFromAnAcre) * wheat / 600 + 1; // считаем по формуле
    if (res < 0) res = 0; // ограничения
    if (res > 50) res = 50;
    return res;
}

// сколько человек умерло от чумы
int deadFromPlague(int population) {
    srand(time(0));
    int prob = rand() % 101; // считаем вероятность чумы
    // если чума случилась, отрезаем от населения половину и хвостик для округления
    if (prob <= 15) return ((population / 2) + (population % 2));  
    else return 0;
}

// сколько бушелей съели крысы
int bushelsAteByRats(int bushelsWheat) {
    srand(time(0));
    float ate = (rand() % 8) * 0.01; // доля съеденного
    return (ate * bushelsWheat);
}

// все ли окей с введенными параметрами
bool interactive(City city, int acresSell, int acresBuy, int bushelsForEat, int acresForSow, int acrePrice) {
    if ((acresSell > city.acresArea) || (acresSell < 0)) {
        cout << "Ne ok s prodajei" << endl;
        return false;
    }
    if (((acresBuy * acrePrice) > city.bushelsWheat)||(acresBuy<0)) {
        cout << "Ne ok s pokupkoi: acrePrice = " << acrePrice << "city wheat = " << city.bushelsWheat << " hochesh kupit' " << acresBuy << " no pri etom vozmojnost' = " << ((acresBuy * acrePrice) < city.bushelsWheat) << endl;
        return false;
    }
    if ((bushelsForEat < 0) || (bushelsForEat > (city.bushelsWheat + acrePrice * acresSell))) {
        cout << "Ne ok s edoi" << endl;
        return false;
    }
    if (((acresForSow * 0.5) > (city.bushelsWheat - bushelsForEat + (acrePrice * acresSell))) || (acresForSow > (city.acresArea + acresBuy - acresSell)) || (acresForSow < 0)) {
        cout << "Ne ok s seianiem" << endl;
        return false;
    }
    return true;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    City city; int i = 1;
    city.name = "Рим";
    city.acresArea = 1000;
    city.bushelsWheat = 2800;
    city.population = 100;

    int acresSell;
    int acresBuy;
    int bushelsForEat = city.bushelsWheat;
    int acresForSow = 0;
    bool notOkayInteractive = true;
    float deadAll[10];

    fstream file;
    file.open("F:\\Labs\\1\\Save.txt");
    char A;
    // что записываем в файл - номер раунда, показатели города: площадь, пшеница, население
    if (!(file.peek() == EOF)) {
        cout << "------------------------------------" << endl;
        cout << "Найдено сохранение. Загрузить? Y/N:" << endl;
        A = getchar();
        if (A == 'Y') {
            file >> i >> city.acresArea >> city.bushelsWheat >> city.population;
            cout << "Сохранение загружено!" << endl;
            cout << "------------------------------------" << endl;
        }
    }
    file.close();
    

    for (i; i < 11; i++) {
        cout << "Приветствую тебя, славный правитель города " << city.name << ", сейчас идет " << i << " год твоего правления." << endl;
        int wheatFromAnAcre = (rand() % 6) + 1;
        int deadThisYear = dieStarving(bushelsForEat, city.population);
        int newcomersThisYear = comeToCity(deadThisYear, wheatFromAnAcre, city.bushelsWheat);
        int deadThisYearPlag = deadFromPlague(city.population);
        if (deadThisYear) {
            if (deadThisYear >= 0.45 * city.population) {
                cout << "   В этом году слишком много смертей... Вы проиграли." << endl;
                break;
            }
            cout << "   В этом году от голода умерло " << deadThisYear << " человек." << endl;
            city.population -= deadThisYear;
        }
        if (newcomersThisYear) {
            cout << "   В этом году в город прибыло " << newcomersThisYear << " человек." << endl;
            city.population += newcomersThisYear;
        }
        if (deadThisYearPlag) {
            cout << "   В этом году в городе свирепствовала чума" << endl;
            city.population -= deadThisYearPlag;
        }
        cout << "   Сейчас в городе живет " << city.population << " человек" << endl;
        if ((wheatFromAnAcre != 0)&&(city.acresForWheat != 0)) {
            cout << "   Пшеницы собрано с акра " << wheatFromAnAcre << endl;
            int wheatThisYear = city.acresForWheat * wheatFromAnAcre;
            cout << "   Всего собрано пшеницы " << wheatThisYear << endl;
            city.bushelsWheat += wheatThisYear;
        }
        int bushelsAte = bushelsAteByRats(city.bushelsWheat);
        if (bushelsAte) {
            cout << "   К сожалению, крысы уничтожили " << bushelsAte << " бушелей пшеницы" << endl;
            city.bushelsWheat -= bushelsAte;
        }
        cout << "   Сейчас в запасах есть " << city.bushelsWheat << " бушелей пшеницы." << endl;
        int acrePrice = (rand() % 10) + 17;
        cout << "   Сейчас город занимает " << city.acresArea << " акров земли" << endl;
        cout << "   Цена покупки акра земли в этом году: " << acrePrice << " бушелей пшеницы за акр." << endl;
        notOkayInteractive = true;
        cout << "------------------------------------" << endl;
        cout << "Желаете сохранить прогресс? (Действие перезапишет файл сохранения) Y/N:" << endl;
        cin >> A;
        if (A == 'Y') {
            file.open("F:\\Labs\\1\\Save.txt");
            file << i << " " << city.acresArea << " "  << city.bushelsWheat << " " << city.population;
            cout << "Файл сохранения успешно перезаписан" << endl;
            file.close();
        }
        cout << "------------------------------------" << endl;
        /*int acresSell;
        int acresBuy;
        int bushelsForEat;
        int acresForSow;
        bool notOkayInteractive = true;*/
        while (notOkayInteractive) {
            cout << "Сколько акров земли повелеваешь купить?" << endl;
            cin >> acresBuy;
            cout << "Сколько акров земли повелеваешь продать?" << endl;
            cin >> acresSell;
            cout << "Сколько бушелей пшеницы повелеваешь съесть?" << endl;
            cin >> bushelsForEat;
            cout << "Сколько акров земли повелеваешь засеять?" << endl;
            cin >> acresForSow;
            notOkayInteractive = (!(interactive(city, acresSell, acresBuy, bushelsForEat, acresForSow, acrePrice)));
            if (notOkayInteractive) {
                cout << "   Но повелитель, пощади нас! У нас всего " << city.population << " людей, " << city.bushelsWheat << " бушелей пшеницы и " << city.acresArea << " акров земли!" << endl;
            }
        }
        city.acresArea = city.acresArea + acresBuy - acresSell;
        city.acresForWheat = acresForSow;
        city.bushelsWheat -= acresBuy * acrePrice;
        city.bushelsWheat += acresSell * acrePrice;
        city.bushelsWheat -= bushelsForEat;
        city.bushelsWheat -= acresForSow * 0.5;

        deadAll[i - 1] = ((deadThisYear + deadThisYearPlag) * 0.01) * city.population;
    }
  
    float deadAllAverage = 0;
    for (int i = 0; i < 10; i++) {
        deadAllAverage += deadAll[i];
    }
    deadAllAverage /= 10;
    if (deadAllAverage < 0) deadAllAverage = 0;
    float acresPerCitizen = city.acresArea / city.population;

    cout << "Твое правление подошло к концу, о великий!" << endl;
    cout << "   Сейчас в городе живет " << city.population << " человек" << endl;
    cout << "   Сейчас город занимает " << city.acresArea << " акров земли" << endl;
    cout << "Совет старейшин постановил: " << endl;

    if ((deadAllAverage > 33) || (acresPerCitizen < 7))
        cout << "   Вы некомпетентны как правитель, и люди страдают. Решено было изнать вас из города!" << endl;
    else if ((deadAllAverage > 10) || (acresPerCitizen < 9))
        cout << "   Вы правили жестокой рукой, и люди, выдохнув, больше не желают видеть вас у власти!" << endl;
    else if ((deadAllAverage > 3) || (acresPerCitizen < 10))
        cout << "   Вы справились вполне неплохо, и старейшины предлагают вам снова занять пост главы города (если мы не найдем кого получше, разумеется)." << endl;
    else
        cout << "   Великолепное правление! Ваше имя впишут в историю, чтобы устыдить тех, кто посмеет править не столь же искусно." << endl;

    cout << ">> Среднегодовой процент умерших " << deadAllAverage << endl;
    cout << ">> Акров на человека " << acresPerCitizen << endl;
    cout << "------------------------------------" << endl;
    cout << "Очистить файл сохранения? Y/N:" << endl;
    cin >> A;
    if (A == 'Y') {
        file.open("F:\\Labs\\1\\Save.txt", std::ios::out);
        file.close();
        cout << "Файл сохранения очищен для новых игр!" << endl;
    }
    else
        cout << "Файл сохранения не изменен." << endl;
    cout << "------------------------------------" << endl;

}

