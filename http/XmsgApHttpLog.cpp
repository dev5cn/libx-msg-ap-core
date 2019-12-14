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

#include "XmsgApHttpLog.h"

XmsgApHttpLog::XmsgApHttpLog() :
		XmsgImHttpLog()
{

}

shared_ptr<XscHttpChannel> XmsgApHttpLog::newXscHttpChannel(XscHttpWorker* wk, int cfd, const string& peer)
{
	return shared_ptr<XmsgImHttpChannel>(new XmsgImHttpChannel(wk, wk->mtu, cfd, peer));
}

void XmsgApHttpLog::didaMainThread(ullong now)
{

}

void XmsgApHttpLog::dida(ullong now)
{

}

void XmsgApHttpLog::rx(XscChannel* channel, uchar* dat, int len)
{

}

void XmsgApHttpLog::tx(XscChannel* channel, uchar* dat, int len)
{

}

void XmsgApHttpLog::transInitStart(SptrXiti trans, shared_ptr<XscProtoPdu> pdu )
{

}

void XmsgApHttpLog::transInitFinished(SptrXiti trans, shared_ptr<XscProtoPdu> pdu )
{

}

void XmsgApHttpLog::transPassStart(SptrXitp trans, shared_ptr<XscProtoPdu> pdu )
{

}

void XmsgApHttpLog::transPassFinished(SptrXitp trans, shared_ptr<XscProtoPdu> pdu )
{

}

void XmsgApHttpLog::transInitUni(SptrXitui trans, shared_ptr<XscProtoPdu> pdu )
{

}

void XmsgApHttpLog::transPassUni(SptrXitup trans, shared_ptr<XscProtoPdu> pdu )
{

}

XmsgApHttpLog::~XmsgApHttpLog()
{

}
