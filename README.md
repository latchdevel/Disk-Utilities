# Disk-Utilities
Limited fork from https://github.com/keirf/Disk-Utilities collection for ripping, dumping, analysing, and modifying disk images.

## License
All code is free and unencumbered Public Domain software. For more information, please refer to http://unlicense.org/ or the accompanying UNLICENSE file.

## Includes
* **libdisk** 
A library for converting and manipulating disk images. It can create disk images in a range of formats from Kryoflux STREAM and SPS/IPF images (among others), and then allow these to be accessed and modified.
Dynamic and static builds for Windows, Linux and macOS.

* **disk-analyse**
The disk image conversion tool. 
Dynamic and static builds for Windows, Linux and macOS.
  * Read/Write support: 
    * SPS/IPF 
    * ADF, Extended ADF 
    * LibDisk (.DSK) 
    * Supercard Pro (.SCP) 
    * ImageDisk (.IMD) 
    * Sector Image (.IMG) 
    * HxC Floppy Emulator (.HFE) (orig,v3)
  * Read-Only support: 
    * Kryoflux STREAM 
    * DiscFerret (.DFI) 
    * Amiga diskread (.DAT) 
    * SPS/CTRaw

### Usage
```
$ disk-analyse -help
Usage: disk-analyse [options] in_file out_file
Options:
  -h, --help          Display this information
  -q, --quiet         Quiesce normal informational output
  -v, --verbose       Print extra diagnostic info
  -i, --index-align   Align all track starts near index mark
  -C, --clear-bad-sectors Clear bad sectors in output
  -p, --pll-period-adj=PCT (PCT=0..100) PLL period adjustment
  -P, --pll-phase-adj=PCT (PCT=0..100) PLL phase adjustment
                      Amount observed flux affects PLL
  -r, --rpm=DRIVE[:DATA] RPM of drive that created the input,
                         Original recording RPM of data [300]
  -D, --double-step   Double Step
  -s, --start-cyl=N   Start cylinder
  -e, --end-cyl=N     End cylinder
  -S, --ss[=0|1]      Single-sided disk (default is side 0)
  -k, --kryoflux-hack Fill empty tracks with prev track's data
  -f, --format=FORMAT Name of format descriptor in config file
  -c, --config=FILE   Config file to parse for format info
Supported file formats (suffix => type):
  .adf  => ADF
  .eadf => Extended-ADF
  .hfe  => HxC Floppy Emulator (orig,v3)
  .imd  => ImageDisk
  .img  => IBM-MFM Sector Dump
  .ipf  => SPS/IPF
  .dsk  => Libdisk
  .scp  => Supercard Pro
  .st   => Atari ST Sector Dump
Read-only support:
  .dat  => Diskread
  .dfi  => DiscFerret DFE2
  *.raw => Kryoflux STREAM
  .{ct,ctr,raw} => CAPS/SPS CT Raw
Write-only support:
  .jv3  => JV3 TRS80 Emulator
```

### Example
```
$ disk-analyse --config=./formats --format=ibm_mfm_dd double.scp double.img
T00.0-78.1: IBM-MFM DD (9 512-byte sectors, 4608 bytes)
T79.0-79.1: IBM-MFM DD (8 512-byte sectors, 4096 bytes)
T80.0-83.1: Unformatted
```

### Generic formats
```
# IBM-MFM Fully-Defined Formats (sector numbering/sizing/layout)

# 5.25-inch and 3.5-inch double density (360K and 720K)
ibm_pc_dd
    * ibm_pc_dd

ibm_pc_dd_10sec
    * ibm_pc_dd_10sec

# 3.5-inch high density (1440K)
ibm_pc_hd
    * ibm_pc_hd

# 5.25-inch high density (1200K)
ibm_pc_hd_5_25
    * ibm_pc_hd_5_25

# 3.5-inch extended density (2880K)
ibm_pc_ed
    * ibm_pc_ed

sega_system_24
    * sega_system_24

siemens_isdx_hd
    * siemens_isdx_hd

microsoft_dmf_hd
    160-161 trace_traceback_hd
    * microsoft_dmf_hd ibm_pc_hd

# Acorn ADFS 'S', 'M' or 'L' (160, 320 or 640K)
acorn_adfs_s_m_l
    * acorn_adfs_s_m_l

# Acorn ADFS 'D' or 'E' (800K)
acorn_adfs_d_e
    * acorn_adfs_d_e

# Acorn ADFS 'F' (1600K)
acorn_adfs_f
    * acorn_adfs_f

# Atari ST
atari_st
    * ibm_mfm_dd

##########################################################
# IBM-MFM Generic Formats (auto-detect sector arrangement)

ibm_mfm_dd
    * ibm_mfm_dd

ibm_mfm_hd
    * ibm_mfm_hd

ibm_mfm_ed
    * ibm_mfm_ed

##########################################################
# IBM-FM Generic Formats (auto-detect sector arrangement)

ibm_fm_sd
    * ibm_fm_sd

ibm_fm_dd
    * ibm_fm_dd

"DEC-RX01"
    * dec_rx01

"DEC-RX02"
    * dec_rx02

"DEC-RX01-525"
    * dec_rx01_525

"DEC-RX02-525"
    * dec_rx02_525

ibm
    * ibm_fm_sd ibm_fm_dd ibm_mfm_dd ibm_mfm_hd ibm_mfm_ed

ibm_fm_sd_recovery
    * ibm_fm_sd_recovery

ibm_mfm_dd_recovery
    * ibm_mfm_dd_recovery

ibm_recovery
    * ibm_fm_sd_recovery ibm_mfm_dd_recovery

##########################################################
# Raw / Unanalysed Formats

raw_sd
    * raw_sd

raw_dd
    * raw_dd

raw_hd
    * raw_hd

raw_ed
    * raw_ed

variable_raw_sd
    * raw_sd

variable_raw_dd
    * raw_dd

variable_raw_hd
    * raw_hd

variable_raw_ed
    * raw_ed
```