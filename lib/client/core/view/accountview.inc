<?php 
namespace ethereum_reader\rest_client\core\view;

require_once('view_context.inc');

use p2pmoney\common\core\model\P2PMoneyAlphaGlobals;
use ethereum_reader\rest_client\core\model\EthereumReaderServer;
use ethereum_reader\rest_client\core\model\Transactions;

class AccountView {
	private $styles;
	
	const TOP_BANNER_STYLE = 'top-banner';
	
	private $account;
	private $ethereumserver;
	
	protected function __construct($ethereumserver, $account) {
		$this->ethereumserver = $ethereumserver;
		$this->account = $account;
	}
	
	
	
	public function setStyles($styles) {
		P2PMoneyAlphaGlobals::logString('AccountView::setStyles called');
		P2PMoneyAlphaGlobals::logString('styles is '.print_r($styles, true));
		
		$this->styles = $styles;
	}
	
	protected function getAccountTopBanner($viewcontext) {
		$account = $this->account;
		$styles = (isset($this->styles) ? $this->styles : array());
		
		$widget = '';
		
	
		$address = $account->getAddress();
		$addressstring = strtoupper($address);
		
		$widget .= '<div'.(isset($styles['top_banner']) ? ' style=\''.$styles['top_banner'].'\'' : '').'>';
		$widget .= 'Address&nbsp;&nbsp;&nbsp;&nbsp;'.$addressstring;
		
		
		$widget .= '<div'.(isset($styles['top_banner_table']) ? ' style=\''.$styles['top_banner_table'].'\'' : ' style=\'top_banner_table\'').'>';
		
		$widget .= '<table>';
		
		$balancestring = EthereumReaderServer::getEtherFromWei($account->getBalance(), 2).' Ether';
		$widget .= '<tr>';
		$widget .= '<td>Balance:</td>';
		$widget .= '<td>'.$balancestring.'</td>';
		$widget .= '</tr>';
		
		$transactioncountstring = $account->getTransactionCount().' txns';
		$widget .= '<tr>';
		$widget .= '<td>N° of transactions:</td>';
		$widget .= '<td>'.$transactioncountstring.'</td>';
		$widget .= '</tr>';
		
		
		$widget .= '</table>';
		
		$widget .= '</div>';
		
		$widget .= '</div>'; // top banner div
		
		return $widget;
	}
	
	
	public function getAccountWidget($viewcontext) {
		$ethereumserver = $this->ethereumserver;
		$account = $this->account;
		$styles = (isset($this->styles) ? $this->styles : array());
		
		$widget = '';
		
		// enclosing div for widget
		$widget .= '<div'.(isset($styles['widget']) ? ' style=\''.$styles['widget'].'\'' : ' style=\'widget\'').'>';
		
		// top banner of widget
		$widget .= AccountView::getAccountTopBanner($viewcontext, $account);
	
		// navigation
		$widget .= '<div'.(isset($styles['nav_band']) ? ' style=\''.$styles['nav_band'].'\'' : ' style=\'nav_band\'').'>';
		$page_url = (isset($viewcontext) ? $viewcontext->getPageUrl() : null);
		
		if (isset($page_url)) {
			$addArgs['view'] = 'accounttxs';
			$targeturl = $viewcontext->getTargetUrl($addArgs);
				
			$widget .= '<a href=\''.$targeturl.'\'>View transactions</a><br>';
		}
		
		$widget .= '</div>';
		
		// account details
		$nonce = $account->getNonce();
		$code= $account->getCode();
		$name= $account->getName();
		$storage = $account->getStorage();
		$firstseen = $account->getFirstSeen();
		$transactioncount = $account->getTransactionCount();
		$currentblocknumber = $account->getCurrentBlockNumber();
		$new_blocks_seen = $account->getNewBlocksSeen();
		
		$widget .= '<div'.(isset($styles['account_details']) ? ' style=\''.$styles['account_details'].'\'' : ' style=\'account_details\'').'>';
		
		$widget .= '<table>';
		
		$currentblock = $ethereumserver->getBlock($currentblocknumber);
		$statementonnstring = date("D M j G:i:s T Y", $currentblock->getTime());
		
		$widget .= '<tr>';
		$widget .= '<td>From blockchain status on:</td><td>'.$statementonnstring.'</td>';
		$widget .= '</tr>';
		
		$firstseenstring = date("D M j G:i:s T Y", $firstseen);
		
		$widget .= '<tr>';
		$widget .= '<td>Tracked since:</td><td>'.$firstseenstring.'</td>';
		$widget .= '</tr>';
		
		
		$widget .= '</table>';
		
		
		
		$widget .= '</div>'; // account_details
		
		$widget .= '</div>'; // widget
		
		return $widget;
	}
	
