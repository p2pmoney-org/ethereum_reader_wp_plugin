<?php 
require_once(dirname(__FILE__) . '/./control/controller.inc');


use ethereum_reader\rest_client\core\view\ViewContext;

// accounts

function ethereum_reader_getAccountInfo($attrs = [], $content = null) {
	ethereum_reader_Globals::logString('shortcode - ethereum_reader_getAccountInfo called');
	
	ethereum_reader_Globals::logString('$attrs is '.print_r($attrs, true));
	
	// get info for this account
	$ethereumserver = ethereum_reader_Globals::getEthereumReaderServer(); // initialize, if necessary, rest connection
	
	$address = $attrs['address'];
	
	$account = $ethereumserver->getAccount($address);
	
	
	$view = (isset($_GET['view']) ?  $_GET['view'] : 'account');
	$page = (isset($_GET['pg']) ?  $_GET['pg'] : 0);
	$accounthash = (isset($_GET['account']) ?  $_GET['account'] : null);
	$blockid = (isset($_GET['block']) ?  $_GET['block'] : null);
	$twhash = (isset($_GET['tx']) ?  $_GET['tx'] : null);
	
	$strip = array('view', 'account', 'block', 'tx', 'pg');
	$current_url = ethereum_reader_Globals::getCurrentPageUrl($strip);
	
	// create the view context
	$viewcontext = ViewContext::createViewContext($current_url);
	
	$viewcontext->setRequestedView($view);
	$viewcontext->setPageNumber($page);
	
	// ask the view from the controller
	$content = ethereum_reader_Controller::getAccountView($viewcontext, $ethereumserver, $account);
	
	return $content;
}


function ethereum_reader_getAccountTransactionsInfo($attrs = [], $content = null) {
	ethereum_reader_Globals::logString('shortcode - ethereum_reader_getAccountTransactionsInfo called');

	ethereum_reader_Globals::logString('$attrs is '.print_r($attrs, true));
	
	// get info for this account
	$ethereumserver = ethereum_reader_Globals::getEthereumReaderServer(); // initialize, if necessary, rest connection

	$address = $attrs['address'];
	
	$content = '';
	
	$account = $ethereumserver->getAccount($address);
	
	$strip = array('action', 'account', 'block', 'tx');
	$current_url = ethereum_reader_Globals::getCurrentPageUrl($strip);
	
	$viewcontext = ViewContext::createViewContext($page_url);
	
	$content .= AccountView::getAccountTransactionsWidget($viewcontext, $ethereumserver, $account);
	
	return $content;
}

// block

function ethereum_reader_getBlockInfo($attrs = [], $content = null) {
	ethereum_reader_Globals::logString('shortcode - ethereum_reader_getBlockInfo called');

	ethereum_reader_Globals::logString('$attrs is '.print_r($attrs, true));
	
	// get info for this account
	$ethereumserver = ethereum_reader_Globals::getEthereumReaderServer(); // initialize, if necessary, rest connection
	
	$lastblocknum = $ethereumserver->getBlocksCount();
	$blocknumber = (isset($attrs['blocknumber']) ? $attrs['blocknumber'] : $lastblocknum);
	
	$block = $ethereumserver->getBlock($blocknumber);
	
	$content .= ethereum_reader_getBlockWidget($block);
	
	return $content;
	
}

function ethereum_reader_getBlockTransactionsInfo($attrs = [], $content = null) {
	ethereum_reader_Globals::logString('shortcode - ethereum_reader_getBlockTransactionsInfo called');

	ethereum_reader_Globals::logString('$attrs is '.print_r($attrs, true));
	
	// get info for this account
	$ethereumserver = ethereum_reader_Globals::getEthereumReaderServer(); // initialize, if necessary, rest connection

	$lastblocknum = $ethereumserver->getBlocksCount();
	$blocknumber = (isset($attrs['blocknumber']) ? $attrs['blocknumber'] : $lastblocknum);
	
	
	$transactions = $ethereumserver->getBlockTransactions($blocknumber);

	$transactionarray = (isset($transactions) ? $transactions->getTransactionArray() : array());

	$content .= ethereum_reader_getTransactionsWidget($transactions);
	
	return $content;
}


function ethereum_reader_getBlockWidget($block) {
	$widget = '';
	
	$widget .= '<table>';
	$widget .= '<tr><td>Block number:</td><td>'.$block->getNumber().'</td></tr>';
	$widget .= '<tr><td>difficulty:</td><td>'.$block->getDifficulty().'</td></tr>';
	$widget .= '<tr><td>size:</td><td>'.$block->getSize().'</td></tr>';
	$widget .= '<tr><td>reward:</td><td>'.$block->getReward().'</td></tr>';
	$widget .= '</table>';
	
	return $widget;
}

