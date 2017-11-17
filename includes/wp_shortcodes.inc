<?php 




function ethereum_reader_getAccountInfo($attrs = [], $content = null) {
	ethereum_reader_Globals::logString('ethereum_reader_getAccountInfo called');
	
	// get info for this account
	$ethereumserver = ethereum_reader_Globals::getEthereumReaderServer(); // initialize, if necessary, rest connection
	
	$address = $attrs['address'];
	
	ethereum_reader_Globals::logString('$attrs is '.print_r($attrs, true));
	ethereum_reader_Globals::logString('address is '.$address);
	
	$account = $ethereumserver->getAccount($address);
	
	// query result
	$address = $account->getAddress();
	$content = 'address is '.$address.'<br>';
	
	$balance = $account->getBalance();
	$content .= 'balance is '.$balance.'<br>';
	
	$nonce = $account->getNonce();
	$content .= 'nonce is '.$nonce.'<br>';
	
	$code= $account->getCode();
	//$content .= 'code is '.$code.'<br>';
	
	$name= $account->getName();
	$content .= 'name is '.$name.'<br>';
	
	$storage = $account->getStorage();
	$content .= 'storage is '.$storage.'<br>';
	
	$firstseen = $account->getFirstSeen();
	$content .= 'firstseen is '.$firstseen.'<br>';
	
	$transactioncount = $account->getTransactionCount();
	$content .= 'transactioncount is '.$transactioncount.'<br>';
	
	$currentblocknumber = $account->getCurrentBlockNumber();
	$content .= 'currentblocknumber is '.$currentblocknumber.'<br>';
	
	$new_blocks_seen = $account->getNewBlocksSeen();
	$content .= 'new_blocks_seen is '.$new_blocks_seen.'<br>';
	
	
	return $content;
}

function ethereum_reader_getLastBlocksInfo($attrs = [], $content = null) {
	ethereum_reader_Globals::logString('ethereum_reader_getLastBlocksInfo called');

	// get info for this account
	$ethereumserver = ethereum_reader_Globals::getEthereumReaderServer(); // initialize, if necessary, rest connection

	$numberofblocks = (isset($attrs['number']) ? $attrs['number'] : 10);

	ethereum_reader_Globals::logString('$attrs is '.print_r($attrs, true));
	ethereum_reader_Globals::logString('numberofblocks is '.$numberofblocks);

	$lastblocknum = ethereum_reader\rest_client\core\model\Block::getLastBlockNumber();
	
	$content = 'last block is '.$lastblocknum.'<br>';
	$content .= 'showing last '.$numberofblocks.' blocks'.'<br>';
	
	$blocks = $ethereumserver->getLastBlocks($numberofblocks, $numberofblocks);
	
	$blockarray = (isset($blocks) ? $blocks->getBlockArray() : array());
	
	foreach($blockarray as $block) {
		$content .= 'Block number '.$block->getNumber().' difficulty '.$block->getDifficulty().' size '.$block->getSize().'<br>';
	}
	
	return $content;
}




function ethereum_reader_getLastTransactionsInfo($attrs = [], $content = null) {
	ethereum_reader_Globals::logString('ethereum_reader_getLastTransactionsInfo called');

	// get info for this account
	$ethereumserver = ethereum_reader_Globals::getEthereumReaderServer(); // initialize, if necessary, rest connection

	$numberoftransactions = (isset($attrs['number']) ? $attrs['number'] : 10);

	ethereum_reader_Globals::logString('$attrs is '.print_r($attrs, true));
	ethereum_reader_Globals::logString('numberoftransactions is '.$numberoftransactions);

	$content = 'transactions'.'<br>';
	
	$transactions = $ethereumserver->getLastTransactions($numberoftransactions, $numberoftransactions);

	$transactionarray = (isset($transactions) ? $transactions->getTransactionArray() : array());

	foreach($transactionarray as $transaction) {
		$content .= '<table>';
		$content .= '<tr><td>hash</td><td>'.$transaction->getHash().'</td></tr>';
		$content .= '<tr><td>sender</td><td>'.$transaction->getSender().'</td></tr>';
		$content .= '<tr><td>recipient</td><td>'.$transaction->getRecipient().'</td></tr>';
		$content .= '<tr><td>amount</td><td>'.$transaction->getAmount().'</td></tr>';
		$content .= '<tr><td>block id</td><td>'.$transaction->getBlockId().'</td></tr>';
		$content .= '<tr><td>index</td><td>'.$transaction->getTransactionIndex().'</td></tr>';
		$content .= '</table>';
	}

	return $content;
}









?>