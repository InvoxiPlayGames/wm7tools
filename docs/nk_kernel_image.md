# "NK" Kernel Image

The Windows CE kernel image is stored in the "NK" partition (type 0x23). It
contains the raw CE kernel as well as drivers, boot-related files and metadata.

## "CECE" Header Structure

Total size: 0x1000

| Offset | Type / Size   | Description                                             |
| ------ | ------------- | ------------------------------------------------------- |
| `0x0`  | byte[0x40]    | Boot code. Contains a single ARM jump to entrypoint.    |
| `0x40` | char[4]       | Magic value (`ECEC`, `CECE` in little endian)           |
| `0x44` | uint32        | Virtual memory address of the ROM header structure.     |
| `0x48` | uint32        | Offset of the ROM header structure in the NK partition. |
| `0x4C` | byte[0xFB4]   | Padding                                                 |

## ROM Header Structure

Total size: 0x54

TODO: Document. Contained in `common/wmnk.h` as well as in several CE sources.

## EXE/DLL TOC Structure

Total size: 0x1C

TODO: Document. Contained in `common/wmnk.h` as well as in several CE sources.
