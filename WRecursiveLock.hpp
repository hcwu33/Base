#pragma once
#include <boost/thread.hpp>

typedef boost::recursive_mutex CWRecursiveLock;
typedef boost::recursive_mutex::scoped_lock CWRecursiveLockGuard;
