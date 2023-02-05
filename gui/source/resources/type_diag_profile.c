/* GIMP RGBA C-Source image dump (type_diag_profile.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[32 * 24 * 4 + 1];
} type_diag_profile = {
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
  "\365\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200\062\177\177\177\231"
  "\200\200\200\232\200\200\200\202\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377"
  "\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177"
  "\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377"
  "\177\177\177\365\200\200\200@\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200"
  "\200\200h\200\200\200\276~~~}\200\200\200\326\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177"
  "\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200"
  "\200\200\202\200\200\200~\200\200\200\026\200\200\200\354\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000"
  "\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200\004\200\200\200\310\200\200\200"
  "~\200\200\200\014\000\000\000\000\000\000\000\000\200\200\200\234\200\200\200d\200\200\200"
  "\002\200\200\200\372\200\200\200\004\000\000\000\000\000\000\000\000\200\200\200(\177\177\177"
  "\235\200\200\200\034\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177"
  "\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000~~~k\200\200\200\302\177\177\177\245\200\200\200\346~~~o"
  "\200\200\200\232\200\200\200\354\200\200\200\070\000\000\000\000\200\200\200\322\200"
  "\200\200\270\200\200\200R\177\177\177\253\177\177\177\351\200\200\200\304"
  "\177\177\177\215\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177"
  "\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\200\200\200\302\177\177\177\251\200\200\200\010\200\200\200"
  "\062\200\200\200\262\177\177\177\201\200\200\200\022\000\000\000\000\000\000\000\000\200\200"
  "\200\006\200\200\200h\200\200\200\312\200\200\200pttt\013\200\200\200\036\177"
  "\177\177\361\200\200\200\022\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177"
  "\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\200\200\200\014\200\200\200\246\177\177\177\315|||!\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000}}}-\200"
  "\200\200\312\200\200\200\272\200\200\200\030\000\000\000\000\177\177\177\377\000\000\000"
  "\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000~~~y\200\200\200\310\000"
  "\000\000\000\000\000\000\000UUU\003\204\204\204~\206\206\206\240{{{\033\000\000\000\000\000\000\000\000\200"
  "\200\200d\177\177\177\351~~~c\000\000\000\001\000\000\000\000\000\000\000\000\177\177\177\377\000\000"
  "\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200\060\177\177"
  "\177\317\000\000\000\000\000\000\000\000\200\200\200Z\230\230\230\364\266\266\266\374\210"
  "\210\210\270\000\000\000\000\000\000\000\000\177\177\177\243\200\200\200`\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000}}}\065\177\177\177\323\000\000\000\000\000\000\000\000~~~a\235\235\235\364"
  "\277\277\277\376\210\210\210\277\000\000\000\000\000\000\000\000\200\200\200\242~~~]\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177"
  "\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\200\200\200N\177\177\177\345\177\177\177\201\000\000\000\000"
  "\000\000\000\000fff\005\206\206\206\224\211\211\211\267\200\200\200$\000\000\000\000\000\000\000"
  "\000~~~w\177\177\177\333}}}\065\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000"
  "\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200\016\200\200\200\240\200\200\200\334"
  "\200\200\200<\000\000\000\000\200\200\200\016\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000~~~S\200\200\200\350\200\200\200x\200\200\200"
  "\002\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000yyy\025\200\200"
  "\200\364|||'\200\200\200.\177\177\177\253\200\200\200\372\200\200\200\216"
  "ttt\013\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200<\200\200\200\240~~~]UUU\003}}}/"
  "\177\177\177\371\200\200\200\040\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177"
  "\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\177\177\177\213\177\177\177\337\200\200\200\346\200"
  "\200\200r\200\200\200\036\177\177\177\241\177\177\177\337\200\200\200\016\200"
  "\200\200\022\200\200\200\316\200\200\200\330~~~w\177\177\177\301\200\200\200"
  "\330\177\177\177\263\200\200\200\226\000\000\000\000\000\000\000\000\177\177\177\377\000\000"
  "\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000zzz\027~~~oqqq\011\000\000\000\000\000\000\000\000\000\000"
  "\000\000\177\177\177\327\200\200\200*\200\200\200\064\177\177\177\317\000\000\000\001"
  "\000\000\000\000\000\000\000\000\200\200\200B\177\177\177\273\200\200\200\032\000\000\000\000\000\000"
  "\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\301\200\200\200@\200\200"
  "\200P\200\200\200\260\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\253\177\177\177"
  "\201\200\200\200\216\177\177\177\227\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177"
  "\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200"
  "p\177\177\177\273\177\177\177\273\200\200\200^\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177"
  "\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\177"
  "\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177"
  "\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177"
  "\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177"
  "\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177"
  "\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177"
  "\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177"
  "\177\177\377\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000",
};

