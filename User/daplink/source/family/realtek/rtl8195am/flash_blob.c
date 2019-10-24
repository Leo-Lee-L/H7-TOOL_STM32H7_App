/* Flash OS Routines (Automagically Generated)
 * Copyright (c) 2009-2019 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

static const uint32_t rtl8195am_flash_prog_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x01f8e92d, 0x8400f8df, 0x46172200, 0x0604f1a8, 0x4cff4dfe, 0x0c04f108, 0x5330f247, 0xf8cc2000,
    0x20010000, 0x0000f8c8, 0x21002005, 0x1000f8cc, 0xf4216831, 0xf4412170, 0x60317140, 0x20017020,
    0x0000f8cc, 0x97002000, 0x1c406829, 0x0101f001, 0x42989100, 0x2900d801, 0x7820d1f6, 0xf8cc2100,
    0xf0101000, 0xd0060f01, 0x42931c52, 0xe8bdd8d6, 0x200001f8, 0xe8bd4770, 0x200101f8, 0xe92d4770,
    0xb0814df7, 0xffbcf7ff, 0x20004ee2, 0xf1a66030, 0x20010b04, 0x0000f8cb, 0x21002005, 0xf1ab6031,
    0x468a0704, 0xf4216839, 0xf4412170, 0x60397140, 0x835cf8df, 0x0000f888, 0x60302001, 0x20004cd3,
    0x5530f247, 0xa000f8cd, 0x1c406821, 0x0101f001, 0x42a89100, 0x2900d801, 0xf898d1f6, 0x21000000,
    0xf0106031, 0xd1420f02, 0x21002006, 0x4aca6031, 0x60112101, 0xf4216839, 0xf4217140, 0x60392170,
    0x0000f888, 0x60302001, 0xf8cd2000, 0x6821a000, 0xf0011c40, 0x91000101, 0xd80142a8, 0xd1f62900,
    0xff6ef7ff, 0x60302000, 0xf8cb2001, 0x20050000, 0x60312100, 0xf4216839, 0xf4412170, 0x60397140,
    0x0000f888, 0x60302001, 0xf8cd2000, 0x6821a000, 0xf0011c40, 0x91000101, 0xd80142a8, 0xd1f62900,
    0x0000f898, 0x60312100, 0x0f02f010, 0xe9ddd0bc, 0x23000102, 0x60339a01, 0xc29cf8df, 0xf8cc2301,
    0x683b3000, 0x7340f423, 0x2370f423, 0xf888603b, 0x22002000, 0xd9062800, 0xf8885c8b, 0x1c523000,
    0x4290b2d2, 0x2001d8f8, 0x20006030, 0xa000f8cd, 0x1c406821, 0x0101f001, 0x42a89100, 0x2900d801,
    0xf7ffd1f6, 0x2000ff1d, 0x20016030, 0x0000f8cb, 0x21002005, 0x68396031, 0x2170f421, 0x7140f441,
    0xf8886039, 0x20010000, 0x20006030, 0xa000f8cd, 0x1c406821, 0x0101f001, 0x42a89100, 0x2900d801,
    0xf898d1f6, 0x21000000, 0xf0106031, 0xd1da0f02, 0xe8bdb004, 0x4a818df0, 0x2000b081, 0x91006811,
    0xd0042921, 0x60112121, 0x28141c40, 0x487cdbf6, 0xf0416801, 0x60010101, 0x680b497a, 0x0310f043,
    0x4979600b, 0xf443680b, 0x600b7380, 0xf443680b, 0x600b7300, 0x68194b75, 0x0120f041, 0x68016019,
    0x0106f021, 0x68106001, 0x1002f3c0, 0xbf282803, 0x4a6f2003, 0xf1a2444a, 0xf8120c04, 0xeb021020,
    0xf88c0080, 0x78421000, 0x2001f88c, 0xf88c7880, 0x48680002, 0xf0106800, 0xd0160f01, 0x68004866,
    0x0f20f010, 0x4865d011, 0xf0136803, 0xd00c7f80, 0xf3c06800, 0xf0004007, 0xeb03031f, 0xf88c0343,
    0xeb023002, 0xf88c1050, 0x23010001, 0xf88c4a51, 0x20003003, 0x485a6010, 0x1f006001, 0x49596003,
    0x90006808, 0x3001f89c, 0x000ff36f, 0x60084318, 0x68084955, 0xf89c9000, 0xf0203002, 0x431800ff,
    0x20046008, 0x60081f11, 0x1f0b2000, 0xf4216819, 0x9100117c, 0x60106019, 0x21384a4c, 0x4a4c6011,
    0x601121eb, 0x68114a4b, 0x71fff421, 0xb0016011, 0x20004770, 0x22004770, 0x0382eb00, 0x43d0f1a3,
    0xf841681b, 0x1c523022, 0x6f80f5b2, 0xf44fd3f4, 0x47705080, 0x45f8e92d, 0xf8df2400, 0x460e80b0,
    0x46a24607, 0x5530f247, 0x0084eb07, 0x40d0f1a0, 0x1024f856, 0x22006001, 0xa000f8cd, 0x0000f8d8,
    0xf0001c52, 0x90000001, 0xd80142aa, 0xd1f52800, 0xfe36f7ff, 0xf5b41c64, 0xd3e56f80, 0x5080f44f,
    0x85f8e8bd, 0x39080b01, 0xbf9c2902, 0x47702000, 0xf1a0b508, 0x0c0140d0, 0x1000f88d, 0xf88d0a01,
    0xf88d1001, 0x466a0002, 0x20202103, 0xfe57f7ff, 0xbd082000, 0x2700b5f8, 0x7000f88d, 0x7001f88d,
    0x4d1e4e1d, 0xf88d4c1e, 0x466a7002, 0x20202103, 0xfe45f7ff, 0xf88d2004, 0xf88d0000, 0xf88d7001,
    0x466a7002, 0xe02d2103, 0x40006004, 0x40006028, 0x40006060, 0x40006008, 0x40006118, 0x40000014,
    0x400002c0, 0x40000210, 0x40000230, 0x40000320, 0x00000008, 0x400001f4, 0x400000e0, 0x40000038,
    0x40006014, 0x4000611c, 0x40000300, 0x40006104, 0x400060f0, 0x40006120, 0x1fff8000, 0x1fff9000,
    0x1fffa000, 0xf7ff2020, 0xf44ffe0a, 0x46324300, 0xeb032000, 0xf1a10180, 0x680941d0, 0x1020f842,
    0xf5b01c40, 0xd3f46f80, 0x4310f44f, 0x2000462a, 0x0180eb03, 0x41d0f1a1, 0xf8426809, 0x1c401020,
    0x6f80f5b0, 0xf44fd3f4, 0x46214320, 0xeb032000, 0xf1a20280, 0x681242d0, 0x2020f841, 0xf5b01c40,
    0xd3f46f80, 0x46112200, 0xf7ff20c7, 0x4631fdd8, 0x4000f44f, 0xff3ef7ff, 0xf44f4629, 0xf7ff4010,
    0x4621ff39, 0x4020f44f, 0xff34f7ff, 0xbdf82000, 0x4ff8e92d, 0x000e4615, 0xf04f4607, 0xd01c0400,
    0x8078f8df, 0xf24746a3, 0x19385a30, 0x40d0f1a0, 0x60016829, 0xf8cd2000, 0xf8d8b000, 0x1c401000,
    0x0101f001, 0x45509100, 0x2900d801, 0xf7ffd1f5, 0x1d24fd67, 0x42b41d2d, 0x2000d3e7, 0x8ff8e8bd,
    0xf5a0b410, 0xf5b34300, 0xd9145f40, 0xebb32300, 0xd2100f91, 0x0c83eb00, 0x4cd0f1ac, 0xc000f8dc,
    0x4023f852, 0xd00245a4, 0x2001bc10, 0x1c5b4770, 0x0f91ebb3, 0xbc10d3ee, 0x47702000, 0x40006028,
    0x00000000, 0x00000000, 0x005e0101, 0x00000101, 0x00230201, 0x00050501
};

/**
* List of start and size for each size of flash sector
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address start + size.
*/
static const sector_info_t sectors_info[] = {
    {0, KB(4)},
};

static const program_target_t flash = {
    0x10007237, // Init
    0x10007353, // UnInit
    0x100073f5, // EraseChip
    0x100073c5, // EraseSector
    0x10007511, // ProgramPage

    0x10007561, // Verify

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x10007001,
        0x100075a0,
        0x10007800
    },

    0x10007000 + 0x00000A00,  // mem buffer location
    0x10007000,               // location to write prog_blob in target RAM
    sizeof(rtl8195am_flash_prog_blob),   // prog_blob size
    rtl8195am_flash_prog_blob,           // address of prog_blob
    0x00000400       // ram_to_flash_bytes_to_be_written
};
