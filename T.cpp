#include <iostream.h>

class Base {
public:
	void wfn() {
		cout << "Base::wfn()" << endl;
	}

	virtual void xfn(int i) {
		cout << "Base::xfn(int i)" << endl;
	}

	virtual void yfn(float f) {
		cout << "Base::yfn(float f)" << endl;
	}

	virtual void zfn() {
		cout << "Base::zfn()" << endl;
	}

	virtual void zzfn() {
		cout << "Base::zzfn()" <<endl;
	}
};

class Derived: public Base {
public:
	void xfn(int i) {
		cout << "Derived::xfn(int i)" << endl;
	}

	virtual void yfn(int c) {
		cout << "Derived::yfn(int c)" << endl;
	}

	void zfn(int i) {
		cout << "Derived::zfn(int i)" << endl;
	}

	void zzfn() {
		cout << "Derived::zzfn()" << endl;
	}

	void zzfn(int i) {
		cout << "Derived::zzfn(int i)" << endl;
	}
};

class DerivedFromDerived: public Derived {
public:
	void zfn() {
		cout << "DerivedFromDerived::zfn()" << endl;
	}
};

void main() {
	Derived d;

	d.xfn(5);
	d.Base::xfn(5);

	d.wfn();
	d.Base::wfn();
	d.Derived::wfn();

	Base & b = d;

	b.xfn(5);
	b.Base::xfn(5);

	Base * pB = &d;
	Derived * pD = &d;

	pB -> xfn(5);
	pD -> xfn(5);

	pB -> yfn(3.14f);
	pD -> yfn(5);

	((Derived *) pB) -> yfn(5);
	pD -> Base::yfn(3.14f);

	pB -> zfn();
	pD -> zfn(5);

	DerivedFromDerived dd;

	dd.zzfn();
	dd.zzfn(5);

	Base * p = &dd;

	p -> zfn();
	p -> Base::zfn();

	p -> zzfn();
	p -> Base::zzfn();
}

/*
#include <stdio.h>

int main() {
	int i;

	printf("%d\n", + + (i = 0));
	printf("%d\n", ++ (i = 0));
	(++ i) = 0;
	(++ i) ++;
//	++ (i ++);

	(+ i) = 0;
	(+ i) ++;

	- (i ++);
//	(- i) = 0;
//	(- i) ++;

	printf("%d\n", -+ ++ + (i = 0));
	printf("%d\n", - ++ ++ (i = 0));
	printf("%d\n", -++++ (i = 0));

	switch(i) {
	case 0:
		int k;
//		int k = 0;
		break;
	default:
		;
//		int k;
	}

	return 0;
}
*/

/*
#include <stdio.h>
#include <string.h>

int n;
char state[] = "0000000000";
char flag[]  = "0000000000";
char min[]   = "9999999999";

bool isRepeated(int i) {
	return flag[i] == (state[i] - '0' + 1) % 10 + '0';
}

void roll(int i) {
	state[i] = (state[i] - '0' + 1) % 10 + '0';
}

int main() {
	scanf("%d", &n);
	state[n] = '\0';
	min[n] = '\0';

	printf("%s\n", min);
	printf("%s\n", state);
	for(;;) {
		int i;
		for(i=0; i<n && isRepeated(i); ++i);
		if(i==n) break;

		roll(i);
		printf("%s\n", state);
		strncpy(flag, state, i);
	}

	return 0;
}
*/

/*
int main() {
	int (a [10]);
	int (* b);
	int (* (c [10]));
	int ((* d) [10]);
	return 0;
}
*/

/*
#include <stdio.h>

int main() {
	char s[1024] = "";
	char c[8] = "";

//	scanf("%[^[][%2c:%2c.%2c]", s, c, c+2, c+4);

	char * p = "[:]" + 1;
	char * q = "[:]" + 2;
	printf("%s\n", p);
	printf("%d\n", q - p);

	return 0;
}
*/

