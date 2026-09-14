# MyReadCfg

# MyDiag

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Made For B&R](https://github.com/hilch/BandR-badges/blob/main/Made-For-BrAutomation.svg)](https://www.br-automation.com)

Helper Library to read '.ini' style machine configuration files during startup

## Dependencies / Preconditions

- ArEventLog
- FileIO
- AsBrStr
- sys_lib
- standard
- brsystem
- runtime

GCC >= V6.3.0

## INI file format

example:
```
[ApplicationSettings]
; STRING
app_name=MyApplication
database_host=192.168.1.100
log_file_path=/var/logs/app.log

; DINT (32-bit Integer)
max_connections=500
port_number=8080
timeout_seconds=30
; base 16
buffer_size=0x16

; BOOL
debug_mode=yes
enable_logging=false
ssl_enabled=TRUE
direction=1

; LREAL (64-bit Float)
cpu_threshold=85.5
memory_limit_gb=4.25
```

## Functions / function blocks

The following examples match the typical usage.
The functions may only be used in _INIT !

### MRC_Open

Open the configuration and return a handle.

```st
cfgIdent := MRC_Open(ADR(cfgDevice), ADR(cfgFileName));
```

Parameters:
- `cfgDevice`: pointer to the device name used by FileIO, for example `USER`.
- `cfgFileName`: pointer to the INI file path, for example `Simulation/50000.INI`.
- Return value: configuration handle; `0` means the file could not be opened.

### MRC_ReadBOOL

Read a boolean value with a fallback value if the key does not exist or is invalid.

```st
gMachineCfg.debug_mode := MRC_ReadBOOL(cfgIdent, ADR('debug_mode'), FALSE);
gMachineCfg.enable_logging := MRC_ReadBOOL(cfgIdent, ADR('enable_logging'), FALSE);
gMachineCfg.ssl_enabled := MRC_ReadBOOL(cfgIdent, ADR('ssl_enabled'), FALSE);
gMachineCfg.direction := MRC_ReadBOOL(cfgIdent, ADR('direction'), FALSE);
```

Parameters:
- `cfgIdent`: handle returned by `MRC_Open`.
- `pKey`: pointer to the key name, for example `ADR('debug_mode')`.
- `Default`: value returned if the key is missing or cannot be converted.
- Return value: the value as `BOOL`.

### MRC_ReadDINT

Read an integer value.

```st
gMachineCfg.max_connections := MRC_ReadDINT(cfgIdent, ADR('max_connections'), 0);
gMachineCfg.port_number := MRC_ReadDINT(cfgIdent, ADR('port_number'), 0);
gMachineCfg.timeout_seconds := MRC_ReadDINT(cfgIdent, ADR('timeout_seconds'), 0);
gMachineCfg.buffer_size := MRC_ReadDINT(cfgIdent, ADR('buffer_size'), 0);
```

Parameters:
- `cfgIdent`: handle returned by `MRC_Open`.
- `pKey`: pointer to the key name.
- `Default`: default `DINT` value if the key is missing or invalid.
- Return value: the value as `DINT`.

### MRC_ReadLREAL

Read a floating-point value.

```st
gMachineCfg.cpu_threshold := MRC_ReadLREAL(cfgIdent, ADR('cpu_threshold'), 0.0);
gMachineCfg.memory_limit_gb := MRC_ReadLREAL(cfgIdent, ADR('memory_limit_gb'), 0.0);
```

Parameters:
- `cfgIdent`: handle returned by `MRC_Open`.
- `pKey`: pointer to the key name.
- `Default`: default `LREAL` value if the key is missing or invalid.
- Return value: the value as `LREAL`.

### MRC_ReadSTRING

Read a string value into a fixed-size buffer.

```st
MRC_ReadSTRING(cfgIdent, ADR('app_name'), ADR(gMachineCfg.app_name), SIZEOF(gMachineCfg.app_name), ADR('<unknown>'));
MRC_ReadSTRING(cfgIdent, ADR('database_host'), ADR(gMachineCfg.database_host), SIZEOF(gMachineCfg.database_host), ADR('0.0.0.0'));
MRC_ReadSTRING(cfgIdent, ADR('log_file_path'), ADR(gMachineCfg.log_file_path), SIZEOF(gMachineCfg.log_file_path), ADR('\'));
```

Parameters:
- `cfgIdent`: handle returned by `MRC_Open`.
- `pKey`: pointer to the key name.
- `pValue`: pointer to the destination buffer, usually a `STRING` or array variable.
- `MaxSize`: size of the destination buffer including the terminating zero.
- `pDefault`: pointer to the fallback string if the key is missing.
- Return value: number of copied characters or a negative error code.

### MRC_Close

Close the configuration handle when it is no longer needed.

```st
MRC_Close(cfgIdent);
cfgIdent := 0;
```

Parameters:
- `cfgIdent`: handle returned by `MRC_Open`.
- Return value: `0` for success; otherwise an error code is returned.

### MRC_GetLastError

Get the last error code after a failed read or when a key does not exist.

```st
last_error := MRC_GetLastError(cfgIdent); (* returns MRC_ERR_KEY_NOT_FOUND *)
```

Parameters:
- `cfgIdent`: handle returned by `MRC_Open`.
- Return value: the last error code for that configuration handle.

### MRC_KeyExists

Check whether a key is present in the configuration.

```st
key_exist := MRC_KeyExists(cfgIdent, ADR('app_name'));
```

Parameters:
- `cfgIdent`: handle returned by `MRC_Open`.
- `pKey`: pointer to the key name.
- Return value: `TRUE` if the key exists, otherwise `FALSE`.

### MRC_ReadContent

Read the complete file content into a buffer.

```st
MRC_ReadContent(cfgIdent, ADR(content), SIZEOF(content));
```

Parameters:
- `cfgIdent`: handle returned by `MRC_Open`.
- `pValue`: pointer to the destination buffer for the file content.
- `MaxSize`: maximum size of the destination buffer including the terminating zero.
- Return value: number of copied characters or a negative error code.

## Diagnosis

All read operations go to the 'MRC_log' logger.
![logger.png](https://github.com/hilch/MyReadCfg/blob/main/doc/logger.png)


