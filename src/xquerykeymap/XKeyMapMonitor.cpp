/*
 * Copyright (c) 2011, Benjamin Peter <BenjaminPeter@arcor.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Benjamin Peter ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Benjamin Peter BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <string>

#include "XKeyMapMonitor.h"

#include <cstring>
#include <iostream> /* debug */
#include <set>

extern "C"
{
#include <X11/Xlib.h>
#include <unistd.h>
}

#include "XKeyMapConstants.h"
#include "XKeyMapListener.h"
#include "IllegalStateException.h"
#include "XUtilException.h"

namespace xutil
{

/* pimpl impl */

/**
 * Implementation of the xkeymap monitor.
 *
 * It periodically queries the keymap and compares it for any changes. Upon change
 * all listeners are notified.
 *
 * Improvement ideas:
 *  - make loop timeout configurable
 *  - run listener notification optionally asynchronously
 */
class XKeyMapMonitorImpl
{
public:
	XKeyMapMonitorImpl();
	~XKeyMapMonitorImpl();

	void connect(const std::string &displayName);
	void disconnect();
	void start();
	void stop();
	bool isRunning();
	std::string getDisplayName();

	void addListener(XKeyMapListener *listener);
	bool removeListener(XKeyMapListener *listener);

private:
	/* constants */

	/*
	 * STARTING: we initiated thread starting and wait for it to come up
	 * RUNNING: the thread has started and is running
	 * STOPPING: we are telling the loop to stop but it is not done yet
	 * STOPPED: the thread is not running or was stopped
	 */
	enum RunState
	{
		STARTING, RUNNING, STOPPING, STOPPED
	};

	/* sleep in the busy loop of the monitor */
	static const useconds_t LOOP_WAIT = 10000;

private:
	/* methods */

	/* mutex protected run state */
	RunState getRunState();
	void setRunState(const RunState &runState);

	/* allow using the object as run target for pthread */
	static void* runObject(void *object);

	/* monitoring loop */
	void run();

