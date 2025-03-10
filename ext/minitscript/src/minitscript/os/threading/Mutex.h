#pragma once

#include <minitscript/os/threading/fwd-minitscript.h>

#include <minitscript/minitscript.h>

#include <mutex>
#include <string>

using std::mutex;
using std::string;

/**
 * Mutex implementation.
 * Mutexes are used to ensure that only one process can run a critical section,
 * which is e.g. modifying shared data between thread.
 * @author Andreas Drewke
 */
class minitscript::os::threading::Mutex {
public:
	// forbid class copy
	_FORBID_CLASS_COPY(Mutex)

	/**
	 * @brief Public constructor
	 * @param name name
	 */
	inline Mutex(const string& name): name(name) {};

	/**
	 * @brief Destroys the mutex
	 */
	inline ~Mutex() {}

	/**
	 * @brief Tries to locks the mutex
	 */
	inline bool tryLock() {
		return stlMutex.try_lock();
	}

	/**
	 * @brief Locks the mutex, additionally mutex locks will block until other locks have been unlocked.
	 */
	inline void lock() {
		stlMutex.lock();
	}

	/**
	 * @brief Unlocks this mutex
	 */
	inline void unlock() {
		stlMutex.unlock();
	}

private:
	string name;
	mutex stlMutex;
};