/*
#include <stdio.h>
#include "Thread.h"

class ThreadTester: public Thread {
public:
	ThreadTester(): Thread(0, (PWORK) &ThreadTester::work) {}

protected:
	void work();
};

void ThreadTester::work() {
	int i;
	for(i=0; i<10000; ++i) {
		if(eventStopping.isSignaled()) return;
		printf("%04d: Hello World!\n", i);
	}
}

ThreadTester t;

int main() {
	printf("start()\n");
	t.start();
	t.join();

	t.stop();
	printf("stop()\n");

	printf("start()\n");
	t.start();

	return 0;
}
*/

/*
#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;

// centisecond
// GetTickCount() millisecond
// QueryPerformanceCounter() microsecond

int main() {
    DWORD  dwGTCBegin, dwGTCEnd;

    __int64  frequency, countBegin, countEnd;

    QueryPerformanceFrequency((PLARGE_INTEGER)&frequency);
	printf("High Resolution Frequency %I64d\n", frequency);

	printf("\nSleep()\n\n");

    dwGTCBegin = GetTickCount();
    Sleep(800);
    dwGTCEnd = GetTickCount();
    printf("%d milliseconds\n", dwGTCEnd - dwGTCBegin);

	printf("\n");

    QueryPerformanceCounter((PLARGE_INTEGER)&countBegin);
    Sleep(800);
    QueryPerformanceCounter((PLARGE_INTEGER)&countEnd);
    printf("Begin Counts %I64d\n", countBegin);
	printf("End Counts %I64d\n", countEnd);
	printf("Difference %I64d\n", countEnd - countBegin);
    printf("%I64d microseconds\n", (countEnd - countBegin) * 1000000 / frequency);

	printf("\nWaitForSingleObject()\n\n");

	HANDLE event = CreateEvent(NULL, TRUE, FALSE, NULL);

    dwGTCBegin = GetTickCount();
	WaitForSingleObject(event, 800);
    dwGTCEnd = GetTickCount();
    printf("%d milliseconds\n", dwGTCEnd - dwGTCBegin);

	printf("\n");

    QueryPerformanceCounter((PLARGE_INTEGER)&countBegin);
	WaitForSingleObject(event, 800);
    QueryPerformanceCounter((PLARGE_INTEGER)&countEnd);
    printf("%I64d microseconds\n", (countEnd - countBegin) * 1000000 / frequency);

	printf("\nprintf()\n\n");

    QueryPerformanceCounter((PLARGE_INTEGER)&countBegin);
	printf("printf() is runing. \"%d\" has been gotten.\n", 0);
    QueryPerformanceCounter((PLARGE_INTEGER)&countEnd);
    printf("%I64d microseconds\n", (countEnd - countBegin) * 1000000 / frequency);

    return 0;
}
*/

