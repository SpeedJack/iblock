package iblock.simulations;
import iblock.bitcoin.node.MinerNode;
import iblock.bitcoin.node.UserNode;
import iblock.bitcoin.global.NodeManager;
import iblock.bitcoin.global.WalletManager;
import iblock.bitcoin.global.GBM;

network BitcoinNetwork
{
	parameters:
		@display("bgb=1000,1000");

		int numNodes = default(50);
		int numMiners = default(10);

		@statistic[circulatingSupply](source=sum(addUTXO)-sum(removeUTXO); record=vector?,stats?,last; unit=sat; interpolationmode=sample-hold; title="Circulating Supply");
		@statistic[walletAddresses](source=count(createAddress); record=vector?,last; unit=addresses; interpolationmode=sample-hold; title="Network Wallet Addresses");
		@statistic[transactions](source=count(txSize); record=vector?,last,stats?; unit=txs; interpolationmode=sample-hold; title="Transactions");
		@statistic[fees](source=sum(txFee); record=vector?,last,stats?; unit=sat; interpolationmode=sample-hold; title="Fees");
		@statistic[blocks](source=minedBlock; record=vector?,count,timeavg; unit=blocks; interpolationmode=sample-hold; title="Blocks");
		@statistic[processedTransactions](source=processedTransactions; record=vector?,stats,sum,histogram?; unit=txs; interpolationmode=sample-hold; title="Processed Transactions");
	submodules:
		nodeManager: NodeManager;
		walletManager: WalletManager;
		gbm: GBM;
		node[numNodes-numMiners]: UserNode {
		};
		miner[numMiners]: MinerNode {
		};
}
