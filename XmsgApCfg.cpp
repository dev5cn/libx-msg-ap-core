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

#include "XmsgApCfg.h"

shared_ptr<XmsgApCfg> XmsgApCfg::cfg = nullptr; 

XmsgApCfg::XmsgApCfg()
{

}

shared_ptr<XmsgApCfg> XmsgApCfg::instance()
{
	return XmsgApCfg::cfg;
}

void XmsgApCfg::setCfg(shared_ptr<XmsgApCfg> cfg)
{
	XmsgApCfg::cfg = cfg;
}

shared_ptr<XmsgApCfg> XmsgApCfg::load(const char* path)
{
	XMLDocument doc;
	if (doc.LoadFile(path) != 0)
	{
		LOG_ERROR("load config file failed, path: %s", path)
		return nullptr;
	}
	XMLElement* root = doc.RootElement();
	if (root == NULL)
	{
		LOG_ERROR("it a empty xml file? path: %s", path)
		return nullptr;
	}
	shared_ptr<XmsgApCfg> cfg(new XmsgApCfg());
	cfg->cfgPb.reset(new XmsgApCfgPb());
	cfg->cgt = ChannelGlobalTitle::parse(Misc::strAtt(root, "cgt"));
	if (cfg->cgt == nullptr)
	{
		LOG_ERROR("channel global title format error: %s", cfg->cfgPb->cgt().c_str())
		return nullptr;
	}
	cfg->cfgPb->set_cgt(cfg->cgt->toString());
	if (!cfg->loadLogCfg(root))
		return nullptr;
	if (!cfg->loadXscServerCfg(root))
		return nullptr;
	if (!cfg->loadXmsgNeN2hCfg(root))
		return nullptr;
	if (!cfg->loadMiscCfg(root))
		return nullptr;
	LOG_INFO("load x-msg-ap config file successful, cfg: %s", cfg->toString().c_str())
	XmsgApCfg::setCfg(cfg);
	return cfg;
}

bool XmsgApCfg::loadLogCfg(XMLElement* root)
{
	auto node = root->FirstChildElement("log");
	if (node == NULL)
	{
		LOG_ERROR("load config failed, node: <log>");
		return false;
	}
	XmsgApCfgLog* log = this->cfgPb->mutable_log();
	log->set_level(Misc::toUpercase(Misc::strAtt(node, "level")));
	log->set_output(Misc::toUpercase(Misc::strAtt(node, "output")));
	return true;
}

bool XmsgApCfg::loadXscServerCfg(XMLElement* root)
{
	XMLElement* node = root->FirstChildElement("xsc-server"); 
	while (node != NULL)
	{
		string name;
		Misc::strAtt(node, "name", &name);
		if ("pub-tcp" == name)
		{
			auto pub = this->loadXscTcpCfg(node);
			if (pub != nullptr)
				this->cfgPb->mutable_pubtcp()->CopyFrom(*pub);
			node = node->NextSiblingElement("xsc-server");
			continue;
		}
		if ("pub-http" == name)
		{
			auto pub = this->loadXscHttpCfg(node);
			if (pub != nullptr)
				this->cfgPb->mutable_pubhttp()->CopyFrom(*pub);
			node = node->NextSiblingElement("xsc-server");
			continue;
		}
		if ("pub-websocket" == name)
		{
			auto pub = this->loadXscWebSocketCfg(node);
			if (pub != nullptr)
				this->cfgPb->mutable_pubwebsocket()->CopyFrom(*pub);
			node = node->NextSiblingElement("xsc-server");
			continue;
		}
		if ("pri-tcp" == name)
		{
			auto pri = this->loadXscTcpCfg(node);
			if (pri == nullptr)
				return false;
			this->cfgPb->mutable_pritcp()->CopyFrom(*pri);
			node = node->NextSiblingElement("xsc-server");
			continue;
		}
		LOG_ERROR("unexpected xsc server name: %s, node: <xsc-server>", name.c_str())
		return false;
	}
	if (!this->cfgPb->has_pubtcp() && !this->cfgPb->has_pubhttp() && !this->cfgPb->has_pubwebsocket() && !this->cfgPb->has_pubudp() && !this->cfgPb->has_pubrudp())
	{
		LOG_ERROR("load x-msg-im-ap config failed, node: <xsc-server>, no public server")
		return false;
	}
	if (!this->cfgPb->has_pritcp())
	{
		LOG_ERROR("load x-msg-im-ap config failed, node: <xsc-server>, no private server")
		return false;
	}
	return true;
}

