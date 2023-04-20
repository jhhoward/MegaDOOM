const u16 framebufferx[] = {
0, 1, 2, 3, 576, 577, 578, 579, 1152, 1153, 1154, 1155, 1728, 1729, 1730, 1731, 2304, 2305, 2306, 2307, 2880, 2881, 2882, 2883, 3456, 3457, 3458, 3459, 4032, 4033, 4034, 4035, 4608, 4609, 4610, 4611, 5184, 5185, 5186, 5187, 5760, 5761, 5762, 5763, 6336, 6337, 6338, 6339, 6912, 6913, 6914, 6915, 7488, 7489, 7490, 7491, 8064, 8065, 8066, 8067, 8640, 8641, 8642, 8643, 9216, 9217, 9218, 9219, 9792, 9793, 9794, 9795, 10368, 10369, 10370, 10371, 10944, 10945, 10946, 10947, 11520, 11521, 11522, 11523, 12096, 12097, 12098, 12099, 12672, 12673, 12674, 12675, 13248, 13249, 13250, 13251, 13824, 13825, 13826, 13827, 14400, 14401, 14402, 14403, };

const u16 screenLayoutTiles[] = {
16, 16, 16, 17, 35, 53, 71, 89, 107, 125, 143, 161, 179, 197, 215, 233, 251, 269, 287, 305, 323, 341, 359, 377, 395, 413, 431, 449, 467, 16, 16, 16, 16, 16, 16, 18, 36, 54, 72, 90, 108, 126, 144, 162, 180, 198, 216, 234, 252, 270, 288, 306, 324, 342, 360, 378, 396, 414, 432, 450, 468, 16, 16, 16, 16, 16, 16, 19, 37, 55, 73, 91, 109, 127, 145, 163, 181, 199, 217, 235, 253, 271, 289, 307, 325, 343, 361, 379, 397, 415, 433, 451, 469, 16, 16, 16, 16, 16, 16, 20, 38, 56, 74, 92, 110, 128, 146, 164, 182, 200, 218, 236, 254, 272, 290, 308, 326, 344, 362, 380, 398, 416, 434, 452, 470, 16, 16, 16, 16, 16, 16, 21, 39, 57, 75, 93, 111, 129, 147, 165, 183, 201, 219, 237, 255, 273, 291, 309, 327, 345, 363, 381, 399, 417, 435, 453, 471, 16, 16, 16, 16, 16, 16, 22, 40, 58, 76, 94, 112, 130, 148, 166, 184, 202, 220, 238, 256, 274, 292, 310, 328, 346, 364, 382, 400, 418, 436, 454, 472, 16, 16, 16, 16, 16, 16, 23, 41, 59, 77, 95, 113, 131, 149, 167, 185, 203, 221, 239, 257, 275, 293, 311, 329, 347, 365, 383, 401, 419, 437, 455, 473, 16, 16, 16, 16, 16, 16, 24, 42, 60, 78, 96, 114, 132, 150, 168, 186, 204, 222, 240, 258, 276, 294, 312, 330, 348, 366, 384, 402, 420, 438, 456, 474, 16, 16, 16, 16, 16, 16, 25, 43, 61, 79, 97, 115, 133, 151, 169, 187, 205, 223, 241, 259, 277, 295, 313, 331, 349, 367, 385, 403, 421, 439, 457, 475, 16, 16, 16, 16, 16, 16, 26, 44, 62, 80, 98, 116, 134, 152, 170, 188, 206, 224, 242, 260, 278, 296, 314, 332, 350, 368, 386, 404, 422, 440, 458, 476, 16, 16, 16, 16, 16, 16, 27, 45, 63, 81, 99, 117, 135, 153, 171, 189, 207, 225, 243, 261, 279, 297, 315, 333, 351, 369, 387, 405, 423, 441, 459, 477, 16, 16, 16, 16, 16, 16, 28, 46, 64, 82, 100, 118, 136, 154, 172, 190, 208, 226, 244, 262, 280, 298, 316, 334, 352, 370, 388, 406, 424, 442, 460, 478, 16, 16, 16, 16, 16, 16, 29, 47, 65, 83, 101, 119, 137, 155, 173, 191, 209, 227, 245, 263, 281, 299, 317, 335, 353, 371, 389, 407, 425, 443, 461, 479, 16, 16, 16, 16, 16, 16, 30, 48, 66, 84, 102, 120, 138, 156, 174, 192, 210, 228, 246, 264, 282, 300, 318, 336, 354, 372, 390, 408, 426, 444, 462, 480, 16, 16, 16, 16, 16, 16, 31, 49, 67, 85, 103, 121, 139, 157, 175, 193, 211, 229, 247, 265, 283, 301, 319, 337, 355, 373, 391, 409, 427, 445, 463, 481, 16, 16, 16, 16, 16, 16, 32, 50, 68, 86, 104, 122, 140, 158, 176, 194, 212, 230, 248, 266, 284, 302, 320, 338, 356, 374, 392, 410, 428, 446, 464, 482, 16, 16, 16, 16, 16, 16, 33, 51, 69, 87, 105, 123, 141, 159, 177, 195, 213, 231, 249, 267, 285, 303, 321, 339, 357, 375, 393, 411, 429, 447, 465, 483, 16, 16, 16, 16, 16, 16, 34, 52, 70, 88, 106, 124, 142, 160, 178, 196, 214, 232, 250, 268, 286, 304, 322, 340, 358, 376, 394, 412, 430, 448, 466, 484, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 
};

