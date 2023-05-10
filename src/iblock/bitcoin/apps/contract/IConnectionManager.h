#ifndef __IBLOCK_BITCOIN_ICONNECTIONMANAGER_H_
#define __IBLOCK_BITCOIN_ICONNECTIONMANAGER_H_

#include "iblock/bitcoin/common/Peer_m.h"

namespace iblock
{
namespace bitcoin
{

class IConnectionManager
{
	public:
		virtual void connect(Peer *peer, std::function<void(bool)> callback) = 0;
		void connect(Peer *peer, std::function<void(void)> callback)
		{ connect(peer, [callback](bool success){ if (success) callback(); }); }
		void connect(Peer *peer, std::function<void(bool, Peer *)> callback)
		{ connect(peer, [callback, peer](bool success){ callback(success, peer); }); }
		void connect(Peer *peer, std::function<void(bool, Peer)> callback)
		{ connect(peer, [callback, peer](bool success){ callback(success, *peer); }); }
};

}
}

#endif
