<?php
namespace p2pmoney\common\core\model;


class P2PMoneyAlphaDeployment {
	
	static public $root_dir = '/';

	static private $codedir;
	
	static private $clientcodedir;
	static private $commoncodedir;
	static private $servercodedir;
	static private $restclientcodedir;
	static private $restservercodedir;
	
	static private $datadir;
	
	static private $modules_datadirs;
	
	// root dir
	static public function setRootDir($rootdir) {
		P2PMoneyAlphaDeployment::$root_dir = $rootdir;
	}

	// code root dir
	static public function getCodeDir() {
		if (isset(P2PMoneyAlphaDeployment::$codedir))
			return P2PMoneyAlphaDeployment::$codedir;
		else
			return realpath(dirname(__FILE__)).'/../../../../';
	}

	static public function setCodeDir($dir) {
		P2PMoneyAlphaDeployment::$codedir = $dir;
	}

	// client code dir
	static public function getClientCodeDir() {
		if (isset(P2PMoneyAlphaDeployment::$clientcodedir))
			return P2PMoneyAlphaDeployment::$clientcodedir;
		else
			return P2PMoneyAlphaDeployment::getCodeDir().'/client';
	}

	static public function setClientCodeDir($dir) {
		P2PMoneyAlphaDeployment::$clientcodedir = $dir;
	}

	// rest client code dir
	static public function getRestClientCodeDir() {
		if (isset(P2PMoneyAlphaDeployment::$restclientcodedir))
			return P2PMoneyAlphaDeployment::$restclientcodedir;
		else
			return P2PMoneyAlphaDeployment::getCodeDir().'/rest_client';
	}

	static public function setRestClientCodeDir($dir) {
		P2PMoneyAlphaDeployment::$restclientcodedir = $dir;
	}

	// common code dir
	static public function getCommonCodeDir() {
		if (isset(P2PMoneyAlphaDeployment::$commoncodedir))
			return P2PMoneyAlphaDeployment::$commoncodedir;
		else
			return realpath(dirname(__FILE__)).'/../../../';
	}

	static public function setCommonCodeDir($dir) {
		P2PMoneyAlphaDeployment::$commoncodedir = $dir;
	}

	// rest server code dir
	static public function getRestServerodeDir() {
		if (isset(P2PMoneyAlphaDeployment::$restservercodedir))
			return P2PMoneyAlphaDeployment::$restservercodedir;
		else
			return P2PMoneyAlphaDeployment::getCodeDir().'/rest_server';
	}

	static public function setRestServerCodeDir($dir) {
		P2PMoneyAlphaDeployment::$restservercodedir = $dir;
	}

	// server code dir
	static public function getServerCodeDir() {
		if (isset(P2PMoneyAlphaDeployment::$servercodedir))
			return P2PMoneyAlphaDeployment::$servercodedir;
		else
			return P2PMoneyAlphaDeployment::getCodeDir().'/server';
	}

	static public function setServerCodeDir($dir) {
		P2PMoneyAlphaDeployment::$servercodedir = $dir;
	}

	// data dir (for settings files, logs, stored files,...)
	static public function initDataDirs() {
	
		global $p2pl_sites;
		global $p2pl_datadirs;
		global $p2pl_moduledatadirs;
		
		$datadir = null;
		
		if (!isset($p2pl_sites))
			return $datadir;
	
		// search for the key entry for the current site
		$site_key = null;
		
		$script_name = (isset($_SERVER['SCRIPT_NAME']) ? $_SERVER['SCRIPT_NAME'] : null);
		$script_filename = (isset($_SERVER['SCRIPT_FILENAME']) ? $_SERVER['SCRIPT_FILENAME'] : '');
	
		$http_post = (isset($_SERVER['HTTP_HOST']) ? $_SERVER['HTTP_HOST'] : '');
	
		$uri = explode('/', $script_name ? $script_name : $script_filename);
		$server = explode('.', implode('.', array_reverse(explode(':', rtrim($http_post, '.')))));
	
		for ($i = count($uri) - 1; $i > 0; $i--) {
			for ($j = count($server); $j > 0; $j--) {
				$dir = implode('.', array_slice($server, -$j)) . implode('.', array_slice($uri, 0, $i));
				//P2PMoneyAlphaGlobals::logString('dir is '.$dir);
	
				if (isset($p2pl_sites[$dir])) {
					$site_key = $p2pl_sites[$dir];
				}
			}
		}
		
		if (isset($site_key)) {
			// look if we have an entry for the main data directory
			if (isset($p2pl_datadirs) && (isset($p2pl_datadirs[$site_key]))) {
				$datadir = $p2pl_datadirs[$site_key];
				
				//P2PMoneyAlphaGlobals::logString('data dir is '.$datadir.' for key '.$site_key);
				
				if (isset($datadir) && file_exists($datadir))
					P2PMoneyAlphaDeployment::setDataDir($datadir);
		
			}
			
			// then we look if there are overloads for the module data directories
			if (isset($p2pl_moduledatadirs) && (isset($p2pl_moduledatadirs[$site_key]))) {
				//P2PMoneyAlphaGlobals::logString('module data directory overload for site key '.$site_key.' '.print_r($p2pl_moduledatadirs[$site_key], true));
				
				foreach($p2pl_moduledatadirs[$site_key] as $module => $dir) {
					//P2PMoneyAlphaGlobals::logString('data dir is '.$dir.' for site key '.$site_key.' and module '.$module);
					
					if (isset($dir) && file_exists($dir))
						P2PMoneyAlphaDeployment::setModuleDataDir($module, $dir);
				}
			}
			else {
				P2PMoneyAlphaGlobals::logString('no module data directory overload for site key '.$site_key);
			}
		}
	
		return $datadir;
	}
	
	static public function getDefaultDataDir() {
		return realpath(dirname(__FILE__)).'/../../../../../datadir/default/';
		
	}
	
	static public function getDataDir() {
		if (isset(P2PMoneyAlphaDeployment::$datadir))
			return P2PMoneyAlphaDeployment::$datadir;
		else
			//return realpath(dirname(__FILE__)).'/../../../../../'; // TODO: for the moment (2015.08.15), to change hard coded value as below
			return P2PMoneyAlphaDeployment::getDefaultDataDir();
	}

	static public function setDataDir($dir) {
		P2PMoneyAlphaGlobals::logString('setting main data directory to '.$dir);
		
		P2PMoneyAlphaDeployment::$datadir = $dir;
	}
	
	static public function getModuleDataDir($modulename) {
		if (isset(P2PMoneyAlphaDeployment::$modules_datadirs) && isset(P2PMoneyAlphaDeployment::$modules_datadirs[$modulename])) {
			// if we have stored a data dir for this module in our array, we return it
			return P2PMoneyAlphaDeployment::$modules_datadirs[$modulename];
		}
		else {
			// else we return the concatanation
			return P2PMoneyAlphaDeployment::getDataDir().'/'.$modulename.'/';
		}
	}
	
	static public function setModuleDataDir($modulename, $dir) {
		if (file_exists($dir)) {
			P2PMoneyAlphaGlobals::logString('setting module data directory for '.$modulename.' to '.$dir);
				
			P2PMoneyAlphaDeployment::$modules_datadirs[$modulename] = $dir;
		}
	}

}

// potential overload of default values
/*if (file_exists(realpath(dirname(__FILE__)).'/./settings/deployment-settings.inc')) {
	include_once(realpath(dirname(__FILE__)).'/./settings/deployment-settings.inc');
}*/

$currentdatadir = P2PMoneyAlphaDeployment::initDataDirs();




