#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"Ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <WS2tcpip.h>
#include <cstring>
#include <conio.h>
#include <Windows.h>
#include <tchar.h>
#include <fstream>
#include <thread>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;
char ServerName[30] = ""; char FileName[50] = "", FileName2[50]="";
SOCKET Server1, Server2, Server3, Server4,Server5, Client1, Client2, Client3, Client4,Client5;
bool ok; HWND handle; RECT rect;
cv::Mat image, image1;  cv::Mat exitimage; bool PrtScr = false, Keylog = 0;

int Connection(SOCKET& Server, SOCKET& Client, int port) {
	WSADATA Wsadata;
	int a = WSAStartup(MAKEWORD(2, 2), &Wsadata);
	Server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = port;

	
	HOSTENT* pHostEnt = gethostbyname(ServerName);
	if (pHostEnt == NULL) return SOCKET_ERROR;
	memcpy(&sin.sin_addr, pHostEnt->h_addr_list[0], pHostEnt->h_length);
	connect(Server, (struct sockaddr*)&sin, sizeof(sin));
	return 1;
}
void ReceiveImageFile() {
	while (1) {
		
		int n = 0;
		recv(Server2, (char*)&n, sizeof(int), 0);
		ofstream f("image.jpg", ios::binary); char c;

		for (int i = 0; i < n; i++) {
			recv(Server2, &c, 1, 0);
			f.write(&c, 1);
		}
	f.close();
		Mat image = imread("image.jpg");
		resize(image, image, Size(1920*2/3, 1080*2/3), INTER_LINEAR);
		imshow("Server's Window", image); handle = FindWindowA(NULL, "Server's Window");
		if (PrtScr) {
			string str = FileName + strlen(FileName) - 4;
			if (str != ".jpg")
			{
				str = FileName; str += ".jpg";
			}
			else str = FileName;
			imwrite(str, image);

			PrtScr = 0;
		}
	waitKey(30);

	}

}
void Mouse() {
	POINT p;
	while (1) {
		Sleep(50);

		if (((GetKeyState(VK_LBUTTON) & 0x8000) != 0)) {
			GetCursorPos(&p);
			GetWindowRect(handle, &rect);
			p.x -= rect.left+10; p.y -= rect.top+34;
			if (p.x > 0 && p.y > 0) {
				if (p.x > 909 && p.x < 909+65 && p.y < 50) {
					char b = '0';
					send(Server4, &b, 1, 0);
					exit(1);
				}

				int a[3] = { p.x,p.y,0 };
				send(Server3, (char*)&a[0], 3 * sizeof(int), 0);

			}
		}
		if (((GetKeyState(VK_RBUTTON) & 0x8000) != 0)) {
			POINT p;
			GetCursorPos(&p);
			GetWindowRect(handle, &rect);
			p.x -= rect.left; p.y -= rect.top;
			if (p.x < 0 || p.y < 0) break;
			int a[3] = { p.x,p.y,1 };
			send(Server3, (char*)&a[0], 3 * sizeof(int), 0);
		}
	}
}
void Keyboard() {
	bool Capslock = false; int Win = 0;
	vector <int> at = {8,9,0x0D, 0x13,0x1B };
	for (int i = 0x20; i < 0xDE; i++)
		if (i != 144 && i != 164
			&& i != VK_LMENU && i != VK_RMENU && i != VK_LWIN && i != VK_RWIN
			&& i != VK_LCONTROL && i != VK_RCONTROL && i != VK_LSHIFT && i != VK_RSHIFT)
			at.push_back(i);
	while (1) {
		Sleep(90);
		if (!Capslock && GetKeyState(VK_CAPITAL)) Capslock = 1;
		if (Capslock && !GetKeyState(VK_CAPITAL)) {
			int a[5] = { VK_CAPITAL,0,0,0 ,0 };
			send(Server1, (char*)&a[0], 5 * sizeof(int), 0);
			Capslock = 0;
		}
		if (Win == 0 && (GetAsyncKeyState(VK_LWIN) || GetAsyncKeyState(VK_LWIN))) Win = 1;
		if (!GetAsyncKeyState(VK_LWIN) && !GetAsyncKeyState(VK_LWIN)) {
			if (Win == 1) {
				int a[5] = { VK_RWIN,0,0,0 ,0 };
				send(Server1, (char*)&a[0], 5 * sizeof(int), 0);
			}
			Win = 0;
		}
		for (int i : at)
			if (GetAsyncKeyState(i)) {
				int a[5] = { i,GetAsyncKeyState(VK_SHIFT),GetAsyncKeyState(VK_MENU),
				GetAsyncKeyState(VK_CONTROL),0 };

				if (GetAsyncKeyState(VK_LWIN) || GetAsyncKeyState(VK_LWIN)) {
					Win = 2; a[4] = 1;
				}
				send(Server1, (char*)&a[0], 5 * sizeof(int), 0);
			}
	}
}
void RecvKeylog() {
	string VK[] = { "[SHIFT]","[ALT]","[CTRL]","[WIN]"}; ofstream f;
	int a[5] = { 0,0,0,0,0 };
	while (1) {
		
		recv(Server5, (char*)&a[0], 5 * sizeof(int), 0);
		
		if (Keylog) {
			f.open("temp.txt", ios::app);
		for (int i = 0; i < 4; i++) if (a[i + 1]) f<< VK[i];
		
			switch (a[0]) {
			case 8: f << "BACKSPACE "; break;
			case 9: f << "TAB "; break;
			case 0x0D: f << "ENTER "; break;
			case 0x20: f << "SPACE "; break;
			case 0x22: f << "PGDOWN "; break;
			case 0x21: f << "PGUP "; break;
			case 0x23: f << "END "; break;
			case 0x24: f << "HOME "; break;
			case 0x25: f << "LEFT "; break;
			case 0x26: f << "UP "; break;
			case 0x27: f << "RIGHT "; break;
			case 0x28: f << "DOWN "; break;
			case 0x2C: f << "PRINTSCREEN "; break;
			case 0x2D: f << "INSERT "; break;
			case 0x2E: f << "DELETE "; break;
			default: 
				if (0x30 <= a[0] && a[0] < 0x39) f << a[0] - 0x30<<' ';
				if (0x41 <= a[0] && a[0] < 0x5A) f <<(char)( a[0] - 0x41+'a')<<' ';
				if (0x60 <= a[0] && a[0] < 0x69) f << a[0] - 0x60<<' ';
				if (0x70 <= a[0] && a[0] < 0x7B) f <<'F'<< a[0] - 0x70+1<<' ';
			}
			f.close();
		}


		Sleep(100);
	}

}
void Stop() {
	ifstream f; char b = '0';
	while (1) {
		f.open("C:/Users/Dien/Documents/output.txt");
		if (!f) continue;
		int n;
		f >> n; 
		if (n == 0)
		{
			send(Server4, &b, 1, 0);
			exit(1);
		}
		
		if (n == 1) {
			PrtScr = 1;
			f >> FileName;
		}
		if (n == 2) {
			Keylog = 1; f >> FileName2;
		}
		if (n == 3) {
			remove(FileName2);
			Keylog = 0;
			cout << "Stop Keylog!\n";
			rename("temp.txt", FileName2);
			
		}

		f.close();
		remove("C:/Users/Dien/Documents/output.txt");
		Sleep(100);
	} while (1);
}
void Menu() {
	ifstream f;
	do {
		f.open("C:/Users/Dien/Documents/output.txt");
		if (f) {
			f.read(ServerName, 30);
			f.close();
			remove("C:/Users/Dien/Documents/output.txt");
			int a=Connection(Server1, Client1, 3000);
			if (a == SOCKET_ERROR) {
				ofstream f2("C:/Users/Dien/Documents/output1.txt");
				f2 << "1";
				f2.close();
				
			}
			else {
				remove("C:/Users/Dien/Documents/output1.txt");
				return;
			}

		}
		f.close();
		Sleep(100);
	} while (1);

}

int main() {
	char b = 'b'; 
	remove("C:/Users/Dien/Documents/output.txt");
	remove("C:/Users/Dien/Documents/output1.txt");
	Menu();
	
	Connection(Server2, Client2, 3555);
	Connection(Server3, Client3, 4444);
	Connection(Server4, Client4, 5000);
	Connection(Server5, Client5, 5500);
	thread thr1(Keyboard);
	thread thr2(ReceiveImageFile);
	thread thr3(Mouse);
    thread thr4(Stop);
	thread thr5(RecvKeylog);
	thr1.join();
	thr2.join();
	thr3.join();
	thr4.join();
	closesocket(Server1);
	closesocket(Server2);
	closesocket(Server3);
	closesocket(Server4);
	closesocket(Server5);
	WSACleanup();
	return 0;
}


