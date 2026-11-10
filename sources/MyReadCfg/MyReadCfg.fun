
{REDUND_ERROR} FUNCTION MRC_Open : UDINT (*Reads a configuration file into memory, returns Ident (0 = error)*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pDeviceName : UDINT; (*Pointer to FileIO device name, e.g. 'USER' (STRING)*)
		pFileName : UDINT; (*Pointer to file name incl. relative path, e.g. 'Simulation/50000.INI' (STRING)*)
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_Close : DINT (*Closes a configuration and frees all resources. 0 = OK*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT; (*Ident from MRC_Open*)
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_GetLastError : DINT (*Error of the last MyReadCfg call of this Ident (0 = Ident independent / open error)*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT; (*Ident from MRC_Open*)
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_KeyExists : BOOL (*TRUE if the key is present*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT; (*Ident from MRC_Open*)
		pKey : UDINT; (*pointer to name of the constant (STRING)*)
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_ReadBOOL : BOOL (*Reads a value as BOOL*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT; (*Ident from MRC_Open*)
		pKey : UDINT; (*Pointer to name of the constant (STRING)*)
		Default : BOOL; (*returned if key is missing or not convertible*)
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_ReadDINT : DINT (*Reads a value as DINT*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT; (*Ident from MRC_Open*)
		pKey : UDINT; (*Pointer to name of the constant (STRING)*)
		Default : DINT; (*returned if key is missing or not convertible*)
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_ReadLREAL : LREAL (*Reads a value as LREAL*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT; (*Ident from MRC_Open*)
		pKey : UDINT; (*Pointer to name of the constant (STRING)*)
		Default : LREAL; (*returned if key is missing or not convertible*)
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_ReadSTRING : DINT (*Reads a value as STRING. Returns copied length or negative error*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT; (*Ident from MRC_Open*)
		pKey : UDINT; (*Pointer to name of the constant (STRING)*)
		pValue : UDINT; (*Pointer to destination buffer (STRING)*)
		MaxSize : UDINT; (*size of destination buffer incl. terminating zero (SIZEOF(STRING))*)
		pDefault : UDINT; (*Pointer to default value, may be 0 (STRING)*)
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION MRC_ReadContent : DINT (*Reads the complete file content into a STRING variable. Returns copied length or negative error*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Ident : UDINT; (*Ident from MRC_Open*)
		pValue : UDINT; (*Pointer to destination buffer (STRING)*)
		MaxSize : UDINT; (*size of destination buffer incl. terminating zero (SIZEOF(STRING))*)
	END_VAR
END_FUNCTION
