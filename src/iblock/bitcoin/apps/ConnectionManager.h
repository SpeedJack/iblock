#ifndef __IBLOCK_BITCOIN_CONNECTIONMANAGER_H_
#define __IBLOCK_BITCOIN_CONNECTIONMANAGER_H_

#include "iblock/bitcoin/apps/base/AppBase.h"
#include "iblock/bitcoin/apps/contract/IConnectionManager.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API ConnectionManager : public AppBase, public IConnectionManager
{
	protected:
		std::unordered_map<unsigned int, std::vector<std::function<void(bool)>>> notifyMap;

		void startHandshake(Peer *peer);
		void completeHandshake(Peer *peer);
		void notifyApps(unsigned int peerId);
		void notifyApps(const Peer *peer) { if (peer) notifyApps(peer->getId()); }

		virtual void handleVersionPacket(Peer *peer, payloads::VersionPl *version) override;
		virtual void handleVerackPacket(Peer *peer, payloads::VerackPl *verack) override;

	public:
		virtual void initialize() override;
		virtual void connect(Peer *, std::function<void(bool)> callback) override;
};

}
}

#endif
