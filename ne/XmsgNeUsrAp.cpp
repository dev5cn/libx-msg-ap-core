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

#include "XmsgNeUsrAp.h"
#include "XmsgNeUsrMgrAp.h"
#include "../XmsgApCfg.h"
#include "../usr/XmsgClientMgr.h"

XmsgNeUsrAp::XmsgNeUsrAp(const string& neg, const string& cgt, shared_ptr<XscChannel> channel, const string& prefix) :
		XmsgNeUsr(neg, cgt, channel)
{
	this->prefix = prefix;
}

void XmsgNeUsrAp::forward(shared_ptr<XscChannel> clientChannel , SptrClient client , shared_ptr<XscProtoPdu> pdu)
{
	int len = 0;
	uchar* dat = pdu->bytes(&len);
	if (Log::isRecord())
	{
		bool exp;
		auto p = XscProtoPdu::decode(dat, len, &exp);
		LOG_RECORD("\n  --> PEER: %s CFD: %d NE: %s\n%s", 
				this->channel->peer.c_str(),
				this->channel->cfd,
				this->uid.c_str(),
				p == nullptr ? "exception" : p->print(dat, len).c_str())
	}
	this->channel->send(dat, len);
}

void XmsgNeUsrAp::receive(shared_ptr<XscProtoPdu> pdu)
{
	string ccid;
	if (!pdu->transm.getOob(XSC_TAG_UID, ccid)) 
	{
		LOG_FAULT("it`s a bug, must be have x-msg-im-client channel id, this: %s", this->toString().c_str())
		return;
	}
	SptrClient client = XmsgClientMgr::instance()->findXmsgClient(ccid); 
	if (client == nullptr) 
	{
		LOG_DEBUG("can not found x-msg-im-client for ccid: %s, may be it was lost", ccid.c_str())
		return;
	}
	if (pdu->transm.trans->refDat && client->wk != this->wk)
		pdu->transm.trans->cloneDat();
	bool tracing = this->channel->worker->server->tracing; 
	string cgt = this->uid;
	string neg = this->neg;
	client->future([client, pdu, neg, cgt, tracing]
	{
		XmsgNeUsrAp::receiveOnClientThread(client, pdu, neg, cgt, tracing);
	});
}

void XmsgNeUsrAp::receiveOnClientThread(SptrClient client, shared_ptr<XscProtoPdu> pdu, const string& neg, const string& cgt , bool tracing)
{
	if (pdu->transm.trans != NULL && pdu->transm.trans->trans == XSC_TAG_TRANS_BEGIN)
		pdu->transm.trans->stid = client->addInitTrans(pdu->transm.trans->stid, neg, cgt); 
	pdu->takeoffHeader(); 
	int len;
	uchar* dat = pdu->bytes(&len);
	client->channel->send(dat, len);
	if (Log::isRecord())
	{
		bool exp;
		auto p = XscProtoPdu::decode(dat, len, &exp);
		LOG_RECORD("\n  --> PEER: %s CFD: %d NE: %s\n%s\n", client->channel->peer.c_str(), client->channel->cfd, client->cgt.c_str(), p == nullptr ? "exception" : p->print(dat, len).c_str())
	}
}

void XmsgNeUsrAp::evnDisc()
{
	LOG_ERROR("%s channel lost, this: %s", this->neg.c_str(), this->toString().c_str())
	auto u = XmsgNeUsrMgrAp::instance()->remove(this->neg);
	if (u == nullptr)
	{
		LOG_FAULT("it`s a bug, neg: %s, this: %s", this->neg.c_str(), this->toString().c_str())
		return;
	}
}

XmsgNeUsrAp::~XmsgNeUsrAp()
{

}

