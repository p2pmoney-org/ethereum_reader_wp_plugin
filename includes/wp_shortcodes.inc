<?php 




function ethereum_reader_getAccountInfo($attrs = [], $content = null) {
	ethereum_reader_Globals::logString('ethereum_reader_getAccountInfo called');
	
	// get info for this account
	$ethereumserver = ethereum_reader_Globals::getEthereumReaderServer(); // initialize, if necessary, rest connection
	
	$address = $attrs['address'];
	
	ethereum_reader_Globals::logString('$attrs is '.print_r($attrs, true));
	ethereum_reader_Globals::logString('address is '.$address);
	
	$accountinfo = $ethereumserver->getAccountInfoAnswerObject($address);
	
	$balance = $accountinfo->get('balance');
	
	$content = 'balance is '.$balance.'<br>';
	
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
	
	$fromblock = $lastblocknum - $numberofblocks + 1;
	$toblock = $lastblocknum;

	$blocks = $ethereumserver->getBlockRange($fromblock, $toblock);
	
	$blockarray = (isset($blocks) ? $blocks->getBlockArray() : array());
	
	foreach($blockarray as $block) {
		$content .= 'Block number '.$block->getNumber().' difficulty '.$block->getDifficulty().' size '.$block->getSize().'<br>';
	}
	
	return $content;
}










?>