shared_ptr<XmsgApCfgXscTcpServer> XmsgApCfg::loadXscTcpCfg(XMLElement* node)
{
	if (node == NULL)
		return nullptr;
	shared_ptr<XmsgApCfgXscTcpServer> tcpServer(new XmsgApCfgXscTcpServer());
	tcpServer->set_addr(Misc::strAtt(node, "addr"));
	tcpServer->set_worker(Misc::hexOrInt(node, "worker"));
	tcpServer->set_peerlimit(Misc::hexOrInt(node, "peerLimit"));
	tcpServer->set_peermtu(Misc::hexOrInt(node, "peerMtu"));
	tcpServer->set_peerrcvbuf(Misc::hexOrInt(node, "peerRcvBuf"));
	tcpServer->set_peersndbuf(Misc::hexOrInt(node, "peerSndBuf"));
	tcpServer->set_lazyclose(Misc::hexOrInt(node, "lazyClose"));
	tcpServer->set_tracing("true" == Misc::strAtt(node, "tracing"));
	tcpServer->set_heartbeat(Misc::hexOrInt(node, "heartbeat"));
	tcpServer->set_n2hzombie(Misc::hexOrInt(node, "n2hZombie"));
	tcpServer->set_n2htranstimeout(Misc::hexOrInt(node, "n2hTransTimeout"));
	tcpServer->set_n2htracing("true" == Misc::strAtt(node, "n2hTracing"));
	tcpServer->set_h2nreconn(Misc::hexOrInt(node, "h2nReConn"));
	tcpServer->set_h2ntranstimeout(Misc::hexOrInt(node, "h2nTransTimeout"));
	return tcpServer;
}

shared_ptr<XmsgApCfgXscHttpServer> XmsgApCfg::loadXscHttpCfg(XMLElement* node)
{
	if (node == NULL)
		return nullptr;
	shared_ptr<XmsgApCfgXscHttpServer> httpServer(new XmsgApCfgXscHttpServer());
	httpServer->mutable_tcp()->CopyFrom(*this->loadXscTcpCfg(node));
	httpServer->set_headerlimit(Misc::hexOrInt(node, "headerLimit"));
	httpServer->set_bodylimit(Misc::hexOrInt(node, "bodyLimit"));
	httpServer->set_closewait(Misc::hexOrInt(node, "closeWait"));
	vector<string> header;
	Misc::split(Misc::strAtt(node, "requiredHeader"), ",", header);
	for (auto& it : header)
		httpServer->add_requiredheader(it);
	return httpServer;
}

shared_ptr<XmsgApCfgXscWebSocketServer> XmsgApCfg::loadXscWebSocketCfg(XMLElement* node)
{
	if (node == NULL)
		return nullptr;
	shared_ptr<XmsgApCfgXscWebSocketServer> webSocketServer(new XmsgApCfgXscWebSocketServer());
	webSocketServer->mutable_tcp()->CopyFrom(*this->loadXscTcpCfg(node));
	return webSocketServer;
}

shared_ptr<XmsgApCfgXscUdpServer> XmsgApCfg::loadXscUdpCfg(XMLElement* node)
{
	if (node == NULL)
		return nullptr;
	return nullptr;
}

shared_ptr<XmsgApCfgXscRudpServer> XmsgApCfg::loadXscRudpCfg(XMLElement* node)
{
	if (node == NULL)
		return nullptr;
	return nullptr;
}

bool XmsgApCfg::loadXmsgNeN2hCfg(XMLElement* root)
{
	XMLElement* node = root->FirstChildElement("ne-group-n2h");
	if (node == NULL)
	{
		LOG_ERROR("load config failed, node: <ne-group-n2h>");
		return false;
	}
	node = node->FirstChildElement("ne");
	while (node != NULL)
	{
		auto ne = this->cfgPb->add_n2h();
		Misc::strAtt(node, "neg", ne->mutable_neg());
		Misc::strAtt(node, "cgt", ne->mutable_cgt());
		Misc::strAtt(node, "prefix", ne->mutable_prefix());
		Misc::strAtt(node, "pwd", ne->mutable_pwd());
		Misc::strAtt(node, "addr", ne->mutable_addr());
		node = node->NextSiblingElement("ne");
	}
	if (this->cfgPb->n2h().empty())
	{
		LOG_ERROR("load config failed, node: <ne-group-n2h>")
		return false;
	}
	return true;
}

bool XmsgApCfg::loadMiscCfg(XMLElement* root)
{
	return true;
}

shared_ptr<XscTcpCfg> XmsgApCfg::pubXscTcpServerCfg()
{
	return this->cfgPb->has_pubtcp() ? this->xmsgApCfgXscTcpServer2xscTcpCfg(&this->cfgPb->pubtcp()) : nullptr;
}

