#include "Thread.h"

//////////////////////////////////////////////////////////////////////////////////////////////

Waitable::Waitable(HANDLE handle): handle(handle) {}

Waitable::~Waitable() {
	CloseHandle(handle);
	handle = NULL;
}

bool Waitable::beWaited() {
	return WaitForSingleObject(handle, INFINITE) != WAIT_FAILED;
}

// Returns whether or not the waitable object is signaled.
bool Waitable::isSignaled() {
	return WaitForSingleObject(handle, 0) == WAIT_OBJECT_0;
}

//////////////////////////////////////////////////////////////////////////////////////////////

Semaphore::Semaphore(LONG initialCount, LONG maximumCount)
: Waitable(CreateSemaphore(NULL, initialCount, maximumCount, NULL))
{}

bool Semaphore::dissipate() {
	return beWaited();
}

bool Semaphore::accumulate() {
	return ReleaseSemaphore(handle, 1, NULL) == TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////

Event::Event(BOOL manualReset, BOOL initialState)
: Waitable(CreateEvent(NULL, manualReset, initialState, NULL))
{}

bool Event::happen() {
	return SetEvent(handle) == TRUE;
}

bool Event::over() {
	return ResetEvent(handle) == TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////

Thread::Thread(DWORD stackSize, PWORK pWork)
: Waitable(NULL), pWork(pWork), eventStarting(TRUE, FALSE), eventStopping(TRUE, FALSE),
  eventDone(TRUE, FALSE), eventExiting(TRUE, FALSE), accessible(1, 1)
{
	handle = CreateThread(NULL, stackSize, ThreadFunc, (LPVOID) this, 0, NULL);
}

Thread::~Thread() {
	eventExiting.happen();
	beWaited();
}

DWORD WINAPI Thread::ThreadFunc(LPVOID param) {
	Thread * p = (Thread *) param;

	CONST HANDLE handlesToBeWaited[2] = {p->eventExiting.handle, p->eventStarting.handle};

	for(;;) {
		WaitForMultipleObjects(2, handlesToBeWaited, FALSE, INFINITE);

		p->eventDone.over(); // !

		if(!p->eventStarting.isSignaled()) break;

		p->eventStopping.over();
		p->accessible.accumulate();

		(p->*p->pWork)();

		p->eventDone.happen(); // !

		p->eventStarting.over();
		p->accessible.accumulate();
	}

	return 0;
}

void Thread::start() {
	accessible.dissipate();
	if(!isSignaled() && !eventStarting.isSignaled())
		eventStarting.happen();
	else
		accessible.accumulate();
}

void Thread::stop() {
	accessible.dissipate();
	if(eventStarting.isSignaled())
		eventStopping.happen();
	else
		accessible.accumulate();
}

void Thread::join() {
	eventDone.beWaited();
}
