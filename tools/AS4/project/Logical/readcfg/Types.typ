
TYPE
	MachineConfiguration_typ : 	STRUCT 
		app_name : STRING[80];
		database_host : STRING[80];
		log_file_path : STRING[80];
		max_connections : DINT;
		port_number : DINT;
		timeout_seconds : DINT;
		buffer_size : DINT;
		debug_mode : BOOL;
		enable_logging : BOOL;
		ssl_enabled : BOOL;
		direction : BOOL;
		cpu_threshold : LREAL;
		memory_limit_gb : LREAL;
	END_STRUCT;
END_TYPE
