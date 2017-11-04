<?php 
// do not use this var or function, use ethereum_reader_Globals::getConfigProperties
function getConfigProperties() {
	global $ethereum_reader_Config;
	
	if (isset($ethereum_reader_Config))
		return $ethereum_reader_Config;
	
	// initialization of array
	$ethereum_reader_Config = array();
	
	$jsonfile = dirname(__FILE__).'/../settings/config.json';
	
	if (!file_exists($jsonfile)) {
		return $ethereum_reader_Config;
		
	}
			
	// read jsonfile
	$jsonstring = file_get_contents($jsonfile);

	$ethereum_reader_Config = json_decode($jsonstring, true);

	return $ethereum_reader_Config ;
}

$ethereum_reader_Config = getConfigProperties();

// includes for libraries

// set libraries common code dir for EthereumReaderServer
if (!isset($ethereum_reader_Config['common_library_code_dir'])) {
	require_once(dirname(__FILE__).'/../lib/common/p2pmoney_alpha/core/model/global.inc');
	p2pmoney\common\core\model\P2PMoneyAlphaDeployment::setCommonCodeDir(dirname(__FILE__).'/../lib/common');
}
else {
	require_once($ethereum_reader_Config['common_library_code_dir'].'/p2pmoney_alpha/core/model/global.inc');
	p2pmoney\common\core\model\P2PMoneyAlphaDeployment::setCommonCodeDir($ethereum_reader_Config['common_library_code_dir']);
}
	

class ethereum_reader_Globals {

	// directories
	static public function getEthereumReaderClientCodeDir() {
		return dirname(__FILE__).'/../lib/client';
	}

	static public function getEthereumReaderCommonCodeDir() {
		return dirname(__FILE__).'/../lib/common';
	}
	
	static public function logString($log) {
		p2pmoney\common\core\model\P2PMoneyAlphaGlobals::logString($log);
	}
	
	// ethereum reader server instance
	static private $ethereum_reader_server;
	
	static public function getEthereumReaderServer() {
		if (isset(ethereum_reader_Globals::$ethereum_reader_server))
			return ethereum_reader_Globals::$ethereum_reader_server;
		
		ethereum_reader_Globals::$ethereum_reader_server = ethereum_reader\rest_client\core\model\EthereumReaderServer::getServer();	
		
		$config = ethereum_reader_Globals::getConfigProperties();
		
		$configurl = (isset($config['eth_reader_rest_server_url']) ? $config['eth_reader_rest_server_url'] : 'http://ethreader.p2pmoney.org');
		$domain = (isset($config['eth_reader_domain']) ? $config['eth_reader_domain'] : '/ethereum-reader/api');
		$apikey = (isset($config['eth_reader_api_key']) ? $config['eth_reader_api_key'] : null);
		
		ethereum_reader_Globals::$ethereum_reader_server->setRestConnectionUrl($configurl);
		ethereum_reader_Globals::$ethereum_reader_server->setRestDomain($domain);
		ethereum_reader_Globals::$ethereum_reader_server->setApiKey($apikey);
		
		return ethereum_reader_Globals::$ethereum_reader_server;
	}
	
	static public function getConfigProperties() {
		// return global array
		return getConfigProperties();
	}

}
