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


#include "streaming/baseinnetstream.h"
#include "streaming/streamstypes.h"

BaseInNetStream::BaseInNetStream(BaseProtocol *pProtocol,
		StreamsManager *pStreamsManager, uint64_t type, string name)
: BaseInStream(pProtocol, pStreamsManager, type, name) {
	if (!TAG_KIND_OF(type, ST_IN_NET)) {
		ASSERT("Incorrect stream type. Wanted a stream type in class %s and got %s",
				STR(tagToString(ST_IN_NET)), STR(tagToString(type)));
	}

	_reportMsgqId = msgget(MSG_KEY, IPC_CREAT|0666);
	_lastReportTimestamp = 0;
	_expirationTimestamp = time(0) + 60;
}

BaseInNetStream::~BaseInNetStream() {
}

uint32_t BaseInNetStream::RedisReport(bool set) {
	time_t currentTimestamp = time(0);
	if (currentTimestamp - _lastReportTimestamp < 10) {
		//INFO("[%s] diff: %ld", STR(_name), currentTimestamp - _lastReportTimestamp);
		return 0;
	}
	_lastReportTimestamp = currentTimestamp;

	INFO("[%s] set: %d", STR(_name), set);
	Msg msg;
	memset(&msg, 0, sizeof(msg));
	msg.mtype = set ? 1 : 2;
	snprintf(msg.mbuf, MSG_BUF_LEN, "%s", STR(_name));
	return msgsnd(_reportMsgqId, &msg, sizeof(msg), IPC_NOWAIT);
}

bool BaseInNetStream::IsExpired() {
	time_t currentTimestamp = time(0);

	if (currentTimestamp >= _expirationTimestamp) {
		if (GetOutStreams().size() == 0)
		  return true;
		_expirationTimestamp = currentTimestamp + 60;
	}
	return false;
}

