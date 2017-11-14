/* 
 *  Copyright (c) 2010,
 *  Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 *
 *  This file is part of crtmpserver.
 *  crtmpserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  crtmpserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with crtmpserver.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _BASEINNETSTREAM_H
#define	_BASEINNETSTREAM_H

#include "streaming/baseinstream.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_BUF_LEN 128
#define MSG_KEY 0x7654

typedef struct {
	long mtype;
	char mbuf[MSG_BUF_LEN];
} Msg;

class DLLEXP BaseInNetStream
: public BaseInStream {
private:
	time_t _expirationTimestamp;
	time_t _lastReportTimestamp;
	int _reportMsgqId;
public:
	BaseInNetStream(BaseProtocol *pProtocol, StreamsManager *pStreamsManager,
			uint64_t type, string name);
	virtual ~BaseInNetStream();

	uint32_t RedisReport(bool);
	bool IsExpired();
};

#endif	/* _BASEINNETSTREAM_H */

