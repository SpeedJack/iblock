package iblock.node.common;

simple MessageDispatcher
{
	parameters:
		bool forwardBlocks = default(true);
		bool forwardTransactions = default(true);
	gates:
		inout port[];	
		output blockOut[];
		input blockIn[];
}
