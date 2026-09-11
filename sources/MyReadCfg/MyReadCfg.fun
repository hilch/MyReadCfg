
{REDUND_ERROR} FUNCTION MRC_Open : UDINT (*Reads a configuration file into memory, returns Ident (0 = error)*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pDeviceName : UDINT; (*char* : FileIO device name, e.g. "USER"*)
		pFileName : UDINT; (*char* : file name incl. relative path, e.g. "Simulation/50000.INI"*)
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_Close : DINT (*Closes a configuration and frees all resources. 0 = OK*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT;
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_GetLastError : DINT (*Error of the last MyReadCfg call of this Ident (0 = Ident independent / open error)*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT;
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_KeyExists : BOOL (*TRUE if the key is present*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT;
		pKey : UDINT; (*char* : name of the constant*)
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_ReadBOOL : BOOL (*Reads a value as BOOL*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT;
		pKey : UDINT; (*char* : name of the constant*)
		Default : BOOL; (*returned if key is missing or not convertible*)
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_ReadDINT : DINT (*Reads a value as DINT*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT;
		pKey : UDINT; (*char* : name of the constant*)
		Default : DINT; (*returned if key is missing or not convertible*)
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_ReadLREAL : LREAL (*Reads a value as LREAL*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT;
		pKey : UDINT; (*char* : name of the constant*)
		Default : LREAL; (*returned if key is missing or not convertible*)
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_ReadSTRING : DINT (*Reads a value as STRING. Returns copied length or negative error*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT;
		pKey : UDINT; (*char* : name of the constant*)
		pValue : UDINT; (*char* : destination buffer*)
		MaxSize : UDINT; (*size of destination buffer incl. terminating zero*)
		pDefault : UDINT; (*char* : default string, may be 0*)
	END_VAR
END_FUNCTION
