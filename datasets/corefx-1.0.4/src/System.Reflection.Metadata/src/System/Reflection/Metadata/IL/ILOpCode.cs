// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

namespace System.Reflection.Metadata
{
    internal enum ILOpCode : ushort
    {
        Nop = 0x00,
        Break = 0x01,
        Ldarg_0 = 0x02,
        Ldarg_1 = 0x03,
        Ldarg_2 = 0x04,
        Ldarg_3 = 0x05,
        Ldloc_0 = 0x06,
        Ldloc_1 = 0x07,
        Ldloc_2 = 0x08,
        Ldloc_3 = 0x09,
        Stloc_0 = 0x0a,
        Stloc_1 = 0x0b,
        Stloc_2 = 0x0c,
        Stloc_3 = 0x0d,
        Ldarg_s = 0x0e,
        Ldarga_s = 0x0f,
        Starg_s = 0x10,
        Ldloc_s = 0x11,
        Ldloca_s = 0x12,
        Stloc_s = 0x13,
        Ldnull = 0x14,
        Ldc_i4_m1 = 0x15,
        Ldc_i4_0 = 0x16,
        Ldc_i4_1 = 0x17,
        Ldc_i4_2 = 0x18,
        Ldc_i4_3 = 0x19,
        Ldc_i4_4 = 0x1a,
        Ldc_i4_5 = 0x1b,
        Ldc_i4_6 = 0x1c,
        Ldc_i4_7 = 0x1d,
        Ldc_i4_8 = 0x1e,
        Ldc_i4_s = 0x1f,
        Ldc_i4 = 0x20,
        Ldc_i8 = 0x21,
        Ldc_r4 = 0x22,
        Ldc_r8 = 0x23,
        Dup = 0x25,
        Pop = 0x26,
        Jmp = 0x27,
        Call = 0x28,
        Calli = 0x29,
        Ret = 0x2a,
        Br_s = 0x2b,
        Brfalse_s = 0x2c,
        Brtrue_s = 0x2d,
        Beq_s = 0x2e,
        Bge_s = 0x2f,
        Bgt_s = 0x30,
        Ble_s = 0x31,
        Blt_s = 0x32,
        Bne_un_s = 0x33,
        Bge_un_s = 0x34,
        Bgt_un_s = 0x35,
        Ble_un_s = 0x36,
        Blt_un_s = 0x37,
        Br = 0x38,
        Brfalse = 0x39,
        Brtrue = 0x3a,
        Beq = 0x3b,
        Bge = 0x3c,
        Bgt = 0x3d,
        Ble = 0x3e,
        Blt = 0x3f,
        Bne_un = 0x40,
        Bge_un = 0x41,
        Bgt_un = 0x42,
        Ble_un = 0x43,
        Blt_un = 0x44,
        Switch = 0x45,
        Ldind_i1 = 0x46,
        Ldind_u1 = 0x47,
        Ldind_i2 = 0x48,
        Ldind_u2 = 0x49,
        Ldind_i4 = 0x4a,
        Ldind_u4 = 0x4b,
        Ldind_i8 = 0x4c,
        Ldind_i = 0x4d,
        Ldind_r4 = 0x4e,
        Ldind_r8 = 0x4f,
        Ldind_ref = 0x50,
        Stind_ref = 0x51,
        Stind_i1 = 0x52,
        Stind_i2 = 0x53,
        Stind_i4 = 0x54,
        Stind_i8 = 0x55,
        Stind_r4 = 0x56,
        Stind_r8 = 0x57,
        Add = 0x58,
        Sub = 0x59,
        Mul = 0x5a,
        Div = 0x5b,
        Div_un = 0x5c,
        Rem = 0x5d,
        Rem_un = 0x5e,
        And = 0x5f,
        Or = 0x60,
        Xor = 0x61,
        Shl = 0x62,
        Shr = 0x63,
        Shr_un = 0x64,
        Neg = 0x65,
        Not = 0x66,
        Conv_i1 = 0x67,
        Conv_i2 = 0x68,
        Conv_i4 = 0x69,
        Conv_i8 = 0x6a,
        Conv_r4 = 0x6b,
        Conv_r8 = 0x6c,
        Conv_u4 = 0x6d,
        Conv_u8 = 0x6e,
        Callvirt = 0x6f,
        Cpobj = 0x70,
        Ldobj = 0x71,
        Ldstr = 0x72,
        Newobj = 0x73,
        Castclass = 0x74,
        Isinst = 0x75,
        Conv_r_un = 0x76,
        Unbox = 0x79,
        Throw = 0x7a,
        Ldfld = 0x7b,
        Ldflda = 0x7c,
        Stfld = 0x7d,
        Ldsfld = 0x7e,
        Ldsflda = 0x7f,
        Stsfld = 0x80,
        Stobj = 0x81,
        Conv_ovf_i1_un = 0x82,
        Conv_ovf_i2_un = 0x83,
        Conv_ovf_i4_un = 0x84,
        Conv_ovf_i8_un = 0x85,
        Conv_ovf_u1_un = 0x86,
        Conv_ovf_u2_un = 0x87,
        Conv_ovf_u4_un = 0x88,
        Conv_ovf_u8_un = 0x89,
        Conv_ovf_i_un = 0x8a,
        Conv_ovf_u_un = 0x8b,
        Box = 0x8c,
        Newarr = 0x8d,
        Ldlen = 0x8e,
        Ldelema = 0x8f,
        Ldelem_i1 = 0x90,
        Ldelem_u1 = 0x91,
        Ldelem_i2 = 0x92,
        Ldelem_u2 = 0x93,
        Ldelem_i4 = 0x94,
        Ldelem_u4 = 0x95,
        Ldelem_i8 = 0x96,
        Ldelem_i = 0x97,
        Ldelem_r4 = 0x98,
        Ldelem_r8 = 0x99,
        Ldelem_ref = 0x9a,
        Stelem_i = 0x9b,
        Stelem_i1 = 0x9c,
        Stelem_i2 = 0x9d,
        Stelem_i4 = 0x9e,
        Stelem_i8 = 0x9f,
        Stelem_r4 = 0xa0,
        Stelem_r8 = 0xa1,
        Stelem_ref = 0xa2,
        Ldelem = 0xa3,
        Stelem = 0xa4,
        Unbox_any = 0xa5,
        Conv_ovf_i1 = 0xb3,
        Conv_ovf_u1 = 0xb4,
        Conv_ovf_i2 = 0xb5,
        Conv_ovf_u2 = 0xb6,
        Conv_ovf_i4 = 0xb7,
        Conv_ovf_u4 = 0xb8,
        Conv_ovf_i8 = 0xb9,
        Conv_ovf_u8 = 0xba,
        Refanyval = 0xc2,
        Ckfinite = 0xc3,
        Mkrefany = 0xc6,
        Ldtoken = 0xd0,
        Conv_u2 = 0xd1,
        Conv_u1 = 0xd2,
        Conv_i = 0xd3,
        Conv_ovf_i = 0xd4,
        Conv_ovf_u = 0xd5,
        Add_ovf = 0xd6,
        Add_ovf_un = 0xd7,
        Mul_ovf = 0xd8,
        Mul_ovf_un = 0xd9,
        Sub_ovf = 0xda,
        Sub_ovf_un = 0xdb,
        Endfinally = 0xdc,
        Leave = 0xdd,
        Leave_s = 0xde,
        Stind_i = 0xdf,
        Conv_u = 0xe0,
        Arglist = 0xfe00,
        Ceq = 0xfe01,
        Cgt = 0xfe02,
        Cgt_un = 0xfe03,
        Clt = 0xfe04,
        Clt_un = 0xfe05,
        Ldftn = 0xfe06,
        Ldvirtftn = 0xfe07,
        Ldarg = 0xfe09,
        Ldarga = 0xfe0a,
        Starg = 0xfe0b,
        Ldloc = 0xfe0c,
        Ldloca = 0xfe0d,
        Stloc = 0xfe0e,
        Localloc = 0xfe0f,
        Endfilter = 0xfe11,
        Unaligned = 0xfe12,
        Volatile = 0xfe13,
        Tail = 0xfe14,
        Initobj = 0xfe15,
        Constrained = 0xfe16,
        Cpblk = 0xfe17,
        Initblk = 0xfe18,
        Rethrow = 0xfe1a,
        Sizeof = 0xfe1c,
        Refanytype = 0xfe1d,
        Readonly = 0xfe1e,
    }
}