/*
#include <windows.h>
#include <stdio.h>

void NewLine(void);
void ScrollScreenBuffer(HANDLE, INT);

HANDLE hStdout, hStdin;
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

void main(void)
{
    LPSTR lpszPrompt1 = "Type a line and press Enter, or q to quit: ";
    LPSTR lpszPrompt2 = "Type any key, or q to quit: ";
    CHAR chBuffer[256];
    DWORD cRead, cWritten, fdwMode, fdwOldMode;
    WORD wOldColorAttrs;

    // Get handles to STDIN and STDOUT. 

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE || 
        hStdout == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, TEXT("GetStdHandle"), TEXT("Console Error"), 
            MB_OK);
        return;
    }

    // Save the current text colors. 

    if (! GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
    {
        MessageBox(NULL, TEXT("GetConsoleScreenBufferInfo"), 
            TEXT("Console Error"), MB_OK);
        return;
    }

    wOldColorAttrs = csbiInfo.wAttributes;

    // Set the text attributes to draw red text on black background. 

    if (! SetConsoleTextAttribute(hStdout, FOREGROUND_RED | 
            FOREGROUND_INTENSITY))
    {
        MessageBox(NULL, TEXT("SetConsoleTextAttribute"), 
            TEXT("Console Error"), MB_OK);
        return;
    }

    // Write to STDOUT and read from STDIN by using the default 
    // modes. Input is echoed automatically, and ReadFile 
    // does not return until a carriage return is typed. 
    // 
    // The default input modes are line, processed, and echo. 
    // The default output modes are processed and wrap at EOL. 

    for (;;)
    {

//        if (! WriteFile( 
//            hStdout,               // output handle 
//            lpszPrompt1,           // prompt string 
//            lstrlenA(lpszPrompt1), // string length 
//            &cWritten,             // bytes written 
//            NULL) )                // not overlapped 
//        {
//            MessageBox(NULL, TEXT("WriteFile"), TEXT("Console Error"), 
//                MB_OK);
//            return;
//        }

//        if (! ReadFile( 
//            hStdin,    // input handle 
//            chBuffer,  // buffer to read into 
//            255,       // size of buffer 
//            &cRead,    // actual bytes read 
//            NULL) )    // not overlapped 
//        break;

        printf("%s", lpszPrompt1);
        scanf("%s", chBuffer);
        if (chBuffer[0] == 'q') break;
    }

    // Turn off the line input and echo input modes 

    if (! GetConsoleMode(hStdin, &fdwOldMode))
    {
       MessageBox(NULL, TEXT("GetConsoleMode"), TEXT("Console Error"),
           MB_OK);
       return;
    }

    fdwMode = fdwOldMode & 
        ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT); 
    if (! SetConsoleMode(hStdin, fdwMode)) 
    {
       MessageBox(NULL, TEXT("SetConsoleMode"), TEXT("Console Error"),
           MB_OK);
       return;
    }

    // ReadFile returns when any input is available.  
    // WriteFile is used to echo input. 

    NewLine();

    for (;;)
    {
        if (! WriteFile( 
            hStdout,               // output handle 
            lpszPrompt2,           // prompt string 
            lstrlenA(lpszPrompt2), // string length 
            &cWritten,             // bytes written 
            NULL) )                // not overlapped 
        {
            MessageBox(NULL, TEXT("WriteFile"), TEXT("Console Error"), 
                MB_OK);
            return;
        }

        if (! ReadFile(hStdin, chBuffer, 1, &cRead, NULL)) 
            break;
        if (chBuffer[0] == '\r')
            NewLine();
        else if (! WriteFile(hStdout, chBuffer, cRead, 
            &cWritten, NULL)) break;
        else
            NewLine();
        if (chBuffer[0] == 'q') break;
    }

    // Restore the original console mode. 

    SetConsoleMode(hStdin, fdwOldMode);

    // Restore the original text colors. 

    SetConsoleTextAttribute(hStdout, wOldColorAttrs);
}

// The NewLine function handles carriage returns when the processed 
// input mode is disabled. It gets the current cursor position 
// and resets it to the first cell of the next row. 

void NewLine(void)
{
    if (! GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
    {
        MessageBox(NULL, TEXT("GetConsoleScreenBufferInfo"), 
            TEXT("Console Error"), MB_OK);
        return;
    }

    csbiInfo.dwCursorPosition.X = 0;

    // If it is the last line in the screen buffer, scroll 
    // the buffer up. 

    if ((csbiInfo.dwSize.Y-1) == csbiInfo.dwCursorPosition.Y) 
    {
        ScrollScreenBuffer(hStdout, 1);
    }

    // Otherwise, advance the cursor to the next line. 

    else csbiInfo.dwCursorPosition.Y += 1;

    if (! SetConsoleCursorPosition(hStdout, 
        csbiInfo.dwCursorPosition))
    {
        MessageBox(NULL, TEXT("SetConsoleCursorPosition"), 
            TEXT("Console Error"), MB_OK);
        return;
    }
}

void ScrollScreenBuffer(HANDLE h, INT x)
{
    SMALL_RECT srctScrollRect, srctClipRect;
    CHAR_INFO chiFill;
    COORD coordDest;

    srctScrollRect.Left = 0;
    srctScrollRect.Top = 1;
    srctScrollRect.Right = csbiInfo.dwSize.X - x;
    srctScrollRect.Bottom = csbiInfo.dwSize.Y - x;

    // The destination for the scroll rectangle is one row up. 

    coordDest.X = 0;
    coordDest.Y = 0;

    // The clipping rectangle is the same as the scrolling rectangle. 
    // The destination row is left unchanged. 

    srctClipRect = srctScrollRect;

    // Set the fill character and attributes. 

    chiFill.Attributes = FOREGROUND_RED|FOREGROUND_INTENSITY;
    chiFill.Char.AsciiChar = (char)' ';

    // Scroll up one line. 

    ScrollConsoleScreenBuffer( 
        h,               // screen buffer handle 
        &srctScrollRect, // scrolling rectangle 
        &srctClipRect,   // clipping rectangle 
        coordDest,       // top left destination cell 
        &chiFill);       // fill character and color 
}
*/

