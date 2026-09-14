
PROGRAM _INIT
	cfgIdent := MRC_Open(ADR(cfgDevice), ADR(cfgFileName));
	
	(* read STRING *)
	MRC_ReadSTRING( cfgIdent, ADR('app_name'), ADR(gMachineCfg.app_name), SIZEOF(gMachineCfg.app_name), ADR('<unknown>') );
	MRC_ReadSTRING( cfgIdent, ADR('database_host'), ADR(gMachineCfg.database_host), SIZEOF(gMachineCfg.database_host), ADR('0.0.0.0') );
	MRC_ReadSTRING( cfgIdent, ADR('log_file_path'), ADR(gMachineCfg.log_file_path), SIZEOF(gMachineCfg.log_file_path), ADR('\') );	
	
	(* read DINT *)
	gMachineCfg.max_connections := MRC_ReadDINT(cfgIdent, ADR('max_connections'), 0);
	gMachineCfg.port_number := MRC_ReadDINT(cfgIdent, ADR('port_number'), 0);
	gMachineCfg.timeout_seconds := MRC_ReadDINT(cfgIdent, ADR('timeout_seconds'), 0);
	
	(* read BOOL *)
	gMachineCfg.debug_mode := MRC_ReadBOOL(cfgIdent, ADR('debug_mode'), FALSE);
	gMachineCfg.enable_logging := MRC_ReadBOOL(cfgIdent, ADR('enable_logging'), FALSE);
	gMachineCfg.ssl_enabled := MRC_ReadBOOL(cfgIdent, ADR('ssl_enabled'), FALSE);	
	
	(* read LREAL *)
	gMachineCfg.cpu_threshold := MRC_ReadLREAL(cfgIdent, ADR('cpu_threshold'), 0.0);
	gMachineCfg.memory_limit_gb := MRC_ReadLREAL(cfgIdent, ADR('memory_limit_gb'), 0.0);	
	
	(* read complete file content *)
	MRC_ReadContent( cfgIdent, ADR(content), SIZEOF(content) );	
	
	MRC_Close(cfgIdent);
	cfgIdent := 0; 
	
	ST_tmp_suspend(0);
END_PROGRAM


PROGRAM _CYCLIC
	 
END_PROGRAM

PROGRAM _EXIT
	IF cfgIdent <> 0 THEN
		MRC_Close(cfgIdent);
		cfgIdent := 0;
	END_IF
END_PROGRAM

