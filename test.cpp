#include "dodatek.h"
#include <windows.h>
#include <ctime>
#include <cstdio>    //do printf

#define ID_PRZYCISK1 501

HWND g_hPrzyciskS;
HWND hRadio1;
HWND hOknoN;       //okienko mili sekund
HWND hOknoS;       //okienko sekund
HWND hOknoM;       //okienko minut
HWND hOknoG;       //okienko minut
HWND hOknoTN;      //okienko mS
HWND hOknoTS;      //okienko S
HWND hOknoTM;      //okienko M
HWND hOknoTG;      //okienko G

HBRUSH hBrush,hBrush2;        //do koloru tla

HICON hbmIkona;

HBRUSH g_hBrush;

char text_ms[50];
char text_s[50];   //char do wyswietlenia
char text_m[50];
char text_g[50];
int czas_start_ms;   //do zapisania czasu startu programu
int czas_ms;         //czas aktualny ms
int czas_s = 0;       //czas aktualny sekundy
int czas_m = 0;      //minuty
int czas_g = 0;      //godziny
int licznik_s = 0;     //licznik sekund
int licznik_m = 0;     //licznik sekund
int licznik_g = 0;     //licznik sekund
int ret;           //o programie

LPSTR NazwaKlasy = "Klasa Okienka";
MSG Komunikat;

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK DlgProc (HWND, UINT, WPARAM, LPARAM);

HWND hwnd,hwnd2,hProgressBar,hButton;
HINSTANCE hInstance;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    WNDCLASSEX wc;       // WYPEŁNIANIE STRUKTURY

    hBrush = CreateSolidBrush(RGB (211, 211, 200));    //tworzy pedzel jednolitego koloru
    hBrush2 = CreateSolidBrush (RGB (211, 211, 200));

    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );     //laduje kursor
    wc.hbrBackground =( HBRUSH )( hBrush + 0 );       //kolor tla
    wc.lpszMenuName = NULL;
    wc.lpszClassName = NazwaKlasy;
    wc.hIconSm = LoadIcon( hInstance, MAKEINTRESOURCE(IKONA) );        //zmieniam ikone okna

    // REJESTROWANIE KLASY OKNA
    if( !RegisterClassEx( & wc ) )
    {
        MessageBox( NULL, "Bład tworzenia okna!", "Spróbuj później...",
        MB_ICONEXCLAMATION | MB_OK );
        return 1;
    }

    // TWORZENIE OKNA
    HWND hwnd;

    HMENU hMenu = LoadMenu( hInstance, MAKEINTRESOURCE( 200 ) );

    hwnd = CreateWindowEx( WS_EX_WINDOWEDGE, NazwaKlasy, "Stoper", WS_SYSMENU,
    CW_USEDEFAULT, CW_USEDEFAULT, 390, 200, NULL, hMenu, hInstance, NULL );

    if( hwnd == NULL )
    {
        MessageBox( NULL, "Bład otwarcia okna!", "Spróbuj później...", MB_ICONEXCLAMATION );
        return 1;
    }

    ShowWindow( hwnd, nCmdShow ); // Pokaż okienko...
    UpdateWindow( hwnd );


    LOGFONT lf = {};                         //ten fragment odpowiada za powiększenie czcionki
    lf.lfHeight = -50;
    strcpy(lf.lfFaceName, "Tahoma");
    HFONT hFont1 = CreateFontIndirect(&lf);

    lf.lfHeight = -40;
    strcpy(lf.lfFaceName, "Tahoma");
    HFONT hFont2 = CreateFontIndirect(&lf);

    lf.lfHeight = -30;
    strcpy(lf.lfFaceName, "Tahoma");
    HFONT hFont3 = CreateFontIndirect(&lf);

    lf.lfHeight = -20;
    strcpy(lf.lfFaceName, "Tahoma");
    HFONT hFont4 = CreateFontIndirect(&lf);

    hOknoTN = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |                 //okna do wyswietlenia mS, S, M, G
                             SS_LEFT, 170, 10, 40, 20, hwnd, NULL, hInstance, NULL);
    SetWindowTextA(hOknoTN,"mS");
    hOknoTS = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
                             SS_LEFT, 125, 10, 40, 20, hwnd, NULL, hInstance, NULL);
    SetWindowTextA(hOknoTS,"S");
    hOknoTM = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
                             SS_LEFT, 75, 10, 40, 20, hwnd, NULL, hInstance, NULL);
    SetWindowTextA(hOknoTM,"M");
    hOknoTG = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
                             SS_LEFT, 30, 10, 40, 20, hwnd, NULL, hInstance, NULL);
    SetWindowTextA(hOknoTG,"G");

    hOknoN = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |                 //okna do wyswietlenia sekund
                             SS_LEFT, 170, 30, 50, 30, hwnd, NULL, hInstance, NULL);
    SetWindowTextA(hOknoS,"0");

    hOknoS = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |                 //okna do wyswietlenia sekund
                             SS_LEFT, 120, 27, 50, 30, hwnd, NULL, hInstance, NULL);
    SetWindowTextA(hOknoS,"0");

    hOknoM = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |                 //okna do wyswietlenia minut
                            SS_LEFT, 70, 25, 50, 40, hwnd, NULL, hInstance, NULL);
    SetWindowTextA(hOknoM,"0");

    hOknoG = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |                 //okna do wyswietlenia godzin
                            SS_LEFT, 20, 23, 50, 60, hwnd, NULL, hInstance, NULL);
    SetWindowTextA(hOknoG,"0");

    SendMessage(hOknoN, WM_SETFONT, (WPARAM)hFont4, false);        //wysyłam większą czcionkę do hOknoS
    SendMessage(hOknoS, WM_SETFONT, (WPARAM)hFont3, false);
    SendMessage(hOknoM, WM_SETFONT, (WPARAM)hFont2, false);
    SendMessage(hOknoG, WM_SETFONT, (WPARAM)hFont1, false);

    g_hPrzyciskS = CreateWindowEx( 0, "BUTTON", "START", WS_CHILD | WS_VISIBLE ,
                                     250, 20, 120, 120, hwnd, ( HMENU ) ID_PRZYCISK1, hInstance, NULL );

    // Pętla komunikatów
    while( GetMessage( & Komunikat, NULL, 0, 0 ) )
    {
        TranslateMessage( & Komunikat );
        DispatchMessage( & Komunikat );
    }
    return Komunikat.wParam;


}

