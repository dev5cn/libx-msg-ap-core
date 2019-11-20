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

#ifndef USR_XMSGCLIENT_H_
#define USR_XMSGCLIENT_H_

#include <libx-msg-im-xsc.h>

typedef struct
{
	uint tid; 
	string neg; 
	string cgt; 
} x_msg_ne_init_trans_cache; 

class XmsgClient: public XscUsr
{
public:
	string cgt; 
	string plat; 
	string did; 
	bool local; 
public:
	void evnDisc(); 
	bool isAttached(); 
	uint addInitTrans(uint tid, const string& neg, const string& cgt); 
	shared_ptr<x_msg_ne_init_trans_cache> getInitTrans(uint tid); 
	shared_ptr<x_msg_ne_init_trans_cache> delInitTrans(uint tid); 
	static string genCcid(); 
	string toString();
	XmsgClient(const string& cgt, const string& plat, const string& did, const string& ccid, shared_ptr<XscChannel> channel);
	virtual ~XmsgClient();
private:
	unordered_map<uint , shared_ptr<x_msg_ne_init_trans_cache>> initTrans; 
	uint stidSeq; 
	void evnDisc4otherNe(); 
};

typedef shared_ptr<XmsgClient> SptrClient;

#endif 
