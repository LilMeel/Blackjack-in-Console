#include <iostream>
#include <random>

using namespace std;

random_device rd; // Добавление функции рандома rd()
mt19937 mersenne(rd());

string digits[13] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"}; // Варианты фигур
char suits[4] = {3,4,5,6}; // Варианты мастей
string digit1,digit2,suit1,suit2; // Переменная для раздачи бота - 0, игрока - 1 // рандомные карты
unsigned int balance = 1000,choise,itterations = 0,digit_pos1,digit_pos2, bet,exit_button;
string main_digit[3];

int scoring_points(int digit_any, int &sum){ // Функция подсчёта карт
	if (digit_any < 9) sum += digit_any + 2;
	else if(digit_any != 12){
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

int begining(int sum){ // Функция раздачи карт

	suit1 = suits[rd() % 3];
	digit_pos1 = rd() % 12;
	digit1 = digits[digit_pos1];
	
	if (itterations != 0){
		suit2 = suits[rd() % 3];
		digit_pos2 = rd() % 12;
		digit2 = digits[digit_pos2];
		
		while (suit1 == suit2 && digit1 == digit2){ // Проверка на получение одинаковых карт
			digit_pos1 = rd() % 12;
			digit1 = digits[digit_pos1];
			suit1 = suits[rd() % 3];
		}
		if (digit_pos1 == digit_pos2){ // Если числа совпадают
			scoring_points(digit_pos1,sum);
			sum *= 2;
			// similar += 1; // Написать перменнную, если выпали одинаковые числа
		}else{
			scoring_points(digit_pos1,sum);
			scoring_points(digit_pos2,sum);
		}
		main_digit[itterations] = string(digit1)+string(suit1)+string(digit2)+string(suit2); // Вывод всех карт в руке
	} else{
		main_digit[itterations] = string(digit1)+string(suit1); // Раздача на бота
		scoring_points(digit_pos1,sum);
		itterations += 1;
	}
	return sum;
	
}

int newCard(int sum,int player){  // Функция добора новой карты

	suit1 = suits[rd() % 3];
	digit_pos1 = rd() % 12;
	digit1 = digits[digit_pos1];
	main_digit[player] += string(digit1)+string(suit1);
	
	scoring_points(digit_pos1, sum); // Подсчёт суммы с новой картой
	cout << main_digit[player] << " " << sum << endl;
	return sum;
}

int end(int sum_player, int sum_bot){
	while (sum_bot < sum_player){
		sum_bot = newCard(sum_bot, 0);
	}
	cout << endl << "-------------------------------" << endl;
	cout << main_digit[itterations] << " " << sum_player << endl;
	
	return sum_bot;
}

int game(){
	short int sum_player = 0, sum_bot = 0; 
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
	
	sum_bot = begining(sum_bot);
	cout << main_digit[itterations-1] << "**" << " " << sum_bot << endl;
	sum_player = begining(sum_player); 
	cout << main_digit[itterations] << " " << sum_player << endl;
	
	if(sum_player == 21){ 
		cout << "Blackjack! - " << sum_player;
		return 0;
	}
		while (sum_player <= 21){ // Выбор после начальной раздачи
			cout << "1 - Enough, " << "2 - One More " << endl;
			cin >> choise;
			if (choise == 2){
				sum_player = newCard(sum_player,1);
				if (sum_player == 21){
					sum_bot = end(sum_player,sum_bot);
				break;
				}
			}else if(choise == 1){
				sum_bot = end(sum_player,sum_bot);
				break;
			} else{
				cout << "Incorrect num" << endl;
				continue;
				}
		}
	if (sum_player > 21){ // Автолуз при кол-ве очков > 21
			cout << "You are looser!" << endl;
			balance -= bet;
			cout << "Your balance is " << balance << endl;
		}else{ // Варианты после распределения всех карт
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
	}
	return 0;
}

int respawn(){
	cout << endl << "Again, noob?" << endl << "1 - Let's go. 2 - Exit: "; // Рестарт игры
	cin >> exit_button;
	cout << endl;
	return exit_button;
}

int main(){
	game();
	while(exit_button != 2){ // Продолжение игры или выход из неё
		if (balance == 0){ // Проверка на баланс
			cout << "Money is over(";
			return 0;
			}
		respawn();
		if (exit_button == 1){
			game();
		}else{
			continue;
		}
	}
	return 0;
}