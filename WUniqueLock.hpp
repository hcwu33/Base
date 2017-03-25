#pragma once
#include <boost/thread.hpp>
typedef boost::mutex CWUniqueLock;
typedef boost::unique_lock<CWUniqueLock> CWUniqueLockGuard;


