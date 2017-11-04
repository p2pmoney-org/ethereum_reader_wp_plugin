<?php
namespace p2pmoney\common\core\model;

class coreSiteLogger {
	
	private $logFilesDir;
	private $logFileRootName;
	
	public $bLogEnabled;
	
	public function isLoggingEnabled() {
		return $this->bLogEnabled;
	}
	
	public function enableLogging() {
		$this->bLogEnabled = true;
	}
	
	public function disableLogging() {
		$this->bLogEnabled = false;
	}
	
	public function setEnableLogging($choice) {
		$this->bLogEnabled = ($choice === true ? true : false);
	}
	
	public function logString($log) {
		if (!$this->bLogEnabled)
			return;

		$string = date("c", time()).": ".$log;
		$filepath = $this->getLogFilePath();
		
		if (file_exists($filepath))
		$file = fopen($filepath, 'a');
		else
		$file = fopen($filepath, 'a+');
		
		if ($file === false)
			return;
		
		fwrite($file, $string . "\n");
		
		fclose($file);
	}
	
	public function getLogFileName() {
		$now = time();
		
		$year = date("Y", $now);
		$month = date("m", $now);
		$day = date("d", $now);
		$hour = date("H", $now);
		$minute = date("i", $now);
		
		$filename = $this->logFileRootName
					.".".$year
					.".".$month
					.".".$day
					.".log";
					
		return $filename;
	}
	
	public function getLogFilesDir() {
		$dirname = $this->logFilesDir;
		
		// test if directory exists
		if (!file_exists($dirname)) {
			// we create it
			mkdir($dirname);
		}
		
		return $dirname;
	}
	
	public function setLogFilesDir($logFilesDir) {
		$this->logFilesDir = $logFilesDir;
	}
	
	public function setLogFileRootName($logFileRootName) {
		$this->logFileRootName = $logFileRootName;
	}
	
	public function getLogFilePath() {
		return $this->getLogFilesDir().$this->getLogFileName();
	}
	
	public function tailLogFile($nlines) {
		$lines = array();
		
		
		$filepath = $this->getLogFilePath();
		
		if (file_exists($filepath)) {
			$file = file($filepath);
		
			$count = count($file);
			$nlines = ($nlines < $count ? $nlines : $count);
			
			$j = 0;
			for ($i = $count-$nlines; $i < $count; $i++) {
				$lines[$j] = $file[$i];
				$j++;
			}
		}
		
		return $lines;
	}
}