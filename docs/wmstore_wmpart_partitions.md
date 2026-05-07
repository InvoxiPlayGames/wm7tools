# "wmstore"/"wmpart" partitions

The WMSTORE partition is usually the largest partition on the NAND of the
device at least on Lumia devices. It contains the OS kernel, loaders and
ROM image itself, as well as all user data.

The sector size is 0x200 / 512 bytes. All values are little endian unless
otherwise specified.

## WMSTORE structure

Total size: 0x200

| Offset | Type / Size   | Description                                           |
| ------ | ------------- | ----------------------------------------------------- |
| `0x0`  | char[8]       | Magic value (ascii '`_wmstore`')                      |
| `0x8`  | GUID (0x10)   | GUID (seen as `7ef3850b-a401-4699-2198-7a4c483f6baa`) |
| `0x18` | wchar_t[0x20] | WMSTORE Name (seen as `OS`)                           |
| `0x58` | uint32        | Unknown (seen as 0x20)                                |
| `0x5C` | uint32        | Unknown (seen as 0x0)                                 |
| `0x60` | uint32        | Unknown (seen as 0x1)                                 |
| `0x64` | uint64        | FILETIME timestamp of store creation                  |
| `0x6C` | uint32        | Unknown (seen as 0x0)                                 |
| `0x70` | byte[0x190]   | Padding                                               |

## WMPART structure

The WMSTORE structure is followed by a number of WMPART structures.

Total size: 0x200

| Offset | Type / Size   | Description                                            |
| ------ | ------------- | ------------------------------------------------------ |
| `0x0`  | char[8]       | Magic value (ascii '`_wmpart_`')                       |
| `0x8`  | wchar_t[0x20] | Partition Name (e.g. `BSP`, `SLDR1`, `SLDR2`, `IMGFS`) |
| `0x48` | uint32        | Unknown                                                |
| `0x4C` | uint32        | Sector offset to start of partition                    |
| `0x50` | uint32        | Unknown                                                |
| `0x54` | uint32        | Partition size in sector count                         |
| `0x58` | uint32        | Unknown                                                |
| `0x5C` | uint64        | FILETIME timestamp of partition creation               |
| `0x64` | uint32        | "Partition type"                                       |
| `0x68` | uint32        | Unknown                                                |
| `0x6C` | uint32        | Unknown, possibly flags?                               |
| `0x70` | byte[0x190]   | Padding                                                |
