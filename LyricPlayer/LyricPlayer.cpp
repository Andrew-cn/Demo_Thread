// Lyric Player v1.0
// Date: 2015-5-12

#include <windows.h>

// #define _WIN32_WINNT 0x0600

#define MAX_TAGS            128
#define MAX_LYRICS         8192
#define FAST_RUNNING_RATE    20
#define REDUNDANCY           50

CONST CONSOLE_CURSOR_INFO CURSOR_INVISIBLE
	= {25, FALSE};
CONST WORD FOREGROUND_NORMAL
	= FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
CONST WORD FOREGROUND_HIGHLIGHT
	= FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;

struct {
	INT milliseconds;
	PTCH begin, end;
	DWORD length;
	COORD position;
} tags[MAX_TAGS];
DWORD count = 0;

TCHAR lyrics[MAX_LYRICS];
DWORD length;

bool schema_1 (TCHAR c) { return c == TEXT('['); }
bool schema_2 (TCHAR c) { return c >= TEXT('0') && c <= TEXT('9'); }
bool schema_3 (TCHAR c) { return c == TEXT(':'); }
bool schema_4 (TCHAR c) { return c >= TEXT('0') && c <= TEXT('5'); }
bool schema_5 (TCHAR c) { return c == TEXT('.'); }
bool schema_6 (TCHAR c) { return c == TEXT(']'); }

bool (* schema[10]) (TCHAR c) = { schema_1, schema_2, schema_2,
                                  schema_3, schema_4, schema_2,
                                  schema_5, schema_2, schema_2,
                                  schema_6 };

class Timer {
public:
	Timer();
	VOID setTime(INT milliseconds);
	VOID setRate(INT rate);
	INT getTime();
	INT getRate() const;

private:
	LONGLONG frequency, countOrigin, countBegin, countEnd;
	INT runningRate;
};

Timer::Timer() {
	QueryPerformanceFrequency( (PLARGE_INTEGER) &frequency );
	runningRate = 1;
	setTime( 0 );
}

VOID Timer::setTime(INT milliseconds) {
	countOrigin = frequency * milliseconds / 1000;
	QueryPerformanceCounter( (PLARGE_INTEGER) &countBegin );
}

VOID Timer::setRate(INT rate) {
	QueryPerformanceCounter( (PLARGE_INTEGER) &countEnd );
	countOrigin = countOrigin + (countEnd - countBegin) * runningRate;
	runningRate = rate;
	QueryPerformanceCounter( (PLARGE_INTEGER) &countBegin );
}

INT Timer::getTime() {
	QueryPerformanceCounter( (PLARGE_INTEGER) &countEnd );
	INT milliseconds = INT( (countOrigin + (countEnd - countBegin) * runningRate) * 1000 / frequency );
	if (milliseconds < 0) {
		setTime( 0 );
		return 0;
	} else
		return milliseconds;
}

INT Timer::getRate() const {
	return runningRate;
}

