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

#ifndef NE_XMSGNEUSRMGRAP_H_
#define NE_XMSGNEUSRMGRAP_H_

#include "XmsgNeUsrAp.h"

class XmsgNeUsrMgrAp
{
public:
	shared_ptr<XmsgNeUsrAp> add(shared_ptr<XmsgNeUsrAp> nu); 
	shared_ptr<XmsgNeUsrAp> get(const string& neg); 
	shared_ptr<XmsgNeUsrAp> getAuth(); 
	shared_ptr<XmsgNeUsrAp> getStatus(); 
	shared_ptr<XmsgNeUsrAp> getHlr(); 
	shared_ptr<XmsgNeUsrAp> getGroup(); 
	shared_ptr<XmsgNeUsrAp> getOrg(); 
	shared_ptr<XmsgNeUsrAp> findByCgt(const string& cgt); 
	shared_ptr<XmsgNeUsrAp> remove(const string& neg); 
	static XmsgNeUsrMgrAp* instance();
public:
	shared_ptr<XmsgNeUsrAp> addSubClientEstbEventGroup(shared_ptr<XmsgNeUsrAp> group); 
	void getSubClientEstbEventGroup(list<shared_ptr<XmsgNeUsrAp>>& lis); 
	shared_ptr<XmsgNeUsrAp> addSubClientDiscEventGroup(shared_ptr<XmsgNeUsrAp> group); 
	void getSubClientDiscEventGroup(list<shared_ptr<XmsgNeUsrAp>>& lis); 
public:
	shared_ptr<XmsgNeUsrAp> findByMsgName(const string& msg); 
public:
	unordered_map<string , shared_ptr<XmsgNeUsrAp>> nes; 
	unordered_map<string , shared_ptr<XmsgNeUsrAp>> subClientEstbEventGroup; 
	unordered_map<string , shared_ptr<XmsgNeUsrAp>> subClientDiscEventGroup; 
	unordered_map<string , shared_ptr<XmsgNeUsrAp>> prefixGroup; 
	mutex lock4nes; 
	mutex lock4sub; 
	mutex lock4prefix; 
private:
	static XmsgNeUsrMgrAp* inst;
	XmsgNeUsrMgrAp();
	virtual ~XmsgNeUsrMgrAp();
};

#endif 
