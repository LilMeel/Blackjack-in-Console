#include <iostream>
#include <random>
#include <stdlib.h>
#include <sstream>
#include <time.h>

using namespace std;

struct cards{
	int digit;
	char suit;
};

int balance = 1000,choise=0,itterations = 0,bet,exit_button,lenLastCards = 0,start_pos,doub;
string main_digit[3];
int lastCards[20];

int scoring_points(int digit_any, int sum){ // Функция подсчёта карт
	if (digit_any < 10){
		 sum += digit_any;
	}else if(digit_any != 14){
		sum += 10;
	}else{
		if (sum + 11 > 21){ // Проверка на очки туза (1 или 11)
			sum += 1;
		} else{
			sum += 11;
		}
	}
	return sum;
}

string intToStr(int a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}

int generate_card(int sum,int itterations){
	char temp_suit;
	string pic[3] = {"J","Q","K"}, rand_pic;
	temp_suit = 3+rand()%3;
	struct cards card1{2+rand()%13,temp_suit};
	lastCards[lenLastCards] = card1.digit;
	lastCards[lenLastCards+1] = card1.suit;
	lenLastCards+=2;
	if (card1.digit > 10 && card1.digit != 14){
		rand_pic = pic[rand() %3];
		main_digit[2] = string(rand_pic) + card1.suit;
		// cout << rand_pic << card1.suit << endl;
	} else if(card1.digit == 14){
		main_digit[2]= string("A")+card1.suit;
		// cout << "A" << card1.suit << endl;
	} else{
		main_digit[2] = intToStr(card1.digit)+card1.suit;
		// cout << card1.digit << card1.suit << endl;
	}
	
	if (start_pos == 0){
		main_digit[itterations] = main_digit[2];
	}else{
		main_digit[itterations] += main_digit[2];
	}
	sum = scoring_points(card1.digit,sum);
	
	return sum;
}

int end(int sum_player, int sum_bot){
	while (sum_bot < sum_player){
		sum_bot = generate_card(sum_bot, 0);
		cout << main_digit[0] << " " << sum_bot << endl;
	}
	cout << "-------------------------------" << endl;
	cout << main_digit[0] << " " << sum_bot << endl;
	cout << main_digit[1] << " " << sum_player << endl;
	cout << "-------------------------------" << endl;
	
	return sum_bot;
}

void bet_count(int sum_player,int sum_bot){
	if (sum_player > 21){ // Автолуз при кол-ве очков > 21
		cout << "You are looser!" << endl;
		balance -= bet;
		cout << "Your balance is " << balance << endl;
	}else if(sum_player < 21){ // Варианты после распределения всех карт
		if (sum_player > sum_bot || sum_bot > 21){
			cout <<  "You are winner!" << endl;
			balance += bet;
			cout << "Your balance is " << balance << endl;
		}else if(sum_player < sum_bot){
			cout << "You are looser" << endl;
			balance -= bet;
			cout << "Your balance is " << balance << endl;
		}else{
			cout << "It's a draw";		
		}
	}else{
		balance += 1.5 * bet;
		cout << endl << "Blackjack! - " << balance << endl;
	}
}

int game(){
	int sum_player = 0, sum_bot = 0,doub = 0,choise = 0;
	start_pos = 0;
	system("cls");
	
	while (true){ // Проверка на ввод тайпа переменной
		cout << "Your balance is " << balance << endl << "Place a bet, please - "; // Ставка
		cin >> bet;
		if(cin.fail()){
			cin.clear();
			cin.ignore(32767,'\n');
		} else if(bet > balance){
			cout << "You can't bet" << endl;
		} else{
			cout << "The bet is made" << endl;
			break; 	
		}
	}
	
	sum_bot = generate_card(sum_bot,0); // Раздача карт и подсчёт сумм
	cout << main_digit[0] << "**" << " " << sum_bot << endl;
	for (int i=0;i<2;i++){
		sum_player = generate_card(sum_player,1);
		start_pos = 1;
	}
	cout << main_digit[1] << " " << sum_player << endl;
	
	if(sum_player == 21){ // Когда выпадает blackjack
		bet_count(sum_player,sum_bot);
		return 0;
	}
	
	while (sum_player <= 21){ // Выбор после начальной раздачи
		if (choise == 0){
			cout << "1 - Enough, " << "2 - One More, " << "3 - Double" << endl;
		}else{
			cout << "1 - Enough, " << "2 - One More" << endl;
		}
		
		cin >> choise;
		if (choise == 2){ // Добор карт
			sum_player = generate_card(sum_player,1);
			cout << main_digit[1] << " " << sum_player << endl;
			if (sum_player == 21){
				sum_bot = end(sum_player,sum_bot);
			break;
			}
		}else if(choise == 1){
			sum_bot = end(sum_player,sum_bot);
			break;
		}else if(choise = 3 && doub == 0 && balance-(bet*2) > 0){
			bet *= 2;
			sum_player = generate_card(sum_player,1);
			cout << main_digit[1] << " " << sum_player << endl;
			if (sum_player <= 21){
				end(sum_player,sum_bot);
				bet_count(sum_player,sum_bot);
				return 0;
			}
		} else{
			cout << "Your balance doesn't allow this bet" << endl;
			continue;
		}
		doub += 1;
	}
	bet_count(sum_player,sum_bot);
	return 0;
}

void respawn(){
	cout << endl << "Again, noob?" << endl << "1 - Let's go. 2 - Exit: "; // Рестарт игры
	cin >> exit_button;
	cout << endl;
}

int main(){
	srand(time(NULL));
	game();
	while(exit_button != 2){ // Продолжение игры или выход из неё
		if (balance == 0){ // Проверка на баланс
			cout << "Money is over(";
			return 0;
			}
		respawn();
		if (exit_button == 1){
			for(int i;i<lenLastCards;i++) lastCards[i] = 0;
			game();
		}
		else{
			continue;
		}
	}
	return 0;
}