shared_ptr<XscHttpCfg> XmsgApCfg::pubXscHttpServerCfg()
{
	if (!this->cfgPb->has_pubhttp())
		return nullptr;
	shared_ptr<XscHttpCfg> httpCfg(new XscHttpCfg());
	httpCfg->addr = this->cfgPb->pubhttp().tcp().addr();
	httpCfg->worker = this->cfgPb->pubhttp().tcp().worker();
	httpCfg->peerLimit = this->cfgPb->pubhttp().tcp().peerlimit();
	httpCfg->peerMtu = this->cfgPb->pubhttp().tcp().peermtu();
	httpCfg->peerRcvBuf = this->cfgPb->pubhttp().tcp().peerrcvbuf();
	httpCfg->peerSndBuf = this->cfgPb->pubhttp().tcp().peersndbuf();
	httpCfg->lazyClose = this->cfgPb->pubhttp().tcp().lazyclose();
	httpCfg->tracing = this->cfgPb->pubhttp().tcp().tracing();
	httpCfg->heartbeat = this->cfgPb->pubhttp().tcp().heartbeat();
	httpCfg->n2hZombie = this->cfgPb->pubhttp().tcp().n2hzombie();
	httpCfg->n2hTransTimeout = this->cfgPb->pubhttp().tcp().n2htranstimeout();
	httpCfg->n2hTracing = this->cfgPb->pubhttp().tcp().n2htracing();
	httpCfg->h2nReConn = this->cfgPb->pubhttp().tcp().h2nreconn();
	httpCfg->h2nTransTimeout = this->cfgPb->pubhttp().tcp().h2ntranstimeout();
	httpCfg->headerLimit = this->cfgPb->pubhttp().headerlimit();
	httpCfg->bodyLimit = this->cfgPb->pubhttp().bodylimit();
	httpCfg->closeWait = this->cfgPb->pubhttp().closewait();
	for (auto& it : this->cfgPb->pubhttp().requiredheader())
		httpCfg->requiredHeader.insert(it);
	return httpCfg;
}

shared_ptr<XscWebSocketCfg> XmsgApCfg::pubXscWebSocketServerCfg()
{
	if (!this->cfgPb->has_pubwebsocket())
		return nullptr;
	shared_ptr<XscWebSocketCfg> webSocketCfg(new XscWebSocketCfg());
	webSocketCfg->addr = this->cfgPb->pubwebsocket().tcp().addr();
	webSocketCfg->worker = this->cfgPb->pubwebsocket().tcp().worker();
	webSocketCfg->peerLimit = this->cfgPb->pubwebsocket().tcp().peerlimit();
	webSocketCfg->peerMtu = this->cfgPb->pubwebsocket().tcp().peermtu();
	webSocketCfg->peerRcvBuf = this->cfgPb->pubwebsocket().tcp().peerrcvbuf();
	webSocketCfg->peerSndBuf = this->cfgPb->pubwebsocket().tcp().peersndbuf();
	webSocketCfg->lazyClose = this->cfgPb->pubwebsocket().tcp().lazyclose();
	webSocketCfg->tracing = this->cfgPb->pubwebsocket().tcp().tracing();
	webSocketCfg->heartbeat = this->cfgPb->pubwebsocket().tcp().heartbeat();
	webSocketCfg->n2hZombie = this->cfgPb->pubwebsocket().tcp().n2hzombie();
	webSocketCfg->n2hTransTimeout = this->cfgPb->pubwebsocket().tcp().n2htranstimeout();
	webSocketCfg->n2hTracing = this->cfgPb->pubwebsocket().tcp().n2htracing();
	webSocketCfg->h2nReConn = this->cfgPb->pubwebsocket().tcp().h2nreconn();
	webSocketCfg->h2nTransTimeout = this->cfgPb->pubwebsocket().tcp().h2ntranstimeout();
	return webSocketCfg;
}

shared_ptr<XscTcpCfg> XmsgApCfg::priXscTcpServerCfg()
{
	return this->xmsgApCfgXscTcpServer2xscTcpCfg(&this->cfgPb->pritcp());
}

shared_ptr<XscTcpCfg> XmsgApCfg::xmsgApCfgXscTcpServer2xscTcpCfg(const XmsgApCfgXscTcpServer* pb)
{
	shared_ptr<XscTcpCfg> tcpCfg(new XscTcpCfg());
	tcpCfg->addr = pb->addr();
	tcpCfg->worker = pb->worker();
	tcpCfg->peerLimit = pb->peerlimit();
	tcpCfg->peerMtu = pb->peermtu();
	tcpCfg->peerRcvBuf = pb->peerrcvbuf();
	tcpCfg->peerSndBuf = pb->peersndbuf();
	tcpCfg->lazyClose = pb->lazyclose();
	tcpCfg->tracing = pb->tracing();
	tcpCfg->heartbeat = pb->heartbeat();
	tcpCfg->n2hZombie = pb->n2hzombie();
	tcpCfg->n2hTransTimeout = pb->n2htranstimeout();
	tcpCfg->n2hTracing = pb->n2htracing();
	tcpCfg->h2nReConn = pb->h2nreconn();
	tcpCfg->h2nTransTimeout = pb->h2ntranstimeout();
	return tcpCfg;
}

string XmsgApCfg::toString()
{
	return this->cfgPb->ShortDebugString();
}

XmsgApCfg::~XmsgApCfg()
{

}

