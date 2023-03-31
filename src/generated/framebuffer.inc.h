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
void TexturedLine(const walltexture_t* texture, int16_t x, int16_t y, int16_t count, int16_t u, int16_t v, int16_t step)
{
	u8* ptr = framebuffer + framebufferx[x];
	ptr += (y << 2);
	const u8* texptr = texture->columns[u];
	int texcoord = v << 8;
	switch(count) {	case 128:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 127:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 126:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 125:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 124:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 123:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 122:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 121:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 120:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 119:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 118:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 117:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 116:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 115:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 114:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 113:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 112:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 111:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 110:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 109:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 108:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 107:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 106:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 105:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 104:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 103:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 102:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 101:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 100:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 99:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 98:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 97:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 96:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 95:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 94:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 93:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 92:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 91:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 90:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 89:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 88:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 87:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 86:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 85:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 84:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 83:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 82:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 81:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 80:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 79:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 78:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 77:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 76:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 75:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 74:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 73:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 72:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 71:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 70:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 69:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 68:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 67:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 66:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 65:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 64:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 63:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 62:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 61:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 60:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 59:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 58:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 57:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 56:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 55:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 54:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 53:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 52:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 51:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 50:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 49:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 48:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 47:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 46:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 45:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 44:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 43:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 42:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 41:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 40:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 39:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 38:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 37:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 36:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 35:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 34:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 33:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 32:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 31:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 30:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 29:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 28:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 27:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 26:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 25:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 24:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 23:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 22:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 21:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 20:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 19:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 18:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 17:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 16:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 15:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 14:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 13:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 12:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 11:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 10:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 9:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 8:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 7:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 6:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 5:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 4:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 3:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 2:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 1:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	break;
	}
}