	/* Notify all listeners about the change, done during the loop, time critical */
	void notifyListeners(const char* keyMap, const char* keyMapPrev);

private:
	/* variables */
	Display *m_display;
	char m_keyMap[KEYMAP_SIZE_BYTES];
	char m_keyMapPrev[KEYMAP_SIZE_BYTES];
	pthread_t m_thread;
	pthread_mutex_t m_runStateMutex;
	RunState m_runState;
	std::set<XKeyMapListener*> m_listeners;
};

XKeyMapMonitorImpl::XKeyMapMonitorImpl() :
	m_display(0), m_runState(STOPPED)
{
	/* Initialize the keymap with a common value */
	memset(m_keyMap, 0, KEYMAP_SIZE_BYTES);
	memset(m_keyMapPrev, 0, KEYMAP_SIZE_BYTES);
	pthread_mutex_init(&m_runStateMutex, 0);
}

XKeyMapMonitorImpl::~XKeyMapMonitorImpl()
{
	pthread_mutex_destroy(&m_runStateMutex);
}

void XKeyMapMonitorImpl::connect(const std::string &displayName)
{
	if (m_display != 0) {
		throw IllegalStateException("Already connected, display already open.");
	}
	if (getRunState() != STOPPED) {
		throw IllegalStateException(
				"Monitoring already running, can't connect afterwards.");
	}

	const char *displayNameChar = 0;
	if (!displayName.empty()) {
		displayNameChar = displayName.c_str();
	}
	m_display = XOpenDisplay(displayNameChar);
}

void XKeyMapMonitorImpl::disconnect()
{
	if (m_display != 0) {
		XFree(m_display);
		m_display = 0;
	}
}

std::string XKeyMapMonitorImpl::getDisplayName()
{
	if (!m_display) {
		throw IllegalStateException(
				"Not connected yet, no is display set. Use connect first.");
	}

	return std::string(XDisplayString(m_display));
}

void XKeyMapMonitorImpl::start()
{
	if (getRunState() != STOPPED) {
		throw IllegalStateException(
				"Monitoring already running, can't use start again. Stop it first");
	}
	if (!m_display) {
		throw IllegalStateException(
				"Not connected yet, no is display set. Use connect first.");
	}

	setRunState(STARTING);
	int rc;
	rc = pthread_create(&m_thread, NULL, runObject,
			reinterpret_cast<void*> (this));

	if (rc != 0) {
		throw IllegalStateException(
				"An error occurred while starting the monitor thread.");
	}
}

void XKeyMapMonitorImpl::stop()
{
	if (getRunState() != RUNNING) {
		throw IllegalStateException("Monitoring not running, can't use stop.");
	}

	setRunState(STOPPING);
	while (getRunState() != STOPPED)
		usleep(LOOP_WAIT / 2);
}

bool XKeyMapMonitorImpl::isRunning()
{
	return getRunState() == RUNNING;
}

void XKeyMapMonitorImpl::run()
{
	setRunState(RUNNING);

	while (getRunState() == RUNNING) {

		XQueryKeymap(m_display, m_keyMap);

		if (memcmp(m_keyMap, m_keyMapPrev, KEYMAP_SIZE_BYTES) != 0) {
			memcpy(m_keyMapPrev, m_keyMap, KEYMAP_SIZE_BYTES);

			notifyListeners(m_keyMap, m_keyMapPrev);
		}
		/* no need to copy if nothing changes */

		usleep(LOOP_WAIT);
	}

	setRunState(STOPPED);
	pthread_exit(NULL);
}

void* XKeyMapMonitorImpl::runObject(void *object)
{
	XKeyMapMonitorImpl* out = reinterpret_cast<XKeyMapMonitorImpl*> (object);
	out->run();
	return 0;
}

XKeyMapMonitorImpl::RunState XKeyMapMonitorImpl::getRunState()
{
	RunState state;
	pthread_mutex_lock(&m_runStateMutex);
	state = m_runState;
	pthread_mutex_unlock(&m_runStateMutex);
	return state;
}

void XKeyMapMonitorImpl::setRunState(const RunState &runState)
{
	pthread_mutex_lock(&m_runStateMutex);
	m_runState = runState;
	pthread_mutex_unlock(&m_runStateMutex);
}

void XKeyMapMonitorImpl::addListener(XKeyMapListener *listener)
{
	m_listeners.insert(listener);
}

bool XKeyMapMonitorImpl::removeListener(XKeyMapListener *listener)
{
	return m_listeners.erase(listener) != 0;
}

void XKeyMapMonitorImpl::notifyListeners(const char *keyMap,
		const char *keyMapPrev)
{
	std::set<XKeyMapListener*>::iterator it = m_listeners.begin();
	while (it != m_listeners.end()) {
		(*it)->onKeyMapChanged(keyMap, keyMapPrev);
		++it;
	}
}

/* public delegation methods */

XKeyMapMonitor::XKeyMapMonitor() :
	m_pimpl(new XKeyMapMonitorImpl())
{

}

XKeyMapMonitor::~XKeyMapMonitor()
{
	delete m_pimpl;
}

void XKeyMapMonitor::connect(const std::string & displayName)
{
	m_pimpl->connect(displayName);
}

std::string XKeyMapMonitor::getDisplayName()
{
	return m_pimpl->getDisplayName();
}

void XKeyMapMonitor::start()
{
	m_pimpl->start();
}

void XKeyMapMonitor::stop()
{
	m_pimpl->stop();
}

bool XKeyMapMonitor::isRunning()
{
	return m_pimpl->isRunning();
}

void XKeyMapMonitor::addListener(XKeyMapListener *listener)
{
	m_pimpl->addListener(listener);
}

bool XKeyMapMonitor::removeListener(XKeyMapListener *listener)
{
	return m_pimpl->removeListener(listener);
}

}
