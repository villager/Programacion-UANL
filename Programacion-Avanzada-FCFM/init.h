#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
using namespace std;

struct Lib {
	string to_string(char* arg) {
		string s;
		for (int i = 0; arg[i] != '\0'; i++) {
			s += arg[i];
		}return s;
	}
	void ok(string title, string body) {
		MessageBox(NULL, title.c_str(), body.c_str(), MB_OK);
	}
	bool isClicked(WPARAM wParam, int ID) {
		if (LOWORD(wParam) == ID && HIWORD(wParam) == BN_CLICKED) return true;
		return false;
	}
	bool isNumber(char* arg) {
		int x = 0, y = 0;
		for (x = 0; x < strlen(arg); x++) {
			if (isdigit(arg[x])) y++;
		}
		if (y >= x) return true;
		return false;
	}
} Lib;

struct User {
	char name[19];
	char password[50];
} users[2];


struct Login {
	void init() {
		ifstream lectura;
		lectura.open("db.txt", ios::binary);
		if (lectura.is_open()) {
			while (!lectura.eof()) {
				lectura.read((char*)&users, sizeof(users));
			}
		}
	}
	void add() {
		strcat_s(users[0].name, "admin");
		strcat_s(users[0].password, (users[0].name));
		strcat_s(users[1].name, "user");
		strcat_s(users[1].password, (users[1].name));
		this->save();
	}
	void save() {
		ofstream archivo;
		archivo.open("db.txt", ios::binary);
		archivo.write((char*)&users, sizeof(users));
		archivo.close();
	}
	bool check(char* name, char* pass) {
		for (int i = 0; i < 3; i++) {
			if (strcmp(users[i].name, name) == 0 && strcmp(users[i].password, (pass)) == 0) {
				return true;
			}
		}
		return false;
	}
	char* encrypth(char* pass) {
		for (int i = 0; i < strlen(pass); i++) {
			pass[i] + 12;
		}
		return pass;
	}
	string* split(string arg, char token) {
		int* tokenPosition, tokenCount = 0, max = arg.size();
		string* args;
		// Get all tokens
		for (int i = 0; i < max; i++) {
			if (arg[i] == token) tokenCount++;
		}
		// Get all positions
		tokenPosition = new int[tokenCount + 1];
		tokenCount = 0;
		for (int i = 0; i < max; i++) {
			if (arg[i] == token) {
				tokenPosition[tokenCount] = i;
				tokenCount++;
			}
		}
		args = new string[tokenCount == 0 ? 2 : tokenCount + 1];
		if (tokenCount == 3) {
			args[0] = arg.substr(0, tokenPosition[0]);
			args[1] = arg.substr(tokenPosition[0] + 1, tokenPosition[1] - 1);
			args[2] = arg.substr(tokenPosition[1] + 1, tokenPosition[2] - 1);
			args[3] = arg.substr(tokenPosition[2] + 1, max);
		}
		else {
			for (int i = 0; i + 1 < tokenCount; i++) {

				if (i == 0 && i != arg.size()) {
					args[i] = arg.substr(0, tokenPosition[i]);
				}
				else if (i == tokenCount - 1) {
					args[i] = arg.substr(tokenPosition[i] + 1, max);
				}
				else {
					args[i] = arg.substr(tokenPosition[i], tokenPosition[i + 1] + 1);
				}
			}
		}
		return args;
	}
} Login;

void Image(HWND hWnd, WPARAM wParam, char* imagen, int m, int n) {

	static HBITMAP bmp1, bmp2;

	bmp1 = (HBITMAP)SendDlgItemMessage(hWnd, wParam, STM_GETIMAGE, IMAGE_BITMAP, 0);
	bmp2 = (HBITMAP)LoadImage(NULL, imagen, IMAGE_BITMAP, m, n, LR_LOADFROMFILE);

	SendDlgItemMessage(hWnd, wParam, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);

}