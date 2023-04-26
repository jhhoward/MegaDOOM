const u16 framebufferx[] = {
0, 1, 2, 3, 512, 513, 514, 515, 1024, 1025, 1026, 1027, 1536, 1537, 1538, 1539, 2048, 2049, 2050, 2051, 2560, 2561, 2562, 2563, 3072, 3073, 3074, 3075, 3584, 3585, 3586, 3587, 4096, 4097, 4098, 4099, 4608, 4609, 4610, 4611, 5120, 5121, 5122, 5123, 5632, 5633, 5634, 5635, 6144, 6145, 6146, 6147, 6656, 6657, 6658, 6659, 7168, 7169, 7170, 7171, 7680, 7681, 7682, 7683, 8192, 8193, 8194, 8195, 8704, 8705, 8706, 8707, 9216, 9217, 9218, 9219, 9728, 9729, 9730, 9731, 10240, 10241, 10242, 10243, 10752, 10753, 10754, 10755, 11264, 11265, 11266, 11267, 11776, 11777, 11778, 11779, 12288, 12289, 12290, 12291, 12800, 12801, 12802, 12803, 13312, 13313, 13314, 13315, 13824, 13825, 13826, 13827, };

void VLine(int x, int y, int count, uint8_t colour)
{
	u8* ptr = framebuffer + framebufferx[x];
	ptr += (y << 2);
	switch(count) {	case 128:
	ptr[508] = colour;
	case 127:
	ptr[504] = colour;
	case 126:
	ptr[500] = colour;
	case 125:
	ptr[496] = colour;
	case 124:
	ptr[492] = colour;
	case 123:
	ptr[488] = colour;
	case 122:
	ptr[484] = colour;
	case 121:
	ptr[480] = colour;
	case 120:
	ptr[476] = colour;
	case 119:
	ptr[472] = colour;
	case 118:
	ptr[468] = colour;
	case 117:
	ptr[464] = colour;
	case 116:
	ptr[460] = colour;
	case 115:
	ptr[456] = colour;
	case 114:
	ptr[452] = colour;
	case 113:
	ptr[448] = colour;
	case 112:
	ptr[444] = colour;
	case 111:
	ptr[440] = colour;
	case 110:
	ptr[436] = colour;
	case 109:
	ptr[432] = colour;
	case 108:
	ptr[428] = colour;
	case 107:
	ptr[424] = colour;
	case 106:
	ptr[420] = colour;
	case 105:
	ptr[416] = colour;
	case 104:
	ptr[412] = colour;
	case 103:
	ptr[408] = colour;
	case 102:
	ptr[404] = colour;
	case 101:
	ptr[400] = colour;
	case 100:
	ptr[396] = colour;
	case 99:
	ptr[392] = colour;
	case 98:
	ptr[388] = colour;
	case 97:
	ptr[384] = colour;
	case 96:
	ptr[380] = colour;
	case 95:
	ptr[376] = colour;
	case 94:
	ptr[372] = colour;
	case 93:
	ptr[368] = colour;
	case 92:
	ptr[364] = colour;
	case 91:
	ptr[360] = colour;
	case 90:
	ptr[356] = colour;
	case 89:
	ptr[352] = colour;
	case 88:
	ptr[348] = colour;
	case 87:
	ptr[344] = colour;
	case 86:
	ptr[340] = colour;
	case 85:
	ptr[336] = colour;
	case 84:
	ptr[332] = colour;
	case 83:
	ptr[328] = colour;
	case 82:
	ptr[324] = colour;
	case 81:
	ptr[320] = colour;
	case 80:
	ptr[316] = colour;
	case 79:
	ptr[312] = colour;
	case 78:
	ptr[308] = colour;
	case 77:
	ptr[304] = colour;
	case 76:
	ptr[300] = colour;
	case 75:
	ptr[296] = colour;
	case 74:
	ptr[292] = colour;
	case 73:
	ptr[288] = colour;
	case 72:
	ptr[284] = colour;
	case 71:
	ptr[280] = colour;
	case 70:
	ptr[276] = colour;
	case 69:
	ptr[272] = colour;
	case 68:
	ptr[268] = colour;
	case 67:
	ptr[264] = colour;
	case 66:
	ptr[260] = colour;
	case 65:
	ptr[256] = colour;
	case 64:
	ptr[252] = colour;
	case 63:
	ptr[248] = colour;
	case 62:
	ptr[244] = colour;
	case 61:
	ptr[240] = colour;
	case 60:
	ptr[236] = colour;
	case 59:
	ptr[232] = colour;
	case 58:
	ptr[228] = colour;
	case 57:
	ptr[224] = colour;
	case 56:
	ptr[220] = colour;
	case 55:
	ptr[216] = colour;
	case 54:
	ptr[212] = colour;
	case 53:
	ptr[208] = colour;
	case 52:
	ptr[204] = colour;
	case 51:
	ptr[200] = colour;
	case 50:
	ptr[196] = colour;
	case 49:
	ptr[192] = colour;
	case 48:
	ptr[188] = colour;
	case 47:
	ptr[184] = colour;
	case 46:
	ptr[180] = colour;
	case 45:
	ptr[176] = colour;
	case 44:
	ptr[172] = colour;
	case 43:
	ptr[168] = colour;
	case 42:
	ptr[164] = colour;
	case 41:
	ptr[160] = colour;
	case 40:
	ptr[156] = colour;
	case 39:
	ptr[152] = colour;
	case 38:
	ptr[148] = colour;
	case 37:
	ptr[144] = colour;
	case 36:
	ptr[140] = colour;
	case 35:
	ptr[136] = colour;
	case 34:
	ptr[132] = colour;
	case 33:
	ptr[128] = colour;
	case 32:
	ptr[124] = colour;
	case 31:
	ptr[120] = colour;
	case 30:
	ptr[116] = colour;
	case 29:
	ptr[112] = colour;
	case 28:
	ptr[108] = colour;
	case 27:
	ptr[104] = colour;
	case 26:
	ptr[100] = colour;
	case 25:
	ptr[96] = colour;
	case 24:
	ptr[92] = colour;
	case 23:
	ptr[88] = colour;
	case 22:
	ptr[84] = colour;
	case 21:
	ptr[80] = colour;
	case 20:
	ptr[76] = colour;
	case 19:
	ptr[72] = colour;
	case 18:
	ptr[68] = colour;
	case 17:
	ptr[64] = colour;
	case 16:
	ptr[60] = colour;
	case 15:
	ptr[56] = colour;
	case 14:
	ptr[52] = colour;
	case 13:
	ptr[48] = colour;
	case 12:
	ptr[44] = colour;
	case 11:
	ptr[40] = colour;
	case 10:
	ptr[36] = colour;
	case 9:
	ptr[32] = colour;
	case 8:
	ptr[28] = colour;
	case 7:
	ptr[24] = colour;
	case 6:
	ptr[20] = colour;
	case 5:
	ptr[16] = colour;
	case 4:
	ptr[12] = colour;
	case 3:
	ptr[8] = colour;
	case 2:
	ptr[4] = colour;
	case 1:
	ptr[0] = colour;
	break;
	}
}