const u16 skyLayoutTiles[] = {
8677, 8678, 8679, 8680, 8681, 8682, 8683, 8684, 8685, 8686, 8687, 8688, 8689, 8690, 8691, 8692, 8693, 8694, 8695, 8696, 8697, 8698, 8699, 8700, 8701, 8702, 8677, 8678, 8679, 8680, 8681, 8682, 8683, 8684, 8685, 8686, 8687, 8688, 8689, 8690, 8691, 8692, 8693, 8694, 8695, 8696, 8697, 8698, 8699, 8700, 8701, 8702, 8703, 8704, 8705, 8706, 8707, 8708, 8709, 8710, 8711, 8712, 8713, 8714, 8715, 8716, 8717, 8718, 8719, 8720, 8721, 8722, 8723, 8724, 8725, 8726, 8727, 8728, 8703, 8704, 8705, 8706, 8707, 8708, 8709, 8710, 8711, 8712, 8713, 8714, 8715, 8716, 8717, 8718, 8719, 8720, 8721, 8722, 8723, 8724, 8725, 8726, 8727, 8728, 8729, 8730, 8731, 8732, 8733, 8734, 8735, 8736, 8737, 8738, 8739, 8740, 8741, 8742, 8743, 8744, 8745, 8746, 8747, 8748, 8749, 8750, 8751, 8752, 8753, 8754, 8729, 8730, 8731, 8732, 8733, 8734, 8735, 8736, 8737, 8738, 8739, 8740, 8741, 8742, 8743, 8744, 8745, 8746, 8747, 8748, 8749, 8750, 8751, 8752, 8753, 8754, 8755, 8756, 8757, 8758, 8759, 8760, 8761, 8762, 8763, 8764, 8765, 8766, 8767, 8768, 8769, 8770, 8771, 8772, 8773, 8774, 8775, 8776, 8777, 8778, 8779, 8780, 8755, 8756, 8757, 8758, 8759, 8760, 8761, 8762, 8763, 8764, 8765, 8766, 8767, 8768, 8769, 8770, 8771, 8772, 8773, 8774, 8775, 8776, 8777, 8778, 8779, 8780, 8781, 8782, 8783, 8784, 8785, 8786, 8787, 8788, 8789, 8790, 8791, 8792, 8793, 8794, 8795, 8796, 8797, 8798, 8799, 8800, 8801, 8802, 8803, 8804, 8805, 8806, 8781, 8782, 8783, 8784, 8785, 8786, 8787, 8788, 8789, 8790, 8791, 8792, 8793, 8794, 8795, 8796, 8797, 8798, 8799, 8800, 8801, 8802, 8803, 8804, 8805, 8806, 8807, 8808, 8809, 8810, 8811, 8812, 8813, 8814, 8815, 8816, 8817, 8818, 8819, 8820, 8821, 8822, 8823, 8824, 8825, 8826, 8827, 8828, 8829, 8830, 8831, 8832, 8807, 8808, 8809, 8810, 8811, 8812, 8813, 8814, 8815, 8816, 8817, 8818, 8819, 8820, 8821, 8822, 8823, 8824, 8825, 8826, 8827, 8828, 8829, 8830, 8831, 8832, 8833, 8834, 8835, 8836, 8837, 8838, 8839, 8840, 8841, 8842, 8843, 8844, 8845, 8846, 8847, 8848, 8849, 8850, 8851, 8852, 8853, 8854, 8855, 8856, 8857, 8858, 8833, 8834, 8835, 8836, 8837, 8838, 8839, 8840, 8841, 8842, 8843, 8844, 8845, 8846, 8847, 8848, 8849, 8850, 8851, 8852, 8853, 8854, 8855, 8856, 8857, 8858, 8859, 8860, 8861, 8862, 8863, 8864, 8865, 8866, 8867, 8868, 8869, 8870, 8871, 8872, 8873, 8874, 8875, 8876, 8877, 8878, 8879, 8880, 8881, 8882, 8883, 8884, 8859, 8860, 8861, 8862, 8863, 8864, 8865, 8866, 8867, 8868, 8869, 8870, 8871, 8872, 8873, 8874, 8875, 8876, 8877, 8878, 8879, 8880, 8881, 8882, 8883, 8884, 8885, 8886, 8887, 8888, 8889, 8890, 8891, 8892, 8893, 8894, 8895, 8896, 8897, 8898, 8899, 8900, 8901, 8902, 8903, 8904, 8905, 8906, 8907, 8908, 8909, 8910, 8885, 8886, 8887, 8888, 8889, 8890, 8891, 8892, 8893, 8894, 8895, 8896, 8897, 8898, 8899, 8900, 8901, 8902, 8903, 8904, 8905, 8906, 8907, 8908, 8909, 8910, 8911, 8912, 8913, 8914, 8915, 8916, 8917, 8918, 8919, 8920, 8921, 8922, 8923, 8924, 8925, 8926, 8927, 8928, 8929, 8930, 8931, 8932, 8933, 8934, 8935, 8936, 8911, 8912, 8913, 8914, 8915, 8916, 8917, 8918, 8919, 8920, 8921, 8922, 8923, 8924, 8925, 8926, 8927, 8928, 8929, 8930, 8931, 8932, 8933, 8934, 8935, 8936, 8937, 8938, 8939, 8940, 8941, 8942, 8943, 8944, 8945, 8946, 8947, 8948, 8949, 8950, 8951, 8952, 8953, 8954, 8955, 8956, 8957, 8958, 8959, 8960, 8961, 8962, 8937, 8938, 8939, 8940, 8941, 8942, 8943, 8944, 8945, 8946, 8947, 8948, 8949, 8950, 8951, 8952, 8953, 8954, 8955, 8956, 8957, 8958, 8959, 8960, 8961, 8962, 8963, 8964, 8965, 8966, 8967, 8968, 8969, 8970, 8971, 8972, 8973, 8974, 8975, 8976, 8977, 8978, 8979, 8980, 8981, 8982, 8983, 8984, 8985, 8986, 8987, 8988, 8963, 8964, 8965, 8966, 8967, 8968, 8969, 8970, 8971, 8972, 8973, 8974, 8975, 8976, 8977, 8978, 8979, 8980, 8981, 8982, 8983, 8984, 8985, 8986, 8987, 8988, 
};

