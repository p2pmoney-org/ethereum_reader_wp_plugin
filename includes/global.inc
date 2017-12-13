<?php 

define('TABLE_PLUGIN_ADDITONAL_PREFIX', 'eth_reader_');


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
	
use ethereum_reader\rest_client\core\model\EthereumReaderServer;

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
		if (isset(ethereum_reader_Globals::$ethereum_reader_server)) {
			if (ethereum_reader_Globals::$ethereum_reader_server->checkServer()) // throws exception if not ok
			return ethereum_reader_Globals::$ethereum_reader_server;
		}
		
		ethereum_reader_Globals::$ethereum_reader_server = EthereumReaderServer::getServer();	
		
		$config = ethereum_reader_Globals::getConfigProperties();
		
		// rest connection to ethereum_reader_server
		$configurl = (isset($config['eth_reader_rest_server_url']) ? $config['eth_reader_rest_server_url'] : 'http://eth-api.p2pmoney.org');
		$domain = (isset($config['eth_reader_domain']) ? $config['eth_reader_domain'] : '/ethereum-reader/api');
		$apikey = (isset($config['eth_reader_api_key']) ? $config['eth_reader_api_key'] : 'wordpress');
		
		ethereum_reader_Globals::$ethereum_reader_server->setRestConnectionUrl($configurl);
		ethereum_reader_Globals::$ethereum_reader_server->setRestDomain($domain);
		ethereum_reader_Globals::$ethereum_reader_server->setApiKey($apikey);
		
		// database connection to wordpress database
		$database = CONSTANT('DB_NAME');
		$username = CONSTANT('DB_USER');
		$password = CONSTANT('DB_PASSWORD');
		$host = CONSTANT('DB_HOST');
		$port = null; // wordpress puts the port in the hostname (e.g. localhost:3306)
		$table_prefix = ethereum_reader_Globals::getPluginTablePrefix();
		
		ethereum_reader_Globals::$ethereum_reader_server->setDBName($database);
		ethereum_reader_Globals::$ethereum_reader_server->setDBUserName($username);
		ethereum_reader_Globals::$ethereum_reader_server->setDBPassword($password);
		ethereum_reader_Globals::$ethereum_reader_server->setDBHost($host);
		ethereum_reader_Globals::$ethereum_reader_server->setDBPort($port);
		ethereum_reader_Globals::$ethereum_reader_server->setDBTablePrefix($table_prefix);
		
		// enable DB caching
		
		// for saving to DB
		$bCacheSaveToDB = (isset($config['eth_reader_db_caching_save']) && ($config['eth_reader_db_caching_save'] == 0) ? false : true);
		
		ethereum_reader_Globals::$ethereum_reader_server->setCacheSaveToDB($bCacheSaveToDB);
		
		$iCacheSaveMode = (isset($config['eth_reader_db_caching_save_all']) && ($config['eth_reader_db_caching_save_all'] == 1) ? EthereumReaderServer::CACHE_SAVE_ALL : EthereumReaderServer::CACHE_SAVE_TX_FOR_KNOWN_ACCOUNTS | EthereumReaderServer::CACHE_SAVE_BLOCKS_FOR_KNOWN_TX);
		ethereum_reader_Globals::$ethereum_reader_server->setCacheSaveToDBMode($iCacheSaveMode);
		
		// for reading from DB
		$bCacheReadFromDB = (isset($config['eth_reader_db_caching_read']) && ($config['eth_reader_db_caching_read'] == 0) ? false : true);
		
		ethereum_reader_Globals::$ethereum_reader_server->setCacheReadFromDB($bCacheReadFromDB);
		
		$iCacheReadMode = (isset($config['eth_reader_db_caching_read_all']) && ($config['eth_reader_db_caching_read_all'] == 1) ? EthereumReaderServer::CACHE_READ_ALL : EthereumReaderServer::CACHE_READ_TX_FOR_KNOWN_ACCOUNTS);
		ethereum_reader_Globals::$ethereum_reader_server->setCacheReadFromDBMode($iCacheReadMode);
		
		ethereum_reader_Globals::logString('***************************************************************************');
		ethereum_reader_Globals::logString('Initializing EthereumReaderServer');
		ethereum_reader_Globals::logString('Rest server url is                 :'.$configurl);
		ethereum_reader_Globals::logString('Rest server domain is              :'.$domain);
		ethereum_reader_Globals::logString('DB Name is                         :'.$database);
		ethereum_reader_Globals::logString('DB User is                         :'.$username);
		ethereum_reader_Globals::logString('DB table prefix is                 :'.$table_prefix);
		ethereum_reader_Globals::logString('Cache Save to DB is                :'.($bCacheSaveToDB ? 'true' : 'false'));
		ethereum_reader_Globals::logString('Cache Save mode is                 :'.$iCacheSaveMode);
		ethereum_reader_Globals::logString('Cache Read from DB is              :'.($bCacheReadFromDB ? 'true' : 'false'));
		ethereum_reader_Globals::logString('Cache Read mode is                 :'.$iCacheReadMode);
		ethereum_reader_Globals::logString('***************************************************************************');
		
		// check remote server support (version). Throws an exception if not ok
		ethereum_reader_Globals::$ethereum_reader_server->checkServer();
		
		ethereum_reader_Globals::logString('Ethereum Reader REST Server is OK');
		
		return ethereum_reader_Globals::$ethereum_reader_server;
	}
	
	static public function getPluginTablePrefix() {
		global $wpdb;
	
		return $wpdb->prefix.CONSTANT('TABLE_PLUGIN_ADDITONAL_PREFIX');
	}
	
	
	
	static public function getConfigProperties() {
		// return global array
		return getConfigProperties();
	}
	
	static public function getPageBaseURL() {
		$pageURL = 'http';
		if ($_SERVER["HTTPS"] == "on") {
			$pageURL .= "s";
		}
		$pageURL .= "://";
		if ($_SERVER["SERVER_PORT"] != "80") {
			$pageURL .= $_SERVER["SERVER_NAME"] . ":" .
					$_SERVER["SERVER_PORT"] . strtok($_SERVER["REQUEST_URI"],'?');
		} else {
			$pageURL .= $_SERVER["SERVER_NAME"] . strtok($_SERVER["REQUEST_URI"],'?');
		}
		return $pageURL;
	}	
	
	static public function getCurrentPageUrl($strip = null) {
	
		$baseurl = ethereum_reader_Globals::getPageBaseURL();
	
		$querystring = $_SERVER['QUERY_STRING'];
		parse_str($querystring, $queryarray);
	
		if (isset($strip)) {
			// remove
			foreach($strip as $key=>$value) {
				if (isset($queryarray[$value]))
					unset($queryarray[$value]);
			}
	
			// rebuild the stripped query string
			foreach( $queryarray as $key => $key_value ){
				//$queryarr[] = urlencode( $key ) . '=' . urlencode( $key_value );
				$queryarr[] = $key . '=' . $key_value;
			}
	
			if (isset($queryarr))
				$querystring = implode( '&', $queryarr );
			else
				$querystring = '';
				
		}
	
		return add_query_arg( $querystring, '', $baseurl);
	}
	
	
}