int main() {
	HANDLE hFile = CreateFile(TEXT("lyric.txt"), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(hFile, lyrics, sizeof(lyrics), &length, NULL);
	length = lstrlen(lyrics);
	CloseHandle(hFile);

	DWORD i, j;

	tags[count].milliseconds = 0;
	tags[count].begin = tags[count].end = lyrics;
	tags[count].length = 0;
	tags[count].position.X = tags[count].position.Y = 0;
	++count;

	for(i=0, j=0; i<length; ++i)
		if( schema[j](lyrics[i]) ) {
			if(j==9) {
				tags[count].milliseconds = (lyrics[i-8] - '0') * 10 + (lyrics[i-7] - '0');
				tags[count].milliseconds = tags[count].milliseconds * 6 + (lyrics[i-5] - '0');
				tags[count].milliseconds = tags[count].milliseconds * 10 + (lyrics[i-4] - '0');
				tags[count].milliseconds = tags[count].milliseconds * 10 + (lyrics[i-2] - '0');
				tags[count].milliseconds = tags[count].milliseconds * 10 + (lyrics[i-1] - '0');
				tags[count].milliseconds *= 10;
				tags[count].begin = lyrics + i + 1;
				++count;
				j=0;
			} else ++j;
		} else j=0;

	for(i=1; i<count-1; ++i) {
		tags[i].end = tags[i+1].begin - 10;
		tags[i].length = tags[i].end - tags[i].begin;
	}
	tags[i].end = lyrics + length;
	tags[i].length = tags[i].end - tags[i].begin;

	tags[count].milliseconds = tags[count-1].milliseconds + 1000;
	tags[count].begin = tags[count].end = lyrics + length;
	tags[count].length = 0;
	tags[count].position.X = tags[count].position.Y = 0;
	++count;

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

//	GetCurrentConsoleFontEx(hStdout, FALSE, lpConsoleCurrentFontEx);
//	SetCurrentConsoleFontEx(hStdout, FALSE, lpConsoleCurrentFontEx);

	SetConsoleCursorInfo(hStdout, &CURSOR_INVISIBLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_NORMAL);

	for(i=0; i<count; ++i) {
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(hStdout, &info);
		tags[i].position = info.dwCursorPosition;

		WriteConsole(hStdout, tags[i].begin, tags[i].end-tags[i].begin, NULL, NULL);
	}

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleMode(hStdin, 0);

	INPUT_RECORD inputRecord;
	DWORD numbleRead, numbleWritten;

	DWORD currentTagIndex = 0;

	Timer timer;
	for(;;) {
		INT millisecondsToWait = INFINITE;
		if (timer.getRate() > 0) {
			millisecondsToWait = ( tags[currentTagIndex+1].milliseconds - timer.getTime() )
				/ timer.getRate() - REDUNDANCY;
			if (millisecondsToWait < 0) millisecondsToWait = 0;
		}
		if (timer.getRate() < 0) {
			millisecondsToWait = ( tags[currentTagIndex].milliseconds - timer.getTime() )
				/ timer.getRate() - REDUNDANCY;
			if (millisecondsToWait < 0) millisecondsToWait = 0;
		}
		if (WaitForSingleObject(hStdin, millisecondsToWait) == WAIT_OBJECT_0) {
			ReadConsoleInput(hStdin, &inputRecord, 1, &numbleRead);
			if (inputRecord.EventType == KEY_EVENT) {
				if (inputRecord.Event.KeyEvent.bKeyDown == TRUE) {
					if (inputRecord.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
						SetConsoleCursorPosition(hStdout, tags[count-2].position);
						break;
					}
					if (timer.getRate() == 1 && inputRecord.Event.KeyEvent.wVirtualKeyCode == 'A')
						timer.setRate( -FAST_RUNNING_RATE );
					if (timer.getRate() == 1 && inputRecord.Event.KeyEvent.wVirtualKeyCode == 'D')
						timer.setRate( +FAST_RUNNING_RATE );
				} else {
					if (timer.getRate() < 0 && inputRecord.Event.KeyEvent.wVirtualKeyCode == 'A')
						timer.setRate( 1 );
					if (timer.getRate() > 0 && inputRecord.Event.KeyEvent.wVirtualKeyCode == 'D')
						timer.setRate( 1 );
				}
			} else if (inputRecord.EventType == FOCUS_EVENT) {
				if (timer.getRate() != 1 && inputRecord.Event.FocusEvent.bSetFocus == FALSE)
					timer.setRate( 1 );
			}
		}
		while (currentTagIndex+1 < count && tags[currentTagIndex+1].milliseconds <= timer.getTime()) {
			FillConsoleOutputAttribute(hStdout, FOREGROUND_NORMAL, tags[currentTagIndex].length,
				tags[currentTagIndex].position, &numbleWritten);
			++currentTagIndex;
			FillConsoleOutputAttribute(hStdout, FOREGROUND_HIGHLIGHT, tags[currentTagIndex].length,
				tags[currentTagIndex].position, &numbleWritten);
			SetConsoleCursorPosition(hStdout, tags[currentTagIndex].position);
		}
		while (currentTagIndex > 0 && tags[currentTagIndex].milliseconds > timer.getTime()) {
			FillConsoleOutputAttribute(hStdout, FOREGROUND_NORMAL, tags[currentTagIndex].length,
				tags[currentTagIndex].position, &numbleWritten);
			--currentTagIndex;
			FillConsoleOutputAttribute(hStdout, FOREGROUND_HIGHLIGHT, tags[currentTagIndex].length,
				tags[currentTagIndex].position, &numbleWritten);
			SetConsoleCursorPosition(hStdout, tags[currentTagIndex].position);
		}
		if (currentTagIndex == count - 1) {
			currentTagIndex = 0;
			timer.setTime( 0 );
		}
	}

	return 0;
}
