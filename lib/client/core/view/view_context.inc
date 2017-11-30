<?php
namespace ethereum_reader\rest_client\core\view;

use p2pmoney\common\core\model\P2PMoneyAlphaGlobals;

class ViewContext {
	private $locale;
	
	private $page_base_url;
	private $page_args;
	
	private $view;
	
	private $bPreview;
	
	private $tab;
	private $pagenumber;
	private $pagesize;
	
	protected function __construct() {
	}
	
	
	public function getLocale() {
		return $this->locale;
	}
	
	public function setLocale($locale) {
		$this->locale = $locale;
	}
	
	public function getPageUrl() {
		return ViewContext::getFullUrl($this->page_base_url, $this->page_args);
	}
	
	private function setPageUrl($page_url) {
		$this->page_base_url = strtok($page_url, '?');
		
		$querystring = (($pos = strpos($page_url, '?')) !== false ? substr($page_url, $pos+1) : null);
		
		if (isset($querystring))
			parse_str($querystring, $this->page_args);
		else 
			$this->page_args = null;
		
	}
	
	public function getTargetUrl($addArgs) {
		$args = null;
		
		if (isset($this->page_args)) {
			$args = array_merge($this->page_args, $addArgs);
		}
		else if (isset($addArgs) && count($addArgs)) {
			$args = $addArgs;
		}
		
		return ViewContext::getFullUrl($this->page_base_url, $args);
	}
	
	public function getRequestedView() {
		return $this->view;
	}
	
	public function setRequestedView($view) {
		$this->view = $view;
	}
	
	public function isPreview() {
		return $this->bPreview;
	}
	
	public function setIsPreview($bchoice) {
		$this->bPreview = $bchoice;
	}
	
	public function getTab() {
		return $this->tab;
	}
	
	public function setTab($tab) {
		$this->tab = $tab;
	}
	
	public function getPageNumber() {
		return $this->pagenumber;
	}
	
	public function setPageNumber($pagenumber) {
		$this->pagenumber = $pagenumber;
	}
	
	public function getPageSize() {
		return $this->pagesize;
	}
	
	public function setPageSize($pagesize) {
		$this->pagesize = $pagesize;
	}
	
	// static
	private static function getFullUrl($baseurl, $queryarrays) {
		$page_url = $baseurl;
		
		if (isset($queryarrays) && (count($queryarrays))) {
			foreach( $queryarrays as $key => $key_value ){
				//$queryarr[] = urlencode( $key ) . '=' . urlencode( $key_value );
				$queryarr[] = $key . '=' . $key_value;
			}
			
			$page_url .= '?'.implode( '&', $queryarr);
			
		}
			
		return $page_url;
	}
	
	static public function createViewContext($page_url) {
		
		$viewcontext = new ViewContext();

		
		$viewcontext->setPageUrl($page_url);
		
		return $viewcontext;
	}
	
}