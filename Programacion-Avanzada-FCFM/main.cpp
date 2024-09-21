#include "init.h"
#include "resource.h" // LOCALES
#include <time.h>
#include <commctrl.h>

int globalId = 1, appoimentsCount();
bool isChange = false, isDown = false;
struct Appoiment {
	string date, hour, name, petName, phone;
	int id, status, reason, specie, gender;
	Appoiment* next, * prev;
	string pic[2];
} *aux, *origin;

HWND listItems;
HINSTANCE _hInst;
string vetName = "Aldair Beltran";
HWND hLogin;
HWND hPcLantern;
HBITMAP hBmpLantern;
char picDoc[_MAX_PATH];


void loadDb(), saveDb();

BOOL CALLBACK principalFunction(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK eventAdd(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK eventLogin(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK eventDelete(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK eventList(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK eventInfo(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInstance,PSTR cmdLine, int showCmd)
{
	aux = origin = NULL;
	loadDb();
	Login.init();
	_hInst = hInst;
	hLogin = CreateDialog(hInst, MAKEINTRESOURCE(IDD_LOGIN_MAIN), 0, eventLogin);
	//SetTimer(hMain,TM_RELOJ,1000,NULL);
	MSG msg;
	ShowWindow(hLogin, SW_SHOW);

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
#pragma region Fn_Menu
void eventsMenu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (Lib.isClicked(wParam, MENU_HOME)) {
		EndDialog(hwnd, 0);
		DialogBox(_hInst, MAKEINTRESOURCE(IDD_MAIN), 0, principalFunction);
	} else if (Lib.isClicked(wParam, ID_SALIR)) {
		if (isChange && MessageBox(hwnd, "Desea guardar los cambios efectuados?", "Salir", MB_YESNO) == IDYES) {
			saveDb();
			// GUARDAR
		}
		isDown = true;
		DestroyWindow(hwnd);
	}
	// Eliminar
	else if (Lib.isClicked(wParam, ID_CITAS_ELIMINAR)) {
		// agregar funcion de delete
		if (appoimentsCount() == 0) {
			MessageBox(hwnd, "No hay objetos registrados", "Error", MB_ICONWARNING);
		}
		else {
			EndDialog(hwnd, 0);
			DialogBox(_hInst, MAKEINTRESOURCE(IDD_DELETE), 0, eventDelete);
		}

	}
	// Informacion
	else if (Lib.isClicked(wParam, ID_INFORMACION)) {
		EndDialog(hwnd, 0);
		DialogBox(_hInst, MAKEINTRESOURCE(IDD_ABOUT), 0, eventInfo);
	}
	else if (Lib.isClicked(wParam, ID_CITAS_EDITAR)) {
		EndDialog(hwnd, 0);
		DialogBox(_hInst, MAKEINTRESOURCE(IDD_ADD_ITEM), 0, eventAdd);
	}
	// Agregar
	else if (Lib.isClicked(wParam, ID_CITAS_AGREGAR)) {
		EndDialog(hwnd, 0);
		DialogBox(_hInst, MAKEINTRESOURCE(IDD_ADD_ITEM), 0, eventAdd);
	}
	else if (Lib.isClicked(wParam, ID_CITAS_LISTADO)) {
		EndDialog(hwnd, 0);
		DialogBox(_hInst, MAKEINTRESOURCE(IDD_DIALOG2), 0, eventList);
	}
	else if (Lib.isClicked(wParam, BN_DOC_PIC)) {
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.hwndOwner = hwnd;
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFile = picDoc;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = "bmp";
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrFilter = "BMP Files\0*.bmp\0All Files\0*.*\0";
		if (GetOpenFileName(&ofn)) {
			Lib.ok(picDoc, picDoc);
			Image(hwnd, DOC_PIC_2, picDoc, 100, 100);
		//	hPcLantern = GetDlgItem(hwnd, DOC_PIC);
		//	hBmpLantern = (HBITMAP)LoadImage(NULL, chDirFile, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
		//	Lib.ok(chDirFile, chDirFile);
		//	SendMessage(hPcLantern, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmpLantern);
		}
	}
}
#pragma endregion

int appoimentsCount() {
	int count = 0;
	while (aux != NULL) {
		aux = aux->next;
		count++;
	}
	aux = origin;
	return count;
}
BOOL CALLBACK principalFunction
(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG: {
		char countC[3];
		_itoa_s(appoimentsCount(), countC, 3);
		SetDlgItemText(hwnd, TXT_COUNT, countC);
		SetDlgItemText(hwnd, TXT_VET_NAME, vetName.c_str());
		Image(hwnd, DOC_PIC_2, picDoc, 100, 100);

		EnableMenuItem(GetSystemMenu(hwnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		int getCurWindow = GetDlgCtrlID(hwnd);
		listItems = GetDlgItem(hwnd, IDC_LIST1);
		if (origin != NULL) {
			while (aux != NULL) {
				string text;
				text += aux->name + " --- ";
				text += aux->date + " - " + aux->hour;
				//int pos = (int)SendMessage(listItems, LB_ADDSTRING, 0, (LPARAM)aux->id);
				SendMessage(listItems, LB_ADDSTRING, 0, (LPARAM)text.c_str());
				aux = aux->next;
			}
			aux = origin;
		}
	}
		break;
	case WM_DESTROY: // DestroyWindow();
		if (isDown) PostQuitMessage(0);
		break;
	case WM_COMMAND:
	{
		eventsMenu(hwnd, msg, wParam, lParam);
	}
		break;
	case WM_TIMER: {
		//time(&secondsOfAllTimezAndEverything);
	//	time_t now = time(0);
	//	actualTime = localtime_s(&actualTime, &now);
	//	char chReloj[80];
	//	strftime(chReloj, 80, "%d-%m-%Y %I:%M:%S", actualTime);
	//	SetWindowText(hLblReloj, chReloj);
	}break;
	case WM_CLOSE: // X
		break;
	}
	return FALSE;
}
HWND specieList, reasonList;
char imagenvet[1024] = "C:\\Users\\aldai\\source\\repos\\practica\\Project1\\add.bmp";

BOOL CALLBACK eventAdd(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG: {
		SetDlgItemText(hwnd, TXT_VET_NAME, vetName.c_str());
		Image(hwnd, DOC_PIC_2, picDoc, 100, 100); 
		Image(hwnd, PIC_ADD_PET, imagenvet, 100, 100);
		specieList = GetDlgItem(hwnd, IDC_COMBO1);
		reasonList = GetDlgItem(hwnd, IDC_COMBO2);
		SetDlgItemText(hwnd, TXT_VET_NAME, vetName.c_str());
		string species[9] = { "Perro", "Gato", "Conejo", "Pez", "Aves", "Roedores", "Reptiles", "Aracnidos", "Otros" };
		for (int i = 0; i < 9; i++) {
			SendMessage(specieList, CB_ADDSTRING, 0, (LPARAM)species[i].c_str());
		}
		SendMessage(specieList, CB_SETCURSEL, 4, NULL);
		string reasons[3] = {
			"Chequeo de rutina",
			"Esta enfermo",
			"Otro"
		};
		for (int i = 0; i < 3; i++) {
			SendMessage(reasonList, CB_ADDSTRING, 0, (LPARAM)reasons[i].c_str());
		}
		SendMessage(reasonList, CB_SETCURSEL, 2, NULL);
	}
		break;
	case WM_CLOSE:
		break;
	case WM_COMMAND: {
		eventsMenu(hwnd, msg, wParam, lParam);
		if (Lib.isClicked(wParam, BN_OK_ADD)) {
			char name[MAX_PATH];
			GetDlgItemText(hwnd, IDC_NAME, name, MAX_PATH);
			if (strlen(name) <= 0) {
				MessageBox(hwnd, "Ingresa el nombre del cliente", "Campo faltante", MB_ICONERROR);
				break;
			}
			char phone[MAX_PATH];
			GetDlgItemText(hwnd, IDC_PHONE, phone, MAX_PATH);
			if (strlen(phone) <= 0) {
				MessageBox(hwnd, "Ingresa el telefono del cliente", "Campo faltante", MB_ICONERROR);
				break;
			}
			else if (strlen(phone) > 10) {
				MessageBox(hwnd, "El telefono no puede ser mayor a 10 digitos", "Campo erroneo", MB_ICONERROR);
				break;
			}
			char petName[MAX_PATH];
			GetDlgItemText(hwnd, IDC_PET_NAME, petName, MAX_PATH);
			if (strlen(petName) <= 0) {
				MessageBox(hwnd, "Ingresa el nombre de la mascota", "Campo faltante", MB_ICONERROR);
				break;
			}
			int gender;
			if (IsDlgButtonChecked(hwnd, RADIX_IS_MALE) == BST_CHECKED) gender = 0;
			else if (IsDlgButtonChecked(hwnd, RADIX_IS_FEMALE) == BST_CHECKED) gender = 1;
			else {
				MessageBox(hwnd, "Debes de seleccionar un genero", "Seleccion faltante", MB_ICONEXCLAMATION);
				break;
			}
			int curSpecie = (int)SendMessage(specieList, LB_GETCURSEL, 0, 0);
			int curReason = (int)SendMessage(reasonList, LB_GETCURSEL, 0, 0);
			char date[MAX_PATH], hour[MAX_PATH];
			GetDlgItemText(hwnd, IDC_DATETIMEPICKER1, date, MAX_PATH);
			GetDlgItemText(hwnd, IDC_DATETIMEPICKER2, hour, MAX_PATH);
			if (origin == NULL) {
				origin = new Appoiment;
				aux = origin;
				aux->prev = NULL;
			}
			else {
				while (aux->next != NULL) aux = aux->next;
				aux->next = new Appoiment;
				aux->next->prev = aux;
				aux = aux->next;
			}
			aux->next = NULL;
			aux->name = name;
			aux->phone = phone;
			aux->petName = petName;
			aux->gender = gender;
			aux->specie = curSpecie;
			aux->reason = curReason;
			aux->date = date;
			aux->hour = hour;
			aux->status = 0; // Sigue activa
			aux->id = globalId;
			globalId++;
			aux = origin;
			isChange = true;
			DestroyWindow(hwnd);
		}
		else if (Lib.isClicked(wParam, IDC_DENY)) {
			DestroyWindow(hwnd);
		}
	}
		break;
	case WM_DESTROY: // DestroyWindow();
		DialogBox(_hInst, MAKEINTRESOURCE(IDD_MAIN), 0, principalFunction);
		break;
	}
	return FALSE;
}
BOOL CALLBACK eventEdit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
		SetDlgItemText(hwnd, TXT_VET_NAME, vetName.c_str());
		Image(hwnd, DOC_PIC_2, picDoc, 100, 100);
		break;
	case WM_CLOSE:
		break;
	case WM_COMMAND:
		eventsMenu(hwnd, msg, wParam, lParam);
		break;
	case WM_DESTROY: // DestroyWindow();
		if(isDown) PostQuitMessage(0);
		break;
	}
	return FALSE;
}
HWND selItem;
char item[_MAX_PATH];

BOOL CALLBACK eventDelete(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
		SetDlgItemText(hwnd, TXT_VET_NAME, vetName.c_str());
		Image(hwnd, DOC_PIC_2, picDoc, 100, 100);
		break;
	case WM_CLOSE:
		break;
	case WM_COMMAND:
		eventsMenu(hwnd, msg, wParam, lParam);
		if (Lib.isClicked(wParam, IDOK)) {
			GetDlgItemText(hwnd, IDC_PRE_DELETE, item, _MAX_PATH);
			selItem = GetDlgItem(hwnd, IDC_LIST_DELETE);
				bool isfound = false;
					SendMessage(selItem, LB_RESETCONTENT, 0, 0); // clear count
					while (aux != NULL) {
						if (aux->id == atoi(item) || strcmp(aux->name.c_str(), item) == 0) {
							string text;
							char id[10];
							_itoa_s(aux->id, id, 10);
							text += id;
							text += " --- " + aux->name + " --- " + aux->date + " - " + aux->hour;
							SendMessage(selItem, LB_ADDSTRING, 0, (LPARAM)text.c_str());

							isfound = true;
						}
						aux = aux->next;
				}
				if (!isfound) {
					SendMessage(selItem, LB_RESETCONTENT, 0, 0);
					SendMessage(selItem, LB_ADDSTRING, 0, (LPARAM)"No hubo resultados");
				}
				aux = origin;
		}
		break;
	case WM_DESTROY: // DestroyWindow();
		if (isDown) PostQuitMessage(0);
		break;
	}
	return FALSE;
}
BOOL CALLBACK eventLogin(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_COMMAND: {
		if (Lib.isClicked(wParam, BN_LOGIN)) {
			char name[MAX_PATH];
			GetDlgItemText(hwnd, IDC_USERNAME, name, MAX_PATH);
			char pass[MAX_PATH];
			GetDlgItemText(hwnd, IDC_PASSWORD, pass, MAX_PATH);
			string user, password;
			if (strlen(name) < 1 || strlen(pass) < 1) {
				MessageBox(hwnd, "Ingresa algun nombre de usuario", "Error", MB_ICONEXCLAMATION);
				break;
			}
			if (Login.check(name, pass)) {
				DestroyWindow(hLogin);
			}
			else {
				MessageBox(hwnd, "El usuario o la contraseña son incorrectos", "Verifque sus datos", MB_ICONEXCLAMATION);
			}
		}
		else if (Lib.isClicked(wParam, IDC_DENY)) {
			PostQuitMessage(0); // cerrar programa
		}
	}
		break;
	case WM_DESTROY: // DestroyWindow();
		DialogBox(_hInst, MAKEINTRESOURCE(IDD_MAIN), 0, principalFunction);
		break;
	}
	return FALSE;
}
BOOL CALLBACK eventInfo(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
		SetDlgItemText(hwnd, TXT_VET_NAME, vetName.c_str());
		Image(hwnd, DOC_PIC_2, picDoc, 100, 100);
		break;
	case WM_CLOSE:
		break;
	case WM_COMMAND:
		eventsMenu(hwnd, msg, wParam, lParam);
		break;
	case WM_DESTROY: // DestroyWindow();
		if (isDown) PostQuitMessage(0);
		break;
	}
	return FALSE;
}
BOOL CALLBACK eventList(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
		case WM_INITDIALOG:
			SetDlgItemText(hwnd, TXT_VET_NAME, vetName.c_str());
			Image(hwnd, DOC_PIC_2, picDoc, 100, 100);
			listItems = GetDlgItem(hwnd, IDC_LIST_FULLY);
			if (origin != NULL) {
				while (aux != NULL) {
					string text;
					text += aux->name + " --- ";
					text += aux->date + " - " + aux->hour;
					SendMessage(listItems, LB_ADDSTRING, aux->id, (LPARAM)text.c_str());
					aux = aux->next;
				}
				aux = origin;
			}
			break;
		case WM_COMMAND:
			eventsMenu(hwnd, msg, wParam, lParam);
			if (LOWORD(wParam) == IDC_LIST_FULLY && HIWORD(wParam) == LBN_SELCHANGE) {
				char testString[50];
				int selIndex = (int)SendMessage(listItems, LB_GETCURSEL, 0, 0);
				/*Y ahora que tenemos todos los datos obtenemos el texto de la seleccion*/
				SendMessage(listItems, LB_GETTEXT, (WPARAM)selIndex, (LPARAM)testString);
				int sel = (int)SendMessageW(listItems, LB_GETCURSEL, 0, 0);
				MessageBox(hwnd, "El texto", testString, MB_OK);
			}
			break;
		case WM_DESTROY: // DestroyWindow();
			if (isDown) PostQuitMessage(0);
			break;
	}
	return FALSE;
}
#pragma region  database
void loadDb() {
	ifstream database;
	database.open("database.bin", ios::in | ios::binary | ios::ate);
	if (!database.is_open()) {
		MessageBox(NULL, "No se pudo abrir el archivo", "Eror", MB_ICONERROR);
		return;
	}
	int size = database.tellg();
	if (size != 0) {
		for (int i = 0; i < (size / sizeof(Appoiment)); i++) {
			if (origin == NULL) {
				origin = new Appoiment;
				aux = origin;
				aux->prev = NULL;
			}
			else {
				while (aux->next != NULL) aux = aux->next;
				aux->next = new Appoiment;
				aux->next->prev = aux;
				aux = aux->next;
			}
			Appoiment* Temp = new Appoiment;
			database.seekg(i * sizeof(Appoiment));
			database.read(reinterpret_cast<char*>(Temp), sizeof(Appoiment));
			aux->name = Temp->name;
			aux->petName = Temp->petName;
			aux->date = Temp->date;
			aux->hour = Temp->hour;
			aux->gender = Temp->gender;
			//				aux->photo = Temp->photo;
			aux->reason = Temp->reason;
			aux->specie = Temp->specie;
			aux->status = Temp->status;
			aux->phone = Temp->phone;
			aux->id = Temp->id;
			delete reinterpret_cast<char*>(Temp);
			aux->next = NULL;
			globalId++;
			aux = origin;
		}
		database.close();
	}
}
void saveDb() {
	if (origin != NULL) {
		ofstream database;
		database.open("database.bin", ios::out | ios::trunc | ios::binary);
		if (!database.is_open()) MessageBox(NULL, "No se pudo guardar los cambios", "Error", MB_ICONERROR);
		else {
			Appoiment* temp = origin;
			while (temp != NULL) {
				database.write(reinterpret_cast<char*>(temp), sizeof(Appoiment));
				temp = temp->next;
			}
			database.close();
		}

	}
}
#pragma endregion