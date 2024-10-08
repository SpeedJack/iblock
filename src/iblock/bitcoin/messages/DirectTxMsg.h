#ifndef __IBLOCK_BITCOIN_DIRECTTXMSG_H_
#define __IBLOCK_BITCOIN_DIRECTTXMSG_H_

#include "DirectTxMsg_m.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API DirectTxMsg : public DirectTxMsg_Base
{
	private:
		void copy(const DirectTxMsg& other) { this->tx = other.tx; }

	protected:
		std::shared_ptr<Transaction> tx;

	public:
		DirectTxMsg() : DirectTxMsg_Base() { setName("tx"); }
		DirectTxMsg(std::shared_ptr<Transaction> tx) : DirectTxMsg() { setTx(tx); }
		DirectTxMsg(const DirectTxMsg& other) : DirectTxMsg_Base(other) { copy(other); }
		DirectTxMsg& operator=(const DirectTxMsg& other) { if (this == &other) return *this; DirectTxMsg_Base::operator=(other); copy(other); return *this; }

		virtual DirectTxMsg* dup() const override { return new DirectTxMsg(*this); }

		virtual const Transaction* getTx() const override { return tx.get(); }
		virtual std::shared_ptr<Transaction> getTxSharedPtr() const { return tx; }
		virtual std::weak_ptr<Transaction> getTxWeakPtr() const { return tx; }
		virtual void setTx(Transaction* tx) override { this->tx = std::shared_ptr<Transaction>(tx); }
		virtual void setTx(std::shared_ptr<Transaction> tx) { this->tx = tx; }
};

}
}

#endif
