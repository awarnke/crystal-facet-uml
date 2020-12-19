/* GIMP RGBA C-Source image dump (edit_highlight.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[32 * 32 * 4 + 1];
} edit_highlight = {
  32, 32, 4,
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\006"
  "\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\003\016\013\001\266\016\013\002\251\000\000\000\004\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\001"
  "\015\013\002\243>\064\010\364C\071\011\365\017\014\003\274[[[\034}}}\067\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\013\013\002\217-'\006\364\363\316\""
  "\377\301\247\022\377dW\012\371\"\040\030\367xxx\370\200\200\200V\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\007\007\002q!\034\004\363\354\310!\377\311\256\025\377"
  "\222\201\005\377\362\316!\377n^\023\377\033\032\022\377rrr\374\200\200\200j\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\005\005\000_\025\022\003\365\341\277\040\377\321\263\027\377\217"
  "~\004\377\357\313\037\377\377\330$\377\377\330$\377\205r\024\377\026\024\015\377"
  "nnn\377\177\177\177\221\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\003\003\000R\021\016\002\365\332\271\037\377\327\270"
  "\031\377\214}\004\377\353\310\036\377\377\330$\377\377\330$\377\370\322\"\377"
  "\232\210\007\377\214y\020\377\023\022\013\377jjj\377\177\177\177\241fff\005\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000A\014\012\002\365\316"
  "\256\035\377\336\275\033\377\213|\003\377\346\305\035\377\377\330$\377\377\330"
  "$\377\373\324#\377\240\214\011\377\265\235\017\377\352\306!\377\030\025\007\377"
  "LLK\377\177\177\177\377\177\177\177\273\200\200\200\016\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000/\014\013\002\362\301\243\033\377\343\302\034\377\213"
  "|\003\377\341\300\033\377\377\330$\377\377\330$\377\375\326#\377\245\221\013"
  "\377\256\230\015\377\364\317#\377\063,\014\377BA=\377\177\177\177\377\177\177"
  "\177\377\177\177\177\377~~~g\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000%\013\011"
  "\001\357\255\223\030\376\351\306\036\377\213|\003\377\333\273\032\377\377\330$\377"
  "\377\330$\377\376\327$\377\254\226\014\377\247\222\013\377\367\322\"\377B\071"
  "\016\377\070\067\060\377\177\177\177\377\177\177\177\377~~~\376\200\200\200v"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\033\013\012\001\351\234\204\026\375\355"
  "\312\037\377\215~\004\377\324\266\030\377\377\330$\377\377\330$\377\376\327$"
  "\377\263\234\017\377\241\215\011\377\371\323#\377QE\017\377,+$\377~~~\377\177"
  "\177\177\377\200\200\200\374~~~c\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\024"
  "\014\012\001\341\212u\023\373\361\315\040\377\220\200\005\377\315\260\026\377\377"
  "\330$\377\377\330$\377\377\330$\377\273\242\021\377\233\211\010\377\371\323"
  "\"\377l[\022\377&$\034\377{{{\377\177\177\177\377\200\200\200\370\200\200\200"
  "R\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\012\017\014\002\322yf\021\371\365\317!\377"
  "\224\203\006\377\306\253\024\377\377\330$\377\377\330$\377\377\330$\377\302"
  "\247\023\377\226\204\006\377\366\321!\377}j\023\377\034\033\023\377www\377\177\177"
  "\177\377\177\177\177\357}}}=\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\006\015\014"
  "\001\302bS\015\366\370\322\"\377\231\206\007\377\300\246\022\377\377\330$\377\377"
  "\330$\377\377\330$\377\312\256\025\377\221\201\005\377\363\316\040\377\225\177"
  "\026\377\027\025\016\377sss\377\177\177\177\377\177\177\177\347\200\200\200\060"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\003\015\013\001\263J>\012\365\367\322#\377"
  "\236\213\011\377\266\236\017\377\377\330$\377\377\330$\377\377\330$\377\321"
  "\263\027\377\216\177\004\377\357\313\037\377\256\224\031\377\024\022\013\377ooo\377"
  "\177\177\177\377\200\200\200\336\200\200\200$\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\001\015\013\002\243;\062\010\364\365\320\"\377\244\220\012\377\257\231"
  "\015\377\376\327$\377\377\330$\377\377\330$\377\330\271\031\377\214}\004\377"
  "\352\307\036\377\276\241\033\377\020\016\007\377ggg\377\177\177\177\377\177\177"
  "\177\323\200\200\200\032\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\013\013\002\217"
  "-'\006\364\362\315\"\377\253\225\014\377\250\223\013\377\375\327#\377\377\330"
  "$\377\377\330$\377\336\276\033\377\213|\003\377\346\304\035\377\320\260\035\377"
  "\022\020\007\377aaa\377\177\177\177\377\200\200\200\300www\017\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000)\027\024\002\367\353\307!\377\262\233\016\377\242"
  "\216\012\377\374\325#\377\377\330$\377\377\330$\377\344\302\034\377\213|\003"
  "\377\340\277\033\377\332\271\037\377\024\022\007\377XXW\377\177\177\177\377\200"
  "\200\200\256\200\200\200\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\"\060+\002\366\254\225\014\377\260\231\016\377\372\325#\377\377\330$"
  "\377\377\330$\377\351\307\036\377\214|\003\377\332\272\031\377\345\302\040\377"
  "\034\030\010\377ONM\377\177\177\177\377\177\177\177\241\200\200\200\004\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\025\"\035\002\370\200"
  "s\000\377\261\232\016\377\376\327$\377\377\330$\377\356\312\037\377\216~\004\377"
  "\323\265\030\377\360\313\"\377+%\013\377GFB\377\177\177\177\377\177\177\177"
  "\221\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000'\022\020\001\373\201t\000\377\200s\000\377\265\236\017\377\361\314\040\377"
  "\220\200\005\377\316\261\026\377\366\320#\377\066/\014\377<;\067\377~~~\376~~~"
  "u\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\023\013\012\001\342\221z\024\374\327\270\031\377\204v\001\377\200s\000\377\264"
  "\235\017\377\323\265\030\377\374\325$\377MB\017\377\062\061*\377~~~\374\200\200"
  "\200b\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\011\014\013\001\317tc\020\370\377\330$\377\377\330$\377\303\250\027\377"
  "\206u\007\377\214{\006\377\256\226\021\377\\N\020\377+)!\377}}}\370\200\200\200"
  "R\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000l\002\002\000\375tb\020\371\342\277\040\377\331\270\037\377\030\025\010"
  "\377\063\062,\377.,!\377\036\034\022\377'%\035\377yyy\360\200\200\200<\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000$\014\012\002\226\010\007\001\364\013\011\002\371?"
  "?=\273\200\200"
  "\200\200~~~o\200\200\200^~~~M\200\200\200\040\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\014\000\000\000\070\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377@\325\014\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\377\063\314\005\377<\322\264\377;\322l\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377<\323]\377=\322"
  "\261\377@\317\040\377<\322\345\377=\322O\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377;\321=\377<\322\352\377:\321,\000\000\000\000\377"
  "=\320\066\377<\322\365\377<\322\377\377<\322\377\377<\322\377\377<\322\377"
  "\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322"
  "\377\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377"
  "<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322\377"
  "\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322"
  "\371\377;\324A\000\000\000\000",
};