void R_DrawColumn(void)
{
	int count;
	byte texel;
	fixed_t frac;
	fixed_t fracstep;
	count = dc_yh - dc_yl;
	if (count < 0) return;
	u8* dest = framebuffer + framebufferx[dc_x];
	dest += (dc_yl << 2);
	fracstep = dc_iscale;
	frac = dc_texturemid + (dc_yl - centery) * fracstep;
	switch(count) {
	case 128:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 127:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 126:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 125:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 124:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 123:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 122:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 121:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 120:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 119:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 118:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 117:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 116:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 115:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 114:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 113:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 112:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 111:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 110:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 109:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 108:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 107:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 106:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 105:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 104:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 103:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 102:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 101:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 100:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 99:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 98:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 97:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 96:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 95:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 94:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 93:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 92:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 91:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 90:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 89:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 88:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 87:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 86:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 85:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 84:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 83:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 82:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 81:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 80:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 79:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 78:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 77:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 76:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 75:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 74:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 73:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 72:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 71:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 70:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 69:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 68:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 67:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 66:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 65:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 64:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 63:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 62:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 61:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 60:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 59:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 58:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 57:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 56:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 55:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 54:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 53:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 52:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 51:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 50:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 49:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 48:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 47:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 46:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 45:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 44:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 43:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 42:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 41:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 40:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 39:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 38:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 37:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 36:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 35:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 34:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 33:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 32:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 31:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 30:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 29:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 28:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 27:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 26:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 25:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 24:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 23:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 22:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 21:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 20:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 19:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 18:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 17:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 16:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 15:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 14:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 13:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 12:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 11:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 10:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 9:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 8:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 7:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 6:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 5:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 4:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 3:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 2:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 1:
		texel = dc_source[(frac >> FRACBITS) & 127];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	}
}

