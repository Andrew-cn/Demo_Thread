#pragma once

#include <windows.h>

class Waitable {
public:
	Waitable(HANDLE handle);
	virtual ~Waitable();

	bool beWaited();
	bool isSignaled();

	HANDLE handle;
};

class Semaphore: public Waitable {
public:
	Semaphore(LONG initialCount, LONG maximumCount);

	bool dissipate();
	bool accumulate();
};

class Event: public Waitable {
public:
	Event(BOOL manualReset, BOOL initialState);

	bool happen();
	bool over();
};

class Thread: private Waitable {
protected:
	typedef void (Thread::*PWORK)();

	PWORK pWork;
	Event eventStopping;

	Thread(DWORD stackSize, PWORK pWork);
	~Thread();

public:
	void start();
	void stop();
	void join();

private:
	static DWORD WINAPI ThreadFunc(LPVOID param);

	Event eventStarting, eventDone, eventExiting;
	Semaphore accessible;
};
