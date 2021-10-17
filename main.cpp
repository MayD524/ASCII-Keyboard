/*
 * Author: May Draskovics
 * Date: 10/16/2021
*/

#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <map>

#define NP_0 0x60
#define NP_1 0X61
#define NP_2 0x62
#define NP_3 0x63
#define NP_4 0x64
#define NP_5 0x65
#define NP_6 0x66
#define NP_7 0x67
#define NP_8 0x68
#define NP_9 0x69
#define NP_E 0x6B

using namespace std;

const short numkeys[11] = {NP_0, NP_1, NP_2, NP_3, NP_4, NP_5, NP_6, NP_7, NP_8, NP_9, NP_E};

INPUT ip;
vector<short> buff;
char curGuess;
int curIndex = 0;

map<short, int> keymap = {
	{NP_0, 0},
	{NP_1, 1},
	{NP_2, 2},
	{NP_3, 3},
	{NP_4, 4},
	{NP_5, 5},
	{NP_6, 6},
	{NP_7, 7},
	{NP_8, 8},
	{NP_9, 9},
	{NP_E, 69}
};

bool getKey( const char& keyChar )
{
	short keyState = GetKeyState(keyChar);
	if (keyState & 0x8000)
		return true;
	return false;
}

int getDec()
{
	ostringstream stream;

	for (int b : buff)
	{
		stream << b;
	}

	return stoi(stream.str());
}

void sendInput(WORD k)
{
	ip.ki.wVk = k;
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));

	// header
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}

void guess()
{
	try
	{
		cout << "guess: ";
		for (int c : buff)
			cout << c;
		cout << endl;
		curGuess = getDec();
		cout << "curGuess: " << (int) curGuess << endl;
	}
	catch(invalid_argument& e)
	{
		cout << e.what() << endl;
		return;
	}
}

void confirm_opt()
{
	curGuess = 0x0;
	// reset buffer
	buff = vector<short>();
}

int main( void )
{
	// using dec 000 - 127
	// keys 0-9 (expected)
	Sleep(1000);
	while (true)
	{
		for (int i = 0; i < 11; i++)
		{
			if (getKey(numkeys[i]))
			{
				auto it = keymap.find(numkeys[i]);
				if (it == keymap.end())
				{
					cout << "how the fuck did this happen?\n" << endl;
					return -1;
				}
				cout << it->second << " was pressed!" << endl;
				cout << (int) curGuess << endl;
				sendInput(VK_BACK);

				
				if (it->second == 69 || curIndex == 2)
				{
					guess();
					sendInput(curGuess);
					confirm_opt();
					curIndex = 0;
				}
				else
				{
					buff.push_back(it->second);
					guess();
					cout << (int) curGuess << endl;
					curIndex++;
				}
			}
		}
		// slow pulling rate
		Sleep(80);
	}
}