// OBSŁUGA ZDARZEŃ
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
    case WM_CLOSE:
        DestroyWindow( hwnd );
        break;

    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;

   case WM_CREATE:
        SetTimer( hwnd, 1, 1, NULL );
        break;

    case WM_COMMAND:        //komunikat do konkretnego przycisku
        {
          if ( (HWND) lParam == g_hPrzyciskS )           //sprawdzam który przycisk został nacisięty
           {
//            clock()=0;
            czas_start_ms = (clock()/10);  //ustawiam nowy czas startowy
            czas_ms = 0;
            licznik_s = 0;
            licznik_m = 0;
            licznik_g = 0;
           }

        // if( LOWORD( wParam ) == 100 )   //sprawdzam ktory przycisk z menu został nacisnięty
         //   MessageBox( hwnd, "Dziekujemy za skorzystanie z naszego menu ;-P",
         //   "Mesydz", MB_ICONINFORMATION );
        if( LOWORD( wParam ) == 101 )
            PostQuitMessage( 0 );
        if ( LOWORD( wParam ) == 100 )   //sprawdzam ktory przycisk z menu został nacisnięty
            {
            ret = DialogBox (GetModuleHandle(NULL), MAKEINTRESOURCE(MOJDIALOG), hwnd, DlgProc);
            }

        }

    case WM_TIMER:
        switch( wParam )
        {
            case 1:

                //if ( (clock()/10) % 100 == 0)                 //co 100 mili seknud rozpoczęcie od 0
                if ( (clock()/10) - czas_start_ms > 99)                 //co 100 mili seknud rozpoczęcie od 0
                {
                    czas_start_ms = (clock()/10);
                    licznik_s++;                              //100ms czyli dodaje 1s
                }
                czas_ms = (clock()/10) - czas_start_ms;    //wyswietlony czas to czas aktualny minus czas startowy
                sprintf(text_ms,"%i", czas_ms);
                SetWindowText(hOknoN, text_ms);

                sprintf(text_s,"%i", licznik_s);
                SetWindowText(hOknoS, text_s);
                if (licznik_s > 59)                   //jeżeli minie 60 seknud liczę seknudy od początku i dodaje 1 minutę
                {
                    licznik_s = 0;
                    licznik_m++;
                }

                sprintf(text_m,"%i", licznik_m);
                SetWindowText(hOknoM, text_m);
                if (licznik_m > 59)
                {
                    licznik_m = 0;
                    licznik_g++;
                }

                sprintf(text_g,"%i", licznik_g);
                SetWindowText(hOknoG, text_g);

                break;
        default:
                break;
        }
        break;

    default:
    return DefWindowProc( hwnd, msg, wParam, lParam );

    }

    return 0;
}

BOOL CALLBACK DlgProc (HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)           // Procedura dialogowa
{
 switch (Msg)
 {
  case WM_COMMAND:
  {
   switch (LOWORD(wParam))              // reakcja na przyciski
   {
    case IDOK: EndDialog (hwnd, IDOK); break;
    case IDCANCEL: EndDialog (hwnd, IDCANCEL); break;
   }
  }
  break;

  default: return FALSE;
 }

 return TRUE;
}