/*
#include <stdio.h>
#include <windows.h>

int main() {
	if(WaitForSingleObject(NULL, INFINITE)==WAIT_FAILED)
		printf("%d\n", GetLastError());

	if(CloseHandle(NULL)==FALSE)
		printf("%d\n", GetLastError());

	HANDLE event = CreateEvent(NULL, TRUE, FALSE, NULL);

	if(WaitForSingleObject(event, -1024)==WAIT_FAILED)
		printf("%d\n", GetLastError());

	CloseHandle(event);

	if(WaitForSingleObject(event, INFINITE)==WAIT_FAILED)
		printf("%d\n", GetLastError());

	if(CloseHandle(event)==FALSE)
		printf("%d\n", GetLastError());

	return 0;
}
*/

/*
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

DWORD ThreadFunc(LPVOID);
// DWORD WINAPI ThreadFunc(LPVOID);

int main() {
	HANDLE hThrd;
	DWORD threadId;

	int i = 0;
	hThrd = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(ThreadFunc), (LPVOID)i, 0, &threadId);
	WaitForSingleObject(hThrd, INFINITE);
	CloseHandle(hThrd);

//	for(;;);

	return EXIT_SUCCESS;
}

DWORD ThreadFunc(LPVOID n) {
	int i;
	for(i=0; i<10; ++i) printf("%d%d%d%d%d%d%d%d\n", n, n, n, n, n, n, n, n);
	return 0;
}
*/

/*
#include <windows.h>
#include <limits.h>
#include <iostream>
using namespace std;

int main() {
	UINT a = UINT_MAX, b = UINT_MAX;
	cout << a << endl << b << endl << a+b << endl;

	UINT size = 10;
	const UINT SIZE = size;
//	int data[SIZE];

	char * const p = "";
//	p = "0";

	return 0;
}
*/

/*
#include <iostream>
using namespace std;

class Animal {
public:
	Animal(int height, int weight) {
		h = height; w = weight;
		cout << "Animal construct" << endl;
	}
	~Animal() {
		cout << "Animal destruct" << endl;
	}
	int h, w;
};

class Fish: public Animal {
public:
	Fish(): Animal(400, 300) {
		this->breatheAir = false;
		this->breatheWater = true;
		cout << "Fish construct" << endl;
	}
	Fish(bool breatheAir): Animal(0, 0), breatheAir(breatheAir) {
//		this->breatheAir = breatheAir;
		cout << "Fish reconstruct" << endl;
	}
	~Fish() {
		cout << "Fish destruct" << endl;
		Animal::~Animal();
	}
	bool breatheAir, breatheWater;
};

int main() {
	Animal::Animal(0, 0);
	Fish f1;
	cout << f1.breatheAir << endl;
	Fish f2(true);
	cout << f2.breatheAir << endl;
	f2.~Fish();
	Animal a(400, 300);
	a.~Animal();
//	a.Animal(0, 0); // Compile Error
//	Animal * pa = & Animal(400, 300);
//	delete pa; // Runtime Error
	return 0;
}
*/