// blocks
function ethereum_reader_getBlocksWidget($blocks) {
	$widget = '';

	$blockarray = (isset($blocks) ? $blocks->getBlockArray() : array());
	
	$widget .= 'List of '.count($blockarray).'  blocks.<br>';
	
	foreach($blockarray as $block) {
		$widget .= ethereum_reader_getBlockWidget($block);
	}
	
	return $widget;
}


function ethereum_reader_getLastBlocksInfo($attrs = [], $content = null) {
	ethereum_reader_Globals::logString('shortcode - ethereum_reader_getLastBlocksInfo called');

	ethereum_reader_Globals::logString('$attrs is '.print_r($attrs, true));
	
	// get info for this account
	$ethereumserver = ethereum_reader_Globals::getEthereumReaderServer(); // initialize, if necessary, rest connection

	$numberofblocks = (isset($attrs['number']) ? $attrs['number'] : 10);

	ethereum_reader_Globals::logString('$attrs is '.print_r($attrs, true));
	ethereum_reader_Globals::logString('numberofblocks is '.$numberofblocks);

	$lastblocknum = $ethereumserver->getBlocksCount();
	
	$content = 'last block is '.$lastblocknum.'<br>';
	$content .= 'showing last '.$numberofblocks.' blocks'.'<br>';
	
	$blocks = $ethereumserver->getLastBlocks($numberofblocks, $numberofblocks);
	
	$content .= ethereum_reader_getBlocksWidget($blocks);
	
	return $content;
}



// transactions
function ethereum_reader_getTransactionWidget($transaction) {
	$widget = '';

	$widget .= '<table>';
	$widget .= '<tr><td>hash</td><td>'.$transaction->getHash().'</td></tr>';
	$widget .= '<tr><td>sender</td><td>'.$transaction->getSender().'</td></tr>';
	$widget .= '<tr><td>recipient</td><td>'.$transaction->getRecipient().'</td></tr>';
	$widget .= '<tr><td>amount</td><td>'.$transaction->getAmount().'</td></tr>';
	$widget .= '<tr><td>block id</td><td>'.$transaction->getBlockId().'</td></tr>';
	$widget .= '<tr><td>time</td><td>'.date('Y-m-d H:i:s', $transaction->getTime()).'</td></tr>';
	$widget .= '<tr><td>index</td><td>'.$transaction->getTransactionIndex().'</td></tr>';
	$widget .= '</table>';
	
	return $widget;

}


function ethereum_reader_getTransactionsWidget($transactions) {
	$widget = '';
	
	$transactionarray = (isset($transactions) ? $transactions->getTransactionArray() : array());
	
	$widget .= 'Showing list of '.count($transactionarray).' transactions.<br>';
	
	foreach($transactionarray as $transaction) {
		$widget .= ethereum_reader_getTransactionWidget($transaction);
	}
	
	return $widget;

}

function ethereum_reader_getLastTransactionsInfo($attrs = [], $content = null) {
	ethereum_reader_Globals::logString('shortcode - ethereum_reader_getLastTransactionsInfo called');

	ethereum_reader_Globals::logString('$attrs is '.print_r($attrs, true));
	
	// get info for this account
	$ethereumserver = ethereum_reader_Globals::getEthereumReaderServer(); // initialize, if necessary, rest connection

	$numberoftransactions = (isset($attrs['number']) ? $attrs['number'] : 10);

	ethereum_reader_Globals::logString('$attrs is '.print_r($attrs, true));
	ethereum_reader_Globals::logString('numberoftransactions is '.$numberoftransactions);

	$content = 'transactions'.'<br>';
	
	$transactions = $ethereumserver->getLastTransactions($numberoftransactions, $numberoftransactions);

	$transactionarray = (isset($transactions) ? $transactions->getTransactionArray() : array());

	$content .= ethereum_reader_getTransactionsWidget($transactions);
	
	return $content;
}

function ethereum_reader_getTransactionInfo($attrs = [], $content = null) {
	ethereum_reader_Globals::logString('shortcode - ethereum_reader_getLastTransactionsInfo called');

	ethereum_reader_Globals::logString('$attrs is '.print_r($attrs, true));
	
	// get info for this account
	$ethereumserver = ethereum_reader_Globals::getEthereumReaderServer(); // initialize, if necessary, rest connection
	
	$hash = (isset($attrs['hash']) ? $attrs['hash'] : null);
	
	if (isset($hash)) {
		$transaction = $ethereumserver->getTransaction($hash);
		
		$content = ethereum_reader_getTransactionWidget($transaction);
	}
	else {
		$content = 'you must specify a hash parameter in the short code';
	}
	
	return $content;
}








?>