	public function getAccountTransactionsWidget($viewcontext) {
		$ethereumserver = $this->ethereumserver;
		$account = $this->account;
		$styles = (isset($this->styles) ? $this->styles : array());
		
		$address = $account->getAddress();
	
		// enclosing div for widget
		$widget .= '<div'.(isset($styles['widget']) ? ' style=\''.$styles['widget'].'\'' : ' style=\'widget\'').'>';
		
		// top banner of widget
		$widget .= AccountView::getAccountTopBanner($viewcontext, $account);
		
		// navigation
		$widget .= '<div'.(isset($styles['nav_band']) ? ' style=\''.$styles['nav_band'].'\'' : ' style=\'nav_band\'').'>';
		$page_url = (isset($viewcontext) ? $viewcontext->getPageUrl() : null);
		
		if (isset($page_url)) {
			$addArgs['view'] = 'account';
			$targeturl = $viewcontext->getTargetUrl($addArgs);
		
			$widget .= '<a href=\''.$targeturl.'\'>View account details</a><br>';
		}

		$widget .= '</div>';
		
		
		// list of transactions
		$blockchaintxcount = $account->getTransactionCount();
		$accnttxscount = 0;
		$highestscannedblock = $account->getLastScannedBlock();
		$lowestscannedblock = $account->getFirstScannedBlock();
		
		
		
		$accounttransactions = $ethereumserver->getAccountTransactions($address);
	
		// sort on descending date and index
		if ((isset($accounttransactions)) && ($accounttransactions->getTransactionCount() > 1)) {
			$accounttransactions->sort(-1);
			$accnttxscount = $accounttransactions->getTransactionCount();
		}
	
		// list of transactions
		$widget .= '<div'.(isset($styles['txs_list']) ? ' style=\''.$styles['txs_list'].'\'' : ' style=\'txs_list\'').'>';
		
		
		$widget .= 'account has '.$blockchaintxcount.' transactions in the blockchain.<br>';
		
		if (isset($accounttransactions)) {
			$widget .= 'scan between blocks '.$lowestscannedblock.' and '.$highestscannedblock.' has found '.$accnttxscount.'  transactions so far.<br>';
		}
		else {
			$widget .= 'scan has not found any transaction for this account yet (scanned '.$lowestscannedblock.' to '.$highestscannedblock.').<br>';
		}
				
		
			
		// take first page size transactions from page number
		$pagesize = $viewcontext->getPageSize();
		$pagesize = (isset($pagesize) ? $pagesize : 10);
		$pagenumber = $viewcontext->getPageNumber();
		$pagenumber = (isset($pagenumber) ? $pagenumber : 0);
		
		$numberofpages = intval(($accnttxscount - 1) / $pagesize) + 1;
		
		// transactions for current page
		if (isset($accounttransactions)) {
			$transactions = $accounttransactions->getTransactionRange($pagenumber * $pagesize, ($pagenumber * $pagesize) + ($pagesize - 1));
		}
		else {
			$transactions = new Transactions();
		}
		
		$txscount = $transactions->getTransactionCount();
		//$widget .= 'showing '.$txscount.'  transactions.<br>';
		
		// paging
		$widget .= '<div'.(isset($styles['txs_paging']) ? ' style=\''.$styles['txs_paging'].'\'' : ' style=\'txs_paging\'').'>';

		$buttonstyle = (isset($styles['txs_nav_button']) ? ' style=\''.$styles['txs_nav_button'].'\'' : ' style=\'txs_nav_button\'');
		$buttonpagestyle = (isset($styles['txs_nav_page_button']) ? ' style=\''.$styles['txs_nav_page_button'].'\'' : ' style=\'txs_nav_page_button\'');
		
		$addArgs['view'] = 'accounttxs';
		
		$addArgs['pg'] = 0;
		$targeturl = $viewcontext->getTargetUrl($addArgs);
		$text = 'first';
	
		$first = '<div'.$buttonstyle.'>'.($pagenumber > 0 ? '<a href=\''.$targeturl.'\'>'.$text.'</a>' : $text).'</div>';

		$addArgs['pg'] = $pagenumber-1;
		$targeturl = $viewcontext->getTargetUrl($addArgs);
		$text = 'previous';
		
		$previous = '<div'.$buttonstyle.'>'.($pagenumber > 0 ? '<a href=\''.$targeturl.'\'>'.$text.'</a>' : $text).'</div>';
		
		$text = ($pagenumber + 1).' / '.$numberofpages;
		
		$page = '<div'.$buttonpagestyle.'>'.$text.'</div>';
		
		$addArgs['pg'] = $pagenumber+1;
		$targeturl = $viewcontext->getTargetUrl($addArgs);
		$text = 'next';
		
		$next = '<div'.$buttonstyle.'>'.($pagenumber < $numberofpages - 1 ? '<a href=\''.$targeturl.'\'>'.$text.'</a>' : $text).'</div>';
		
		$addArgs['pg'] = $numberofpages - 1;
		$targeturl = $viewcontext->getTargetUrl($addArgs);
		$text = 'last';
		
		$last = '<div'.$buttonstyle.'>'.($pagenumber < $numberofpages - 1 ? '<a href=\''.$targeturl.'\'>'.$text.'</a>' : $text).'</div>';
		
		$widget .= $last;
		$widget .= $next;
		$widget .= $page;
		$widget .= $previous;
		$widget .= $first;
		
		$widget .= '</div>';// txs_paging
		
		// list
		$widget .= '<table>';
		$i = 0;

		$headerstyle = (isset($styles['txs_header_row']) ? $styles['txs_header_row'] : 'txs_header_row');
		$evenstyle = (isset($styles['txs_even_row']) ? $styles['txs_even_row'] : 'txs_even_row');
		$oddstyle = (isset($styles['txs_odd_row']) ? $styles['txs_odd_row'] : 'txs_odd_row');
		
		// header
		$widget .= '<tr style=\''.$headerstyle.'\'>';
		
		$widget .= '<td>hash</td>';
		$widget .= '<td>sender</td>';
		$widget .= '<td>recipient</td>';
		$widget .= '<td>amount</td>';
		$widget .= '<td>time</td>';
		
		$widget .= '</tr>';
		
		foreach($transactions->getTransactionArray() as $transaction) {
			$style = ($i % 2 == 0 ? $evenstyle : $oddstyle);
			
			$hash = $transaction->getHash();
			$sender = $transaction->getSender();
			$recipient = $transaction->getRecipient();
			$amount = $transaction->getAmount();
			$blockid = $transaction->getBlockId();
			$time = $transaction->getTime();
			$txindex = $transaction->getTransactionIndex();
			
			$hashstring = substr($hash, 0, 10).'...';
			$senderstring = substr($sender, 0, 10).'...';
			$recipientstring = substr($recipient, 0, 10).'...';
			$amountstring = $ethereumserver->getEtherFromWei($amount).' Ether';
			$timestring = date("Y-m-d H:i:s",$time);
				
			
			$widget .= '<tr style=\''.$style.'\'>';
			
			$widget .= '<td>'.$hashstring.'</td>';
			$widget .= '<td>'.$senderstring.'</td>';
			$widget .= '<td>'.$recipientstring.'</td>';
			$widget .= '<td>'.$amountstring.'</td>';
			$widget .= '<td>'.$timestring.'</td>';
			
			$widget .= '</tr>';
				
			$i++;
		}
		
		$widget .= '</table>';
		
		$widget .= '</div>';// txs_list
		
	
		$widget .= '</div>'; // widget
		
		return $widget;
	}
	
	// static
	static public function createAccountView($ethereumserver, $account) {
		$accountview = new AccountView($ethereumserver, $account);
		
		return $accountview;
	}
	
	
}


?>

