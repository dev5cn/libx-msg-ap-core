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

#include <libx-msg-ap-pb.h>
#include "XmsgClient.h"
#include "XmsgClientMgr.h"
#include "../XmsgApCfg.h"
#include "../ne/XmsgNeUsrMgrAp.h"

XmsgClient::XmsgClient(const string& cgt, const string& plat, const string& did, const string& ccid, shared_ptr<XscChannel> channel) :
		XscUsr(ccid , channel)
{
	this->cgt = cgt;
	this->plat = plat;
	this->did = did;
	this->local = true;
	this->stidSeq = Crypto::randomInt();
}

void XmsgClient::evnDisc()
{
	LOG_DEBUG("x-msg-client channel lost, this: %s", this->toString().c_str())
	if (!XmsgClientMgr::instance()->delXmsgClient(this->uid))
	{
		LOG_FAULT("it`s a bug, can not found in x-msg-client manager, this: %s", this->toString().c_str())
	}
	if (this->cgt.empty()) 
		return;
	this->evnDisc4otherNe();
}

void XmsgClient::evnDisc4otherNe()
{
	list<shared_ptr<XmsgNeUsrAp>> lis;
	XmsgNeUsrMgrAp::instance()->getSubClientDiscEventGroup(lis);
	if (lis.empty())
	{
		LOG_DEBUG("no network element group subscribe x-msg-client lost event, this: %s", this->toString().c_str())
		return;
	}
	shared_ptr<XmsgApClientLostNotice> notice(new XmsgApClientLostNotice());
	notice->set_apcgt(XmsgApCfg::instance()->cgt->toString());
	notice->set_cgt(this->cgt);
	notice->set_plat(this->plat);
	notice->set_did(this->did);
	notice->set_ccid(this->uid);
	SptrOob oob(new list<pair<uchar, string>>());
	oob->push_back(make_pair<>(XSC_TAG_UID, this->uid));
	for (auto& it : lis)
		XmsgImChannel::cast(it->channel)->unidirection(notice, oob);
}

bool XmsgClient::isAttached()
{
	return (!this->plat.empty()) && (!this->did.empty()); 
}

uint XmsgClient::addInitTrans(uint tid, const string& neg, const string& cgt)
{
	shared_ptr<x_msg_ne_init_trans_cache> cache(new x_msg_ne_init_trans_cache());
	cache->tid = tid;
	cache->neg = neg;
	cache->cgt = cgt;
	this->initTrans[++(this->stidSeq)] = cache;
	return this->stidSeq;
}

shared_ptr<x_msg_ne_init_trans_cache> XmsgClient::getInitTrans(uint tid)
{
	auto it = this->initTrans.find(tid);
	return it == this->initTrans.end() ? nullptr : it->second;
}

shared_ptr<x_msg_ne_init_trans_cache> XmsgClient::delInitTrans(uint tid)
{
	auto it = this->initTrans.find(tid);
	if (it == this->initTrans.end())
		return nullptr;
	auto cache = it->second;
	this->initTrans.erase(it);
	return cache;
}

string XmsgClient::genCcid()
{
	return XscMisc::uuid();
}

string XmsgClient::toString()
{
	string str;
	SPRINTF_STRING(&str, "cgt: %s, plat: %s, did: %s, ccid: %s, local: %s", this->cgt.c_str(), this->plat.c_str(), this->did.c_str(), this->uid.c_str(), this->local ? "true" : "false")
	return str;
}

XmsgClient::~XmsgClient()
{

}

