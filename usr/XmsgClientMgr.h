/*
  Copyright 2019 www.dev5.cn, Inc. dev5@qq.com
 
  This file is part of X-MSG-IM.
 
  X-MSG-IM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  X-MSG-IM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU Affero General Public License
  along with X-MSG-IM.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef USR_XMSGCLIENTMGR_H_
#define USR_XMSGCLIENTMGR_H_

#include "XmsgClient.h"

#define X_MSG_CLIENT_MGR_LOCK_COUNT																0x40	

class XmsgClientMgr
{
public:
	void addXmsgClient(shared_ptr<XmsgClient> usr); 
	bool delXmsgClient(const string& ccid); 
	shared_ptr<XmsgClient> findXmsgClient(const string& ccid); 
	shared_ptr<XmsgClient> removeXmsgClient(const string& ccid); 
	static XmsgClientMgr* instance();
private:
	unordered_map<string , shared_ptr<XmsgClient>> client[X_MSG_CLIENT_MGR_LOCK_COUNT]; 
	mutex lock[X_MSG_CLIENT_MGR_LOCK_COUNT];
	static XmsgClientMgr* inst;
	XmsgClientMgr();
	virtual ~XmsgClientMgr();
};

#endif 
