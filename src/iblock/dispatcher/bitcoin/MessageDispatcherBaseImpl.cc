#include "../base/MessageDispatcherBaseImpl.h"

#include "../../messages/bitcoin/MessageKind_m.h"

namespace iblock
{

template class MessageDispatcherBase<bitcoin::MessageKind>;

}
