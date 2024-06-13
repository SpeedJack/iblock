#ifndef __IBLOCK_BITCOIN_WALLET_H_
#define __IBLOCK_BITCOIN_WALLET_H_

#include "iblock/bitcoin/apps/base/AppBase.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API Wallet : public AppBase
{
	protected:
		virtual void initialize() override;

	public:
		Wallet() : AppBase() { }
};

}
}

#endif
