const u16 framebufferx[] = {
0, 1, 2, 3, 576, 577, 578, 579, 1152, 1153, 1154, 1155, 1728, 1729, 1730, 1731, 2304, 2305, 2306, 2307, 2880, 2881, 2882, 2883, 3456, 3457, 3458, 3459, 4032, 4033, 4034, 4035, 4608, 4609, 4610, 4611, 5184, 5185, 5186, 5187, 5760, 5761, 5762, 5763, 6336, 6337, 6338, 6339, 6912, 6913, 6914, 6915, 7488, 7489, 7490, 7491, 8064, 8065, 8066, 8067, 8640, 8641, 8642, 8643, 9216, 9217, 9218, 9219, 9792, 9793, 9794, 9795, 10368, 10369, 10370, 10371, 10944, 10945, 10946, 10947, 11520, 11521, 11522, 11523, 12096, 12097, 12098, 12099, 12672, 12673, 12674, 12675, 13248, 13249, 13250, 13251, 13824, 13825, 13826, 13827, 14400, 14401, 14402, 14403, };

void VLine(int x, int y, int count, uint8_t colour)
{
	u8* ptr = framebuffer + framebufferx[x];
	ptr += (y << 3);
	switch(count) {	case 72:
	ptr[568] = colour;
	ptr[572] = colour;
	case 71:
	ptr[560] = colour;
	ptr[564] = colour;
	case 70:
	ptr[552] = colour;
	ptr[556] = colour;
	case 69:
	ptr[544] = colour;
	ptr[548] = colour;
	case 68:
	ptr[536] = colour;
	ptr[540] = colour;
	case 67:
	ptr[528] = colour;
	ptr[532] = colour;
	case 66:
	ptr[520] = colour;
	ptr[524] = colour;
	case 65:
	ptr[512] = colour;
	ptr[516] = colour;
	case 64:
	ptr[504] = colour;
	ptr[508] = colour;
	case 63:
	ptr[496] = colour;
	ptr[500] = colour;
	case 62:
	ptr[488] = colour;
	ptr[492] = colour;
	case 61:
	ptr[480] = colour;
	ptr[484] = colour;
	case 60:
	ptr[472] = colour;
	ptr[476] = colour;
	case 59:
	ptr[464] = colour;
	ptr[468] = colour;
	case 58:
	ptr[456] = colour;
	ptr[460] = colour;
	case 57:
	ptr[448] = colour;
	ptr[452] = colour;
	case 56:
	ptr[440] = colour;
	ptr[444] = colour;
	case 55:
	ptr[432] = colour;
	ptr[436] = colour;
	case 54:
	ptr[424] = colour;
	ptr[428] = colour;
	case 53:
	ptr[416] = colour;
	ptr[420] = colour;
	case 52:
	ptr[408] = colour;
	ptr[412] = colour;
	case 51:
	ptr[400] = colour;
	ptr[404] = colour;
	case 50:
	ptr[392] = colour;
	ptr[396] = colour;
	case 49:
	ptr[384] = colour;
	ptr[388] = colour;
	case 48:
	ptr[376] = colour;
	ptr[380] = colour;
	case 47:
	ptr[368] = colour;
	ptr[372] = colour;
	case 46:
	ptr[360] = colour;
	ptr[364] = colour;
	case 45:
	ptr[352] = colour;
	ptr[356] = colour;
	case 44:
	ptr[344] = colour;
	ptr[348] = colour;
	case 43:
	ptr[336] = colour;
	ptr[340] = colour;
	case 42:
	ptr[328] = colour;
	ptr[332] = colour;
	case 41:
	ptr[320] = colour;
	ptr[324] = colour;
	case 40:
	ptr[312] = colour;
	ptr[316] = colour;
	case 39:
	ptr[304] = colour;
	ptr[308] = colour;
	case 38:
	ptr[296] = colour;
	ptr[300] = colour;
	case 37:
	ptr[288] = colour;
	ptr[292] = colour;
	case 36:
	ptr[280] = colour;
	ptr[284] = colour;
	case 35:
	ptr[272] = colour;
	ptr[276] = colour;
	case 34:
	ptr[264] = colour;
	ptr[268] = colour;
	case 33:
	ptr[256] = colour;
	ptr[260] = colour;
	case 32:
	ptr[248] = colour;
	ptr[252] = colour;
	case 31:
	ptr[240] = colour;
	ptr[244] = colour;
	case 30:
	ptr[232] = colour;
	ptr[236] = colour;
	case 29:
	ptr[224] = colour;
	ptr[228] = colour;
	case 28:
	ptr[216] = colour;
	ptr[220] = colour;
	case 27:
	ptr[208] = colour;
	ptr[212] = colour;
	case 26:
	ptr[200] = colour;
	ptr[204] = colour;
	case 25:
	ptr[192] = colour;
	ptr[196] = colour;
	case 24:
	ptr[184] = colour;
	ptr[188] = colour;
	case 23:
	ptr[176] = colour;
	ptr[180] = colour;
	case 22:
	ptr[168] = colour;
	ptr[172] = colour;
	case 21:
	ptr[160] = colour;
	ptr[164] = colour;
	case 20:
	ptr[152] = colour;
	ptr[156] = colour;
	case 19:
	ptr[144] = colour;
	ptr[148] = colour;
	case 18:
	ptr[136] = colour;
	ptr[140] = colour;
	case 17:
	ptr[128] = colour;
	ptr[132] = colour;
	case 16:
	ptr[120] = colour;
	ptr[124] = colour;
	case 15:
	ptr[112] = colour;
	ptr[116] = colour;
	case 14:
	ptr[104] = colour;
	ptr[108] = colour;
	case 13:
	ptr[96] = colour;
	ptr[100] = colour;
	case 12:
	ptr[88] = colour;
	ptr[92] = colour;
	case 11:
	ptr[80] = colour;
	ptr[84] = colour;
	case 10:
	ptr[72] = colour;
	ptr[76] = colour;
	case 9:
	ptr[64] = colour;
	ptr[68] = colour;
	case 8:
	ptr[56] = colour;
	ptr[60] = colour;
	case 7:
	ptr[48] = colour;
	ptr[52] = colour;
	case 6:
	ptr[40] = colour;
	ptr[44] = colour;
	case 5:
	ptr[32] = colour;
	ptr[36] = colour;
	case 4:
	ptr[24] = colour;
	ptr[28] = colour;
	case 3:
	ptr[16] = colour;
	ptr[20] = colour;
	case 2:
	ptr[8] = colour;
	ptr[12] = colour;
	case 1:
	ptr[0] = colour;
	ptr[4] = colour;
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
	dest += (dc_yl << 3);
	fracstep = dc_iscale;
	frac = dc_texturemid + (dc_yl - centery) * fracstep;
	switch(count) {
	case 72:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 71:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 70:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 69:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 68:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 67:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 66:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 65:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 64:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 63:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 62:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 61:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 60:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 59:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 58:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 57:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 56:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 55:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 54:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 53:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 52:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 51:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 50:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 49:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 48:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 47:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 46:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 45:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 44:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 43:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 42:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 41:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 40:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 39:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 38:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 37:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 36:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 35:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 34:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 33:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 32:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 31:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 30:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 29:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 28:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 27:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 26:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 25:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 24:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 23:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 22:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 21:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 20:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 19:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 18:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 17:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 16:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 15:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 14:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 13:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 12:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 11:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 10:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 9:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 8:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 7:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 6:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 5:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 4:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 3:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 2:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 1:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		*dest = texel;
		dest += 4;
		frac += fracstep;
	}
}

