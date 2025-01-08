//если всё, что делает /subsystem:console -- это создаёт буферы по умолчанию, то переключить на другое (но а если он еще и в ответе за окно и ещё что-нибудь?)
//error handling

//This project files includes:
#include "BookStack.h"
#include "ConsoleDrawer.h"
#include "resource.h"

//This solution common files includes:
#include "WindowsErrorInfoShowman.h"

HINSTANCE hInstance;
HWND hwnd;

int main()
{
	hInstance = GetModuleHandle(NULL);
	hwnd = GetConsoleWindow();

	WEISH::Initialize(hwnd);

	HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	console_drawer::Initialize();

	//
	//класс консольное изображение, с конструтором, аргумент которого -- jpeg
	//консольное изображение проще вывести
	console_drawer::ConsoleImage image(IDR_JPEG1);
	//size из объекат взять
	console_drawer::DrawImage(image, { console_drawer::bufferSize.X, console_drawer::bufferSize.Y }, { 0,0 });
	console_drawer::Present();

	//возможность двигать окно пропала
	//дописать апи функции для художника, включая передвижение по заданным точкам (анимация по траектории)
	//ico пиксельная книжки
	//создать звукорежиссера
	//стек под книжный переделать
	//BookStack<int> stackEx[10];
	//в common добавить выход из консоли через esc
	
	//BookStack<int> bs;
	//bs.push(5);
	//bs.pop();
	//bs.push(7);
	while (true) 
	{
		
	}
	console_drawer::Uninitialize();
}

