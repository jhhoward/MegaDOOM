const u16 framebufferx[] = {
0, 1, 2, 3, 576, 577, 578, 579, 1152, 1153, 1154, 1155, 1728, 1729, 1730, 1731, 2304, 2305, 2306, 2307, 2880, 2881, 2882, 2883, 3456, 3457, 3458, 3459, 4032, 4033, 4034, 4035, 4608, 4609, 4610, 4611, 5184, 5185, 5186, 5187, 5760, 5761, 5762, 5763, 6336, 6337, 6338, 6339, 6912, 6913, 6914, 6915, 7488, 7489, 7490, 7491, 8064, 8065, 8066, 8067, 8640, 8641, 8642, 8643, 9216, 9217, 9218, 9219, 9792, 9793, 9794, 9795, 10368, 10369, 10370, 10371, 10944, 10945, 10946, 10947, 11520, 11521, 11522, 11523, 12096, 12097, 12098, 12099, 12672, 12673, 12674, 12675, 13248, 13249, 13250, 13251, 13824, 13825, 13826, 13827, 14400, 14401, 14402, 14403, };

const u16 screenLayoutTiles[] = {
16, 16, 16, 17, 35, 53, 71, 89, 107, 125, 143, 161, 179, 197, 215, 233, 251, 269, 287, 305, 323, 341, 359, 377, 395, 413, 431, 449, 467, 16, 16, 16, 16, 16, 16, 18, 36, 54, 72, 90, 108, 126, 144, 162, 180, 198, 216, 234, 252, 270, 288, 306, 324, 342, 360, 378, 396, 414, 432, 450, 468, 16, 16, 16, 16, 16, 16, 19, 37, 55, 73, 91, 109, 127, 145, 163, 181, 199, 217, 235, 253, 271, 289, 307, 325, 343, 361, 379, 397, 415, 433, 451, 469, 16, 16, 16, 16, 16, 16, 20, 38, 56, 74, 92, 110, 128, 146, 164, 182, 200, 218, 236, 254, 272, 290, 308, 326, 344, 362, 380, 398, 416, 434, 452, 470, 16, 16, 16, 16, 16, 16, 21, 39, 57, 75, 93, 111, 129, 147, 165, 183, 201, 219, 237, 255, 273, 291, 309, 327, 345, 363, 381, 399, 417, 435, 453, 471, 16, 16, 16, 16, 16, 16, 22, 40, 58, 76, 94, 112, 130, 148, 166, 184, 202, 220, 238, 256, 274, 292, 310, 328, 346, 364, 382, 400, 418, 436, 454, 472, 16, 16, 16, 16, 16, 16, 23, 41, 59, 77, 95, 113, 131, 149, 167, 185, 203, 221, 239, 257, 275, 293, 311, 329, 347, 365, 383, 401, 419, 437, 455, 473, 16, 16, 16, 16, 16, 16, 24, 42, 60, 78, 96, 114, 132, 150, 168, 186, 204, 222, 240, 258, 276, 294, 312, 330, 348, 366, 384, 402, 420, 438, 456, 474, 16, 16, 16, 16, 16, 16, 25, 43, 61, 79, 97, 115, 133, 151, 169, 187, 205, 223, 241, 259, 277, 295, 313, 331, 349, 367, 385, 403, 421, 439, 457, 475, 16, 16, 16, 16, 16, 16, 26, 44, 62, 80, 98, 116, 134, 152, 170, 188, 206, 224, 242, 260, 278, 296, 314, 332, 350, 368, 386, 404, 422, 440, 458, 476, 16, 16, 16, 16, 16, 16, 27, 45, 63, 81, 99, 117, 135, 153, 171, 189, 207, 225, 243, 261, 279, 297, 315, 333, 351, 369, 387, 405, 423, 441, 459, 477, 16, 16, 16, 16, 16, 16, 28, 46, 64, 82, 100, 118, 136, 154, 172, 190, 208, 226, 244, 262, 280, 298, 316, 334, 352, 370, 388, 406, 424, 442, 460, 478, 16, 16, 16, 16, 16, 16, 29, 47, 65, 83, 101, 119, 137, 155, 173, 191, 209, 227, 245, 263, 281, 299, 317, 335, 353, 371, 389, 407, 425, 443, 461, 479, 16, 16, 16, 16, 16, 16, 30, 48, 66, 84, 102, 120, 138, 156, 174, 192, 210, 228, 246, 264, 282, 300, 318, 336, 354, 372, 390, 408, 426, 444, 462, 480, 16, 16, 16, 16, 16, 16, 31, 49, 67, 85, 103, 121, 139, 157, 175, 193, 211, 229, 247, 265, 283, 301, 319, 337, 355, 373, 391, 409, 427, 445, 463, 481, 16, 16, 16, 16, 16, 16, 32, 50, 68, 86, 104, 122, 140, 158, 176, 194, 212, 230, 248, 266, 284, 302, 320, 338, 356, 374, 392, 410, 428, 446, 464, 482, 16, 16, 16, 16, 16, 16, 33, 51, 69, 87, 105, 123, 141, 159, 177, 195, 213, 231, 249, 267, 285, 303, 321, 339, 357, 375, 393, 411, 429, 447, 465, 483, 16, 16, 16, 16, 16, 16, 34, 52, 70, 88, 106, 124, 142, 160, 178, 196, 214, 232, 250, 268, 286, 304, 322, 340, 358, 376, 394, 412, 430, 448, 466, 484, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 
};

