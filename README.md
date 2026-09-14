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

; BOOL
debug_mode=true
enable_logging=false
ssl_enabled=true

; LREAL (64-bit Float)
cpu_threshold=85.5
memory_limit_gb=4.25
```

## Functions / function blocks

### MRC_Open

### MRC_ReadBOOL

### MRC_ReadDINT

### MRC_ReadLREAL

### MRC_ReadSTRING

### MRC_Close

### MRC_GetLastError

### MRC_KeyExists

### MRC_ReadContent

## Diagnosis

all read operations go to 'MRC_log' logger.


