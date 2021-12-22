#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <sstream>
#include <time.h>

using namespace std;

struct cards{
	int digit;
	char suit;
};

map <string,int> mp;

unsigned int balance,choise=0, bet, exit_button, lenLastCards = 0, doub, aces[2];
string main_digit[3], lastCards[30], temp_nick;

void delay(int ms) 
{
  int c = clock() + ms;
  while (clock() < c);
}

void scoring_points(int digit_any, int &sum, int player){ 
	if (digit_any < 10){
		 sum += digit_any;
	}else if(digit_any != 14){
		sum += 10;
	}else{
		if (sum + 11 > 21){ 
			sum += 1;
		} else{
			sum += 11;
			aces[player] += 1;
		}
	}
}

string intToStr(int a){
    ostringstream temp;
    temp<<a;
    return temp.str();
}

bool check_repeats(string card){
	for (int i;i<lenLastCards;i++){
		if (card == lastCards[i]){
			return 0;
		}
	}
	return 1;
}

void generate_card(int &sum,int itterations, bool start_pos){
	char temp_suit;
	string pic[3] = {"J","Q","K"}, rand_pic;
	temp_suit = 3+rand()%3;
	struct cards card1{2+rand()%13,temp_suit}; 
	if(!check_repeats(intToStr(card1.digit)+card1.suit)){
		generate_card(sum, itterations,start_pos);
	}
	
	lastCards[lenLastCards] += intToStr(card1.digit) + card1.suit; // массив с картами, которые уже были
	lenLastCards+=1;
	
	if(card1.digit == 14){
		main_digit[2] = string("A")+card1.suit;
	}else if (card1.digit > 10){
		rand_pic = pic[rand() %3];
		main_digit[2] = string(rand_pic) + card1.suit;
	}else{
		main_digit[2] = intToStr(card1.digit)+card1.suit;
	}
	
	if (!start_pos){
		main_digit[itterations] = main_digit[2];
	}else{
		main_digit[itterations] += main_digit[2];
	} 
	
	scoring_points(card1.digit,sum,itterations);
	
	if(sum > 21 && aces[itterations]){
		aces[itterations]-= 1;
		sum -= 10;
	}
}

void end(int &sum_player, int &sum_bot){
	while (sum_bot < sum_player && sum_bot > 14){
		generate_card(sum_bot, 0,true);
		cout << main_digit[0] << " " << sum_bot << endl;
	}
	cout << "-------------------------------" << endl;
	cout << main_digit[1] << " " << sum_player << endl;
	cout << main_digit[0] << " " << sum_bot << endl;
	cout << "-------------------------------" << endl;
	
}

void bet_scores(int sum_player,int sum_bot){
	if (sum_player > 21){ 
		cout << "You are looser!" << endl;
		balance -= bet;
		cout << "Your balance is " << balance << endl;
	}else if(main_digit[1][5] == 0 && sum_player == 21){
		balance += 1.5 * bet;
		cout << endl << "Blackjack! - " << balance << endl;
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
			cout << "It's a draw" << endl;		
		}
	}
}

int game(){
	bool start_pos = false;
	int doub = 0,choise = 0;
	int sum_player = 0,sum_bot = 0;
	
	for(int i = 0; i < 2;i++){
		aces[i] = 0;
	}
	
	system("cls");
	
	while (true){ 
		cout << "Your balance is " << balance << endl << "Place a bet, please - "; 
		cin >> bet;
		if(cin.fail()){
			cin.clear();
			cin.ignore(32767,'\n');
			cout << "You can't bet" << endl;
		}else if(bet > balance || bet < 0){
			cout << "You can't bet" << endl;
		}else{
			cout << "The bet is made" << endl;
			break; 	
		}
	}
	
	generate_card(sum_bot,0,start_pos); 
	cout << main_digit[0] << "**" << " " << sum_bot << endl;
	
	for (int i=0;i<2;i++){
		generate_card(sum_player,1,start_pos);
		start_pos = true;
	}
	
	cout << main_digit[1] << " " << sum_player << endl;
	
	if(sum_player == 21){ 
		bet_scores(sum_player,sum_bot);
		return 0;
	}

	while (sum_player <= 21){ 
		if (choise == 0){
			cout << "1 - Enough, " << "2 - One More, " << "3 - Double" << endl;
		}else{
			cout << "1 - Enough, " << "2 - One More" << endl;
		}
		
		cin >> choise;
		
		if (choise == 2){ 
			generate_card(sum_player,1,start_pos);
			cout << main_digit[1] << " " << sum_player << endl;
			if (sum_player == 21){
				end(sum_player,sum_bot);
				break;
			}
		}else if(choise == 1){
			end(sum_player,sum_bot);
			break;
		}else if(choise == 3 && doub == 0 && bet*2 < balance){ // Дабл
			bet *= 2;
			generate_card(sum_player,1,start_pos);
			cout << main_digit[1] << " " << sum_player << endl;
			if (sum_player <= 21){
				end(sum_player,sum_bot);
				bet_scores(sum_player,sum_bot);
				return 0;
			}
		}else{
			cout << "Your balance doesn't allow to be this count!" << endl;
			choise = 0;
			continue;
		}
		doub += 1;
	}
	bet_scores(sum_player,sum_bot);
	return 0;
}

void respawn(){
	ofstream ofs;
	ofs.open("Saves.txt", ofstream::out | ofstream::trunc);
	if(balance) mp[temp_nick] = balance;
	else mp.erase(temp_nick);
	map <string,int> :: iterator it = mp.begin();
	for(;it != mp.end();it++){
		cout << it->first << " " << it->second << endl;
		ofs << it->first << " " << it->second << endl;
	}
	ofs.close();
	if(!balance) return;
	while (true){
		cout << endl << "Again, noob?" << endl << "1 - Let's go. 2 - Exit: "; // Рестарт игры
		cin >> exit_button;
		if(cin.fail()){
			cin.clear();
			cin.ignore(32767,'\n');
		}else{
			cout << endl;
			break;
		}
	}
}

int accounts(){
	fstream inf("Saves.txt", ios::in | ios::app);
	string nickname, strInput, input_balance, input_nick;
	int change;
	cout << "Enter your last nickname or sign in a new player: ";
	cin >> nickname;

	if (!inf){
		cerr << "Uh oh, something wrong!" << endl;
		exit(1);
	}
	getline(inf, strInput);
	while (inf){
		input_balance = "", input_nick = "";
		change = 0, balance = 1000;
		for(int i = 0;i < strInput.length();i++){
			if(strInput[i] == ' '){
				change += 1;
			}else if (change == 0){
				input_nick += strInput[i];
			}else if(isdigit(strInput[i]) != 0){
				input_balance += strInput[i];
			}
		}
		cout << input_nick << endl;
		delay(1000);
		if(input_nick == nickname){
			cout << "Welcome back, " << input_nick << endl;
			temp_nick = input_nick;
		}
		mp[input_nick] = stoi(input_balance);
		strInput.clear();
		getline(inf, strInput);
	}
	if(temp_nick != ""){
		return mp[temp_nick];
	}
	temp_nick = nickname;
	return 1000;
}

int main(){
	srand(time(NULL));
	balance = accounts();
	game();
	while(exit_button != 2){ 
		if (balance == 0){ 
			cout << "Money is over, looser" << endl;
			respawn();
			return 0; 
		}
		respawn();
		if (exit_button == 1){
			for(int i;i<lenLastCards;i++) lastCards[i] = "";
			lenLastCards = 0;
			game();
		}
		else{
			continue;
		}
	}
	return 0;
}
 