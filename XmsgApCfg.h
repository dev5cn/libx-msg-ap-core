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

#ifndef XMSGAPCFG_H_
#define XMSGAPCFG_H_

#include <libx-msg-common-dat-struct-cpp.h>
#include <libx-msg-ap-pb.h>

class XmsgApCfg
{
public:
	shared_ptr<XmsgApCfgPb> cfgPb; 
	SptrCgt cgt; 
public:
	static shared_ptr<XmsgApCfg> instance(); 
	static shared_ptr<XmsgApCfg> load(const char* path); 
	static void setCfg(shared_ptr<XmsgApCfg> cfg); 
	shared_ptr<XscTcpCfg> pubXscTcpServerCfg(); 
	shared_ptr<XscHttpCfg> pubXscHttpServerCfg(); 
	shared_ptr<XscWebSocketCfg> pubXscWebSocketServerCfg(); 
	shared_ptr<XscTcpCfg> priXscTcpServerCfg(); 
	string toString();
	XmsgApCfg();
	virtual ~XmsgApCfg();
private:
	static shared_ptr<XmsgApCfg> cfg; 
	bool loadLogCfg(XMLElement* node); 
	bool loadXscServerCfg(XMLElement* root); 
	bool loadXmsgNeN2hCfg(XMLElement* root); 
	bool loadMiscCfg(XMLElement* root); 
private:
	shared_ptr<XmsgApCfgXscTcpServer> loadXscTcpCfg(XMLElement* node); 
	shared_ptr<XmsgApCfgXscHttpServer> loadXscHttpCfg(XMLElement* node); 
	shared_ptr<XmsgApCfgXscWebSocketServer> loadXscWebSocketCfg(XMLElement* node); 
	shared_ptr<XmsgApCfgXscUdpServer> loadXscUdpCfg(XMLElement* node); 
	shared_ptr<XmsgApCfgXscRudpServer> loadXscRudpCfg(XMLElement* node); 
	shared_ptr<XscTcpCfg> xmsgApCfgXscTcpServer2xscTcpCfg(const XmsgApCfgXscTcpServer* pb); 
};

#endif 
