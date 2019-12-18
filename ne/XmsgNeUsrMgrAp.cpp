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

#include "XmsgNeUsrMgrAp.h"

XmsgNeUsrMgrAp* XmsgNeUsrMgrAp::inst = new XmsgNeUsrMgrAp();

XmsgNeUsrMgrAp::XmsgNeUsrMgrAp()
{

}

XmsgNeUsrMgrAp* XmsgNeUsrMgrAp::instance()
{
	return XmsgNeUsrMgrAp::inst;
}

shared_ptr<XmsgNeUsrAp> XmsgNeUsrMgrAp::add(shared_ptr<XmsgNeUsrAp> nu)
{
	unique_lock<mutex> lock(this->lock4nes);
	auto it = this->nes.find(nu->neg);
	if (it != this->nes.end())
	{
		shared_ptr<XmsgNeUsrAp> old = it->second;
		it->second = nu;
		return old;
	}
	this->nes[nu->neg] = nu;
	return nullptr;
}

shared_ptr<XmsgNeUsrAp> XmsgNeUsrMgrAp::get(const string& neg)
{
	unique_lock<mutex> lock(this->lock4nes);
	auto it = this->nes.find(neg);
	return it == this->nes.end() ? nullptr : it->second;
}

shared_ptr<XmsgNeUsrAp> XmsgNeUsrMgrAp::getAuth()
{
	return this->get(X_MSG_IM_AUTH);
}

shared_ptr<XmsgNeUsrAp> XmsgNeUsrMgrAp::getStatus()
{
	return this->get(X_MSG_CHANNEL_STATUS);
}

shared_ptr<XmsgNeUsrAp> XmsgNeUsrMgrAp::getHlr()
{
	return this->get(X_MSG_IM_HLR);
}

shared_ptr<XmsgNeUsrAp> XmsgNeUsrMgrAp::getGroup()
{
	return this->get(X_MSG_IM_GROUP);
}

shared_ptr<XmsgNeUsrAp> XmsgNeUsrMgrAp::getOrg()
{
	return this->get(X_MSG_IM_ORG);
}

shared_ptr<XmsgNeUsrAp> XmsgNeUsrMgrAp::findByCgt(const string& cgt)
{
	unique_lock<mutex> lock(this->lock4nes);
	for (auto& it : this->nes)
	{
		if (it.second->uid == cgt)
			return it.second;
	}
	return nullptr;
}

shared_ptr<XmsgNeUsrAp> XmsgNeUsrMgrAp::remove(const string& neg)
{
	unique_lock<mutex> lock(this->lock4nes);
	auto it = this->nes.find(neg);
	if (it == this->nes.end())
		return nullptr;
	shared_ptr<XmsgNeUsrAp> ne = it->second;
	this->nes.erase(it);
	return ne;
}

shared_ptr<XmsgNeUsrAp> XmsgNeUsrMgrAp::addSubClientEstbEventGroup(shared_ptr<XmsgNeUsrAp> nu)
{
	unique_lock<mutex> lock(this->lock4sub);
	auto it = this->subClientEstbEventGroup.find(nu->neg);
	if (it != this->subClientEstbEventGroup.end())
	{
		shared_ptr<XmsgNeUsrAp> old = it->second;
		it->second = nu;
		return old;
	}
	this->subClientEstbEventGroup[nu->neg] = nu;
	return nullptr;
}

void XmsgNeUsrMgrAp::getSubClientEstbEventGroup(list<shared_ptr<XmsgNeUsrAp>>& lis)
{
	unique_lock<mutex> lock(this->lock4sub);
	for (auto& it : this->subClientEstbEventGroup)
		lis.push_back(it.second);
}

shared_ptr<XmsgNeUsrAp> XmsgNeUsrMgrAp::addSubClientDiscEventGroup(shared_ptr<XmsgNeUsrAp> nu)
{
	unique_lock<mutex> lock(this->lock4sub);
	auto it = this->subClientDiscEventGroup.find(nu->neg);
	if (it != this->subClientDiscEventGroup.end())
	{
		shared_ptr<XmsgNeUsrAp> old = it->second;
		it->second = nu;
		return old;
	}
	this->subClientDiscEventGroup[nu->neg] = nu;
	return nullptr;
}

void XmsgNeUsrMgrAp::getSubClientDiscEventGroup(list<shared_ptr<XmsgNeUsrAp>>& lis)
{
	unique_lock<mutex> lock(this->lock4sub);
	for (auto& it : this->subClientDiscEventGroup)
		lis.push_back(it.second);
}

shared_ptr<XmsgNeUsrAp> XmsgNeUsrMgrAp::findByMsgName(const string& msg)
{
	string neg;
	unique_lock<mutex> lock0(this->lock4msgName4neg);
	auto it = this->msgName4neg.find(msg);
	if (it != this->msgName4neg.end())
		neg = it->second;
	lock0.unlock();
	if (!neg.empty()) 
		return this->get(neg); 
	shared_ptr<XmsgNeUsrAp> nu = nullptr;
	unique_lock<mutex> lock1(this->lock4nes);
	for (auto& it : this->nes)
	{
		shared_ptr<XmsgNeUsrAp> n = static_pointer_cast<XmsgNeUsrAp>(it.second);
		if (msg.find(n->prefix) != 0)
			continue;
		nu = n;
		break;
	}
	lock1.unlock();
	if (nu == nullptr)
		return nullptr;
	unique_lock<mutex> lock2(XmsgNeUsrMgrAp::lock4msgName4neg);
	XmsgNeUsrMgrAp::msgName4neg[msg] = nu->neg;
	return nu;
}

XmsgNeUsrMgrAp::~XmsgNeUsrMgrAp()
{

}

