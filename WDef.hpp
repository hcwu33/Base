#pragma once

typedef int BOOL;

#define  FALSE		(0)
#define  TRUE		(1)

#define W_DATE_TIME_STR_LEN	20


#define W_NEW_BUFFER(p, t) \
do \
{ \
	p = new t; \
	if (NULL == p) \
	{ \
		assert(false); \
	} \
} while (0);

#define W_DELETE_BUFFER(p) \
do {\
	if (NULL != p) {\
		delete p; \
		p = NULL; \
	} \
} while (0)

#define W_RELEASE_BUFFER_ARRAY(p) \
if (NULL != p) \
{ \
	delete[]p; \
	p = NULL; \
	}

#define W_CHECK_INIT_START(flag) \
do {\
	if (flag) {\
		return true; \
	} \
	flag = true; \
} while (0)

#define W_CHECK_UNINIT_STOP(flag) \
do {\
	if (!flag) {\
		return; \
	} \
	flag = false; \
} while (0)

#define W_INIT_FAILED() \
	this->UnInit(); \
	return false;