const u16 skyLayoutTiles[] = {
8677, 8678, 8679, 8680, 8681, 8682, 8683, 8684, 8685, 8686, 8687, 8688, 8689, 8690, 8691, 8692, 8693, 8694, 8695, 8696, 8697, 8698, 8699, 8700, 8701, 8702, 8677, 8678, 8679, 8680, 8681, 8682, 8683, 8684, 8685, 8686, 8687, 8688, 8689, 8690, 8691, 8692, 8693, 8694, 8695, 8696, 8697, 8698, 8699, 8700, 8701, 8702, 8703, 8704, 8705, 8706, 8707, 8708, 8709, 8710, 8711, 8712, 8713, 8714, 8715, 8716, 8717, 8718, 8719, 8720, 8721, 8722, 8723, 8724, 8725, 8726, 8727, 8728, 8703, 8704, 8705, 8706, 8707, 8708, 8709, 8710, 8711, 8712, 8713, 8714, 8715, 8716, 8717, 8718, 8719, 8720, 8721, 8722, 8723, 8724, 8725, 8726, 8727, 8728, 8729, 8730, 8731, 8732, 8733, 8734, 8735, 8736, 8737, 8738, 8739, 8740, 8741, 8742, 8743, 8744, 8745, 8746, 8747, 8748, 8749, 8750, 8751, 8752, 8753, 8754, 8729, 8730, 8731, 8732, 8733, 8734, 8735, 8736, 8737, 8738, 8739, 8740, 8741, 8742, 8743, 8744, 8745, 8746, 8747, 8748, 8749, 8750, 8751, 8752, 8753, 8754, 8755, 8756, 8757, 8758, 8759, 8760, 8761, 8762, 8763, 8764, 8765, 8766, 8767, 8768, 8769, 8770, 8771, 8772, 8773, 8774, 8775, 8776, 8777, 8778, 8779, 8780, 8755, 8756, 8757, 8758, 8759, 8760, 8761, 8762, 8763, 8764, 8765, 8766, 8767, 8768, 8769, 8770, 8771, 8772, 8773, 8774, 8775, 8776, 8777, 8778, 8779, 8780, 8781, 8782, 8783, 8784, 8785, 8786, 8787, 8788, 8789, 8790, 8791, 8792, 8793, 8794, 8795, 8796, 8797, 8798, 8799, 8800, 8801, 8802, 8803, 8804, 8805, 8806, 8781, 8782, 8783, 8784, 8785, 8786, 8787, 8788, 8789, 8790, 8791, 8792, 8793, 8794, 8795, 8796, 8797, 8798, 8799, 8800, 8801, 8802, 8803, 8804, 8805, 8806, 8807, 8808, 8809, 8810, 8811, 8812, 8813, 8814, 8815, 8816, 8817, 8818, 8819, 8820, 8821, 8822, 8823, 8824, 8825, 8826, 8827, 8828, 8829, 8830, 8831, 8832, 8807, 8808, 8809, 8810, 8811, 8812, 8813, 8814, 8815, 8816, 8817, 8818, 8819, 8820, 8821, 8822, 8823, 8824, 8825, 8826, 8827, 8828, 8829, 8830, 8831, 8832, 8833, 8834, 8835, 8836, 8837, 8838, 8839, 8840, 8841, 8842, 8843, 8844, 8845, 8846, 8847, 8848, 8849, 8850, 8851, 8852, 8853, 8854, 8855, 8856, 8857, 8858, 8833, 8834, 8835, 8836, 8837, 8838, 8839, 8840, 8841, 8842, 8843, 8844, 8845, 8846, 8847, 8848, 8849, 8850, 8851, 8852, 8853, 8854, 8855, 8856, 8857, 8858, 8859, 8860, 8861, 8862, 8863, 8864, 8865, 8866, 8867, 8868, 8869, 8870, 8871, 8872, 8873, 8874, 8875, 8876, 8877, 8878, 8879, 8880, 8881, 8882, 8883, 8884, 8859, 8860, 8861, 8862, 8863, 8864, 8865, 8866, 8867, 8868, 8869, 8870, 8871, 8872, 8873, 8874, 8875, 8876, 8877, 8878, 8879, 8880, 8881, 8882, 8883, 8884, 8885, 8886, 8887, 8888, 8889, 8890, 8891, 8892, 8893, 8894, 8895, 8896, 8897, 8898, 8899, 8900, 8901, 8902, 8903, 8904, 8905, 8906, 8907, 8908, 8909, 8910, 8885, 8886, 8887, 8888, 8889, 8890, 8891, 8892, 8893, 8894, 8895, 8896, 8897, 8898, 8899, 8900, 8901, 8902, 8903, 8904, 8905, 8906, 8907, 8908, 8909, 8910, 8911, 8912, 8913, 8914, 8915, 8916, 8917, 8918, 8919, 8920, 8921, 8922, 8923, 8924, 8925, 8926, 8927, 8928, 8929, 8930, 8931, 8932, 8933, 8934, 8935, 8936, 8911, 8912, 8913, 8914, 8915, 8916, 8917, 8918, 8919, 8920, 8921, 8922, 8923, 8924, 8925, 8926, 8927, 8928, 8929, 8930, 8931, 8932, 8933, 8934, 8935, 8936, 8937, 8938, 8939, 8940, 8941, 8942, 8943, 8944, 8945, 8946, 8947, 8948, 8949, 8950, 8951, 8952, 8953, 8954, 8955, 8956, 8957, 8958, 8959, 8960, 8961, 8962, 8937, 8938, 8939, 8940, 8941, 8942, 8943, 8944, 8945, 8946, 8947, 8948, 8949, 8950, 8951, 8952, 8953, 8954, 8955, 8956, 8957, 8958, 8959, 8960, 8961, 8962, 8963, 8964, 8965, 8966, 8967, 8968, 8969, 8970, 8971, 8972, 8973, 8974, 8975, 8976, 8977, 8978, 8979, 8980, 8981, 8982, 8983, 8984, 8985, 8986, 8987, 8988, 8963, 8964, 8965, 8966, 8967, 8968, 8969, 8970, 8971, 8972, 8973, 8974, 8975, 8976, 8977, 8978, 8979, 8980, 8981, 8982, 8983, 8984, 8985, 8986, 8987, 8988, 
};

