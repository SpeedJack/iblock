#ifndef IBLOCK_MODEL_BITCOIN_BLOCK_H_
#define IBLOCK_MODEL_BITCOIN_BLOCK_H_

#include <omnetpp/cobject.h>

namespace iblock {
namespace bitcoin {

class Block: public omnetpp::cObject, omnetpp::noncopyable
{
public:
	Block();
	virtual ~Block();

protected:
	unsigned int id = 0;
};

}
}

#endif
