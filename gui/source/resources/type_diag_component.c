/* GIMP RGBA C-Source image dump (type_diag_component.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[32 * 24 * 4 + 1];
} type_diag_component = {
  32, 24, 4,
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\177\177"
  "\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377"
  "\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177"
  "\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377"
  "\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177"
  "\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377"
  "\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177"
  "\177\377\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177"
  "\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200@\177\177\177"
  "\365\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200H\200\200"
  "\200\202\200\200\200\202\200\200\200\202\200\200\200\202\200\200\200\202"
  "\200\200\200\202\200\200\200\202\200\200\200\202~~~C\000\000\000\000\177\177\177"
  "\377\000\000\000\000\000\000\000\000\177\177\177\377\177\177\177\377\177\177\177\377\177"
  "\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177"
  "\377\177\177\177\377\177\177\177\377\177\177\177\365\200\200\200@\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200\216\177\177\177\377"
  "~~~\376~~~\376~~~\376~~~\376~~~\376~~~\376\177\177\177\377\177\177\177\205"
  "\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200\216\200\200"
  "\200\362\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\373"
  "\177\177\177\205\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200"
  "\200\216\200\200\200\362\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\177\177\177\373\177\177\177\205\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000"
  "\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\200\200\200\216\200\200\200\362\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\177\177\177\373\177\177\177\205\000\000\000\000\177\177\177\377"
  "\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200\216\200\200\200\362\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\373\177\177\177\205\000\000\000\000"
  "\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\200\200\200\226"
  "\177\177\177\373\177\177\177\373\177\177\177\373\177\177\177\373\177\177"
  "\177\373\177\177\177\373\177\177\177\373\177\177\177\373\177\177\177\373"
  "\177\177\177\373\177\177\177\373\200\200\200\266\000\000\000\000\000\000\000\000\000\000\000\000"
  "\200\200\200\216\200\200\200\362\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\177\177\177\373\177\177\177\205\000\000\000\000\177\177\177\377\000\000\000\000"
  "\000\000\000\000\177\177\177\377\000\000\000\000\177\177\177\231\177\177\177\363\177\177"
  "\177\205\177\177\177\205\177\177\177\205\177\177\177\205\177\177\177\205"
  "\177\177\177\205\177\177\177\205\177\177\177\205\177\177\177\205\200\200"
  "\200\344\200\200\200\272\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200\216\177\177"
  "\177\371\177\177\177\205\177\177\177\205\177\177\177\205\177\177\177\205"
  "\177\177\177\205\177\177\177\205\177\177\177\375\177\177\177\205\000\000\000\000"
  "\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\177\177\177\231"
  "\177\177\177\347\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\200\200\200\306\200\200\200\272\000\000\000\000\000\000\000\000\000\000\000\000"
  "\177\177\177\213\177\177\177\373\177\177\177\373\177\177\177\373\177\177"
  "\177\373~~~\374\177\177\177\373\177\177\177\373\177\177\177\373\200\200\200"
  "\202\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\177"
  "\177\177\231\177\177\177\347\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200\306\200\200\200\272\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200l\200\200\200\224"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000"
  "\177\177\177\377\000\000\000\000\177\177\177\231\177\177\177\347\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200\306"
  "\200\200\200\272\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200\004\000\000\000\000\000"
  "\000\000\000\200\200\200l\200\200\200\224\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\177\177\177\231"
  "\177\177\177\347\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\200\200\200\306\200\200\200\272\000\000\000\000\000\000\000\000qqq\011\200"
  "\200\200\256\200\200\200\316\000\000\000\000}}}\067\206\206\206\324\204\204\204\336"
  "\200\200\200F\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000"
  "\000\000\177\177\177\377\000\000\000\000\177\177\177\231\177\177\177\347\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200"
  "\306\177\177\177\275\200\200\200\016\200\200\200\016\200\200\200z\177\177\177"
  "\251fff\005\200\200\200\012\205\205\205\360\322\322\322\377\331\331\331\377"
  "\205\205\205\363\200\200\200\032\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000"
  "\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\177\177\177\231\177\177\177\347\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200"
  "\200\200\306\177\177\177\353\200\200\200\362\200\200\200\362~~~\336~~~k\000"
  "\000\000\000\200\200\200(\215\215\215\362\377\377\377\377\377\377\377\377\232\232"
  "\232\357\200\200\200B\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000"
  "\000\000\177\177\177\377\000\000\000\000\177\177\177\231\177\177\177\347\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200"
  "\306\200\200\200\272\000\000\000\000\000\000\000\000\200\200\200N\177\177\177\331\200\200"
  "\200D\200\200\200\004\206\206\206\341\266\266\266\372\274\274\274\374\205\205"
  "\205\354\200\200\200\016\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000"
  "\000\000\000\177\177\177\377\000\000\000\000\177\177\177\231\177\177\177\347\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200"
  "\306\200\200\200\272\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200Z\200\200\200\206"
  "\000\000\000\000\200\200\200\026\202\202\202\207\205\205\205\216\200\200\200\040\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177"
  "\377\000\000\000\000\177\177\177\231\177\177\177\377\200\200\200\372\200\200\200"
  "\372\200\200\200\372\200\200\200\372\200\200\200\372\200\200\200\372\200"
  "\200\200\372\200\200\200\372\200\200\200\372~~~\376\200\200\200\272\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177"
  "\177\377\000\000\000\000\200\200\200P\200\200\200\206\200\200\200\206\200\200\200"
  "\206\200\200\200\206\200\200\200\206\200\200\200\206\200\200\200\206\200"
  "\200\200\206\200\200\200\206\200\200\200\206\200\200\200\206~~~a\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177"
  "\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\177\177"
  "\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377"
  "\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177"
  "\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377"
  "\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177"
  "\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377"
  "\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177"
  "\177\377\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000",
};