void R_DrawVLine(void)
{
	int16_t count = dc_yh - dc_yl;
	if(count < 0) return;
	u8* ptr = framebuffer + framebufferx[dc_x];
	ptr += (dc_yl << 2);
	switch(count) {	case 144:
	*ptr = dc_col;
	ptr += 4;
	case 143:
	*ptr = dc_col;
	ptr += 4;
	case 142:
	*ptr = dc_col;
	ptr += 4;
	case 141:
	*ptr = dc_col;
	ptr += 4;
	case 140:
	*ptr = dc_col;
	ptr += 4;
	case 139:
	*ptr = dc_col;
	ptr += 4;
	case 138:
	*ptr = dc_col;
	ptr += 4;
	case 137:
	*ptr = dc_col;
	ptr += 4;
	case 136:
	*ptr = dc_col;
	ptr += 4;
	case 135:
	*ptr = dc_col;
	ptr += 4;
	case 134:
	*ptr = dc_col;
	ptr += 4;
	case 133:
	*ptr = dc_col;
	ptr += 4;
	case 132:
	*ptr = dc_col;
	ptr += 4;
	case 131:
	*ptr = dc_col;
	ptr += 4;
	case 130:
	*ptr = dc_col;
	ptr += 4;
	case 129:
	*ptr = dc_col;
	ptr += 4;
	case 128:
	*ptr = dc_col;
	ptr += 4;
	case 127:
	*ptr = dc_col;
	ptr += 4;
	case 126:
	*ptr = dc_col;
	ptr += 4;
	case 125:
	*ptr = dc_col;
	ptr += 4;
	case 124:
	*ptr = dc_col;
	ptr += 4;
	case 123:
	*ptr = dc_col;
	ptr += 4;
	case 122:
	*ptr = dc_col;
	ptr += 4;
	case 121:
	*ptr = dc_col;
	ptr += 4;
	case 120:
	*ptr = dc_col;
	ptr += 4;
	case 119:
	*ptr = dc_col;
	ptr += 4;
	case 118:
	*ptr = dc_col;
	ptr += 4;
	case 117:
	*ptr = dc_col;
	ptr += 4;
	case 116:
	*ptr = dc_col;
	ptr += 4;
	case 115:
	*ptr = dc_col;
	ptr += 4;
	case 114:
	*ptr = dc_col;
	ptr += 4;
	case 113:
	*ptr = dc_col;
	ptr += 4;
	case 112:
	*ptr = dc_col;
	ptr += 4;
	case 111:
	*ptr = dc_col;
	ptr += 4;
	case 110:
	*ptr = dc_col;
	ptr += 4;
	case 109:
	*ptr = dc_col;
	ptr += 4;
	case 108:
	*ptr = dc_col;
	ptr += 4;
	case 107:
	*ptr = dc_col;
	ptr += 4;
	case 106:
	*ptr = dc_col;
	ptr += 4;
	case 105:
	*ptr = dc_col;
	ptr += 4;
	case 104:
	*ptr = dc_col;
	ptr += 4;
	case 103:
	*ptr = dc_col;
	ptr += 4;
	case 102:
	*ptr = dc_col;
	ptr += 4;
	case 101:
	*ptr = dc_col;
	ptr += 4;
	case 100:
	*ptr = dc_col;
	ptr += 4;
	case 99:
	*ptr = dc_col;
	ptr += 4;
	case 98:
	*ptr = dc_col;
	ptr += 4;
	case 97:
	*ptr = dc_col;
	ptr += 4;
	case 96:
	*ptr = dc_col;
	ptr += 4;
	case 95:
	*ptr = dc_col;
	ptr += 4;
	case 94:
	*ptr = dc_col;
	ptr += 4;
	case 93:
	*ptr = dc_col;
	ptr += 4;
	case 92:
	*ptr = dc_col;
	ptr += 4;
	case 91:
	*ptr = dc_col;
	ptr += 4;
	case 90:
	*ptr = dc_col;
	ptr += 4;
	case 89:
	*ptr = dc_col;
	ptr += 4;
	case 88:
	*ptr = dc_col;
	ptr += 4;
	case 87:
	*ptr = dc_col;
	ptr += 4;
	case 86:
	*ptr = dc_col;
	ptr += 4;
	case 85:
	*ptr = dc_col;
	ptr += 4;
	case 84:
	*ptr = dc_col;
	ptr += 4;
	case 83:
	*ptr = dc_col;
	ptr += 4;
	case 82:
	*ptr = dc_col;
	ptr += 4;
	case 81:
	*ptr = dc_col;
	ptr += 4;
	case 80:
	*ptr = dc_col;
	ptr += 4;
	case 79:
	*ptr = dc_col;
	ptr += 4;
	case 78:
	*ptr = dc_col;
	ptr += 4;
	case 77:
	*ptr = dc_col;
	ptr += 4;
	case 76:
	*ptr = dc_col;
	ptr += 4;
	case 75:
	*ptr = dc_col;
	ptr += 4;
	case 74:
	*ptr = dc_col;
	ptr += 4;
	case 73:
	*ptr = dc_col;
	ptr += 4;
	case 72:
	*ptr = dc_col;
	ptr += 4;
	case 71:
	*ptr = dc_col;
	ptr += 4;
	case 70:
	*ptr = dc_col;
	ptr += 4;
	case 69:
	*ptr = dc_col;
	ptr += 4;
	case 68:
	*ptr = dc_col;
	ptr += 4;
	case 67:
	*ptr = dc_col;
	ptr += 4;
	case 66:
	*ptr = dc_col;
	ptr += 4;
	case 65:
	*ptr = dc_col;
	ptr += 4;
	case 64:
	*ptr = dc_col;
	ptr += 4;
	case 63:
	*ptr = dc_col;
	ptr += 4;
	case 62:
	*ptr = dc_col;
	ptr += 4;
	case 61:
	*ptr = dc_col;
	ptr += 4;
	case 60:
	*ptr = dc_col;
	ptr += 4;
	case 59:
	*ptr = dc_col;
	ptr += 4;
	case 58:
	*ptr = dc_col;
	ptr += 4;
	case 57:
	*ptr = dc_col;
	ptr += 4;
	case 56:
	*ptr = dc_col;
	ptr += 4;
	case 55:
	*ptr = dc_col;
	ptr += 4;
	case 54:
	*ptr = dc_col;
	ptr += 4;
	case 53:
	*ptr = dc_col;
	ptr += 4;
	case 52:
	*ptr = dc_col;
	ptr += 4;
	case 51:
	*ptr = dc_col;
	ptr += 4;
	case 50:
	*ptr = dc_col;
	ptr += 4;
	case 49:
	*ptr = dc_col;
	ptr += 4;
	case 48:
	*ptr = dc_col;
	ptr += 4;
	case 47:
	*ptr = dc_col;
	ptr += 4;
	case 46:
	*ptr = dc_col;
	ptr += 4;
	case 45:
	*ptr = dc_col;
	ptr += 4;
	case 44:
	*ptr = dc_col;
	ptr += 4;
	case 43:
	*ptr = dc_col;
	ptr += 4;
	case 42:
	*ptr = dc_col;
	ptr += 4;
	case 41:
	*ptr = dc_col;
	ptr += 4;
	case 40:
	*ptr = dc_col;
	ptr += 4;
	case 39:
	*ptr = dc_col;
	ptr += 4;
	case 38:
	*ptr = dc_col;
	ptr += 4;
	case 37:
	*ptr = dc_col;
	ptr += 4;
	case 36:
	*ptr = dc_col;
	ptr += 4;
	case 35:
	*ptr = dc_col;
	ptr += 4;
	case 34:
	*ptr = dc_col;
	ptr += 4;
	case 33:
	*ptr = dc_col;
	ptr += 4;
	case 32:
	*ptr = dc_col;
	ptr += 4;
	case 31:
	*ptr = dc_col;
	ptr += 4;
	case 30:
	*ptr = dc_col;
	ptr += 4;
	case 29:
	*ptr = dc_col;
	ptr += 4;
	case 28:
	*ptr = dc_col;
	ptr += 4;
	case 27:
	*ptr = dc_col;
	ptr += 4;
	case 26:
	*ptr = dc_col;
	ptr += 4;
	case 25:
	*ptr = dc_col;
	ptr += 4;
	case 24:
	*ptr = dc_col;
	ptr += 4;
	case 23:
	*ptr = dc_col;
	ptr += 4;
	case 22:
	*ptr = dc_col;
	ptr += 4;
	case 21:
	*ptr = dc_col;
	ptr += 4;
	case 20:
	*ptr = dc_col;
	ptr += 4;
	case 19:
	*ptr = dc_col;
	ptr += 4;
	case 18:
	*ptr = dc_col;
	ptr += 4;
	case 17:
	*ptr = dc_col;
	ptr += 4;
	case 16:
	*ptr = dc_col;
	ptr += 4;
	case 15:
	*ptr = dc_col;
	ptr += 4;
	case 14:
	*ptr = dc_col;
	ptr += 4;
	case 13:
	*ptr = dc_col;
	ptr += 4;
	case 12:
	*ptr = dc_col;
	ptr += 4;
	case 11:
	*ptr = dc_col;
	ptr += 4;
	case 10:
	*ptr = dc_col;
	ptr += 4;
	case 9:
	*ptr = dc_col;
	ptr += 4;
	case 8:
	*ptr = dc_col;
	ptr += 4;
	case 7:
	*ptr = dc_col;
	ptr += 4;
	case 6:
	*ptr = dc_col;
	ptr += 4;
	case 5:
	*ptr = dc_col;
	ptr += 4;
	case 4:
	*ptr = dc_col;
	ptr += 4;
	case 3:
	*ptr = dc_col;
	ptr += 4;
	case 2:
	*ptr = dc_col;
	ptr += 4;
	case 1:
	*ptr = dc_col;
	ptr += 4;
	case 0:
	*ptr = dc_col;
	break;
	}
}