void VLine(int x, int y, int count, uint8_t colour)
{
	u8* ptr = framebuffer + framebufferx[x];
	ptr += (y << 2);
	switch(count) {	case 144:
	ptr[572] = colour;
	case 143:
	ptr[568] = colour;
	case 142:
	ptr[564] = colour;
	case 141:
	ptr[560] = colour;
	case 140:
	ptr[556] = colour;
	case 139:
	ptr[552] = colour;
	case 138:
	ptr[548] = colour;
	case 137:
	ptr[544] = colour;
	case 136:
	ptr[540] = colour;
	case 135:
	ptr[536] = colour;
	case 134:
	ptr[532] = colour;
	case 133:
	ptr[528] = colour;
	case 132:
	ptr[524] = colour;
	case 131:
	ptr[520] = colour;
	case 130:
	ptr[516] = colour;
	case 129:
	ptr[512] = colour;
	case 128:
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
void TexturedLine(const uint8_t* texptr, int16_t x, int16_t y, int16_t count, int16_t u, int16_t v, int16_t step)
{
	u8* ptr = framebuffer + framebufferx[x];
	ptr += (y << 2);
	int texcoord = v << 8;
	switch(count) {	case 144:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 143:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 142:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 141:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 140:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 139:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 138:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 137:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 136:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 135:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 134:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 133:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 132:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 131:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 130:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 129:
	*ptr = texptr[(texcoord >> 8) & 127]; texcoord += step; ptr += 4;
	case 128:
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