void R_DrawColumn(void)
{
	int16_t count;
	byte texel;
	fixed_t frac;
	ufixed16_t fracstep;
	count = dc_yh - dc_yl;
	if (count < 0) return;
	u8* dest = framebuffer + framebufferx[dc_x];
	fracstep = dc_iscale;
	frac = dc_texturemid + (dc_yl - centery) * fracstep;
	dest += (dc_yl << 2);
	switch(count) {
	case 144:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 143:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 142:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 141:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 140:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 139:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 138:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 137:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 136:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 135:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 134:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 133:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 132:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 131:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 130:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 129:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 128:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 127:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 126:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 125:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 124:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 123:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 122:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 121:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 120:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 119:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 118:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 117:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 116:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 115:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 114:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 113:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 112:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 111:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 110:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 109:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 108:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 107:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 106:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 105:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 104:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 103:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 102:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 101:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 100:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 99:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 98:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 97:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 96:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 95:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 94:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 93:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 92:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 91:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 90:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 89:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 88:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 87:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 86:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 85:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 84:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 83:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 82:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 81:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 80:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 79:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 78:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 77:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 76:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 75:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 74:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 73:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 72:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 71:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 70:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 69:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 68:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 67:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 66:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 65:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 64:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 63:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 62:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 61:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 60:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 59:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 58:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 57:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 56:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 55:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 54:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 53:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 52:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 51:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 50:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 49:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 48:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 47:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 46:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 45:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 44:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 43:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 42:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 41:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 40:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 39:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 38:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 37:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 36:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 35:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 34:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 33:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 32:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 31:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 30:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 29:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 28:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 27:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 26:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 25:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 24:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 23:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 22:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 21:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 20:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 19:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 18:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 17:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 16:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 15:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 14:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 13:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 12:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 11:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 10:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 9:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 8:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 7:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 6:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 5:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 4:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 3:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 2:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 1:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
		dest += 4;
		frac += fracstep;
	case 0:
		texel = dc_source[(frac >> FRACBITS)];
		*dest = texel;
	}
}

