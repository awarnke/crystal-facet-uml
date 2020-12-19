/* GIMP RGBA C-Source image dump (message_info.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[32 * 32 * 4 + 1];
} message_info = {
  32, 32, 4,
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\025\000\000\000f\004\012\022\265\004\010\016\346\001\001\002\374\002\004\007\371\003\005"
  "\011\370\000\000\001\377\003\006\015\354\005\013\022\302\002\002\002u\000\000\000#\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\040\004\007\017\255"
  "\002\005\010\367\027,T\364-V\241\377<r\326\377F\205\371\377H\210\377\377H\210\377"
  "\377G\207\375\377?v\336\377\061]\257\377\035\067f\367\004\007\017\366\005\011\020\311"
  "\000\000\000\066\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\013\002\005\011\223\003\005"
  "\012\365$C~\372D\201\362\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377G\206\373\377-T\236\376\005\012\024\365\004\010\021\266\000\000\000\035\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\036\004\007\015\332\031/Y\367B|\351\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377E\203\366\377%D\200\373\003\005\013\357\000\000\000:\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\065\003\005\012\356)M\216"
  "\372H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377=s\330\377&E~\377$;e\377,T\235\377G\206\373\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "\064b\270\377\004\007\017\366\003\003\010^\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\036\003\005\013\357\062_\262\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377\064a\265"
  "\377\017\022\027\377z{|\377\232\232\232\377?BG\377\031%<\377F\204\370\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "<r\326\377\004\010\020\366\000\000\000A\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\011\004\007\016\327)L\216\373H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377F\204\370\377\017\023\033\377\310"
  "\310\310\377\377\377\377\377\377\377\377\377\377\377\377\377MPU\377*K\210"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377\067g\302\377\003\005\013\361\000\000\000$\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\003\007\014\224\032\060Y\366H\210\377\377H\210\377\377H\210\377\377H"
  "\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377:m\315\377"
  "'+\062\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\274\274\274\377\036+C\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377)M\221\374\004\011\017"
  "\315\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\040\003\005\012\366B|\350\377H\210\377\377H"
  "\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377;o\321\377$'.\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\267\267\267\377\037.H\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377G\207\375\377\012\025'\363\000\000\000M\000\000\000\000\000\000\000\000\004\007\017\254$C~\372H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377G\205\372\377\021\027!\377\272\272\272"
  "\377\377\377\377\377\377\377\377\377\374\374\374\377@CH\377,P\222\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377\063a\266\377\003\007\016\341\000\000\000\002\000"
  "\000\000\024\002\004\010\367D\201\362\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377\067g\302\377\016\022\030\377bdh\377\204\204\205\377,/\065\377\036"
  "/N\377G\206\374\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H"
  "\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377\014\025'\361\000\000\000D\000\000\000e\027,S\364H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377A{\346\377*N\223\377%By\377"
  "\063_\263\377H\207\376\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377(L\217\376\005\010\020\244\004\012\021\263-U\240\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377<r\326\377\003"
  "\007\015\351\004\010\020\345<r\325\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "\065d\273\377\020\026\"\377\022\027\"\377\022\027\"\377\022\027\"\377\022\027\"\377"
  "\022\027\"\377!\071e\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\207\376\377\002\004\007\371\001\002\003\373F\205\371\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377\062^\260\377;>D\377\335\335\335\377\335"
  "\335\335\377\335\335\335\377\335\335\335\377\230\230\230\377!\062P\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377\012\023#\360\002\003\006\372H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377\062^\260\377DHO\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\254\254\254\377\"\063S\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377\017\034\064\357\002\004\007\371"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377\062^\260\377)+/\377"
  "\231\231\231\377\317\317\317\377\377\377\377\377\377\377\377\377\252\252"
  "\252\377\"\064U\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377\020\036\067\357\001\001\002\375G\206\374\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377;o\320\377$<f\377\023\036\061\377\206\206\206"
  "\377\377\377\377\377\377\377\377\377\250\250\250\377#\065X\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377\014"
  "\025(\360\003\006\015\354>v\335\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377%C{\377\204\204\204\377\377\377\377\377\377\377"
  "\377\377\245\245\245\377#\067[\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377\003\006\013\367\005\013\022\301\061\\\254"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "$C}\377\202\202\202\377\377\377\377\377\377\377\377\377\242\242\242\377#"
  "\067]\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377@y\342\377\003\005\012\361\002\002\002t\035\066e\367H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377$D\177\377\200\200\200\377\377\377"
  "\377\377\377\377\377\377\240\240\240\377$\071`\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377-U\237\377\004\011\021\264\000\000\000\040"
  "\004\007\017\366G\205\372\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377$D\200\377~\177\177\377\377\377\377\377\377\377\377\377\235\235\235"
  "\377$:b\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377\021\040<\362\000\000\000V\000\000\000\000\004\011\021\310+S\234\376H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377G\206\374\377\037;n\377{{|\377\377\377\377\377\377"
  "\377\377\377\233\233\233\377\024!\070\377%D\177\377\"<n\377H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377;o\320\377\003\005\013\360\000\000\000\011\000\000\000\000\000\000\000\065\005\011\022\365"
  "E\203\365\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377<r\325\377\012\016\024\377&)\060\377\260\261\261"
  "\377\377\377\377\377\377\377\377\377\273\273\273\377acg\377xxx\377\010\012"
  "\016\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377\021\040>\364\002\002\005j\000\000\000\000\000\000\000\000"
  "\000\000\000\000\004\011\020\264$D\177\373H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377\067i\304\377/\063:\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\361\361\361\377\010\012\016\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377\063`\264"
  "\377\003\007\015\344\000\000\000\005\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\034\003\005\013\356\063a\266"
  "\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377\066f\300\377\063\070?\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\361\361\361\377\010\012"
  "\016\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377>u\334\377\004\007\017\365\000\000\000=\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\071\003\006\014\365<r\325\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377H\210\377\377\065e\275\377\005\007\011\377\030\034\"\377\031\034\""
  "\377\032\035\"\377\032\035\"\377\037#)\377'+\063\377%(\060\377\007\011\016\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377D\200\360\377"
  "\011\022!\365\002\005\012k\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\003\003\006[\004\007\017\365\065e\275\377H\210\377\377H\210\377\377H\210\377\377H"
  "\210\377\377E\203\366\377>v\335\377>v\335\377>v\335\377>v\335\377>v\335\377"
  "=s\327\377:m\314\377:m\314\377:n\317\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377>u\334\377\011\021\040\364\004\007\016\221\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000@\003\005\013\361(L\217"
  "\374G\206\374\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210"
  "\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377\063a\265"
  "\377\004\010\017\365\002\005\007i\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\"\004\010\017\314\011\022\"\363\063"
  "a\265\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377"
  "H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377H\210\377\377:n\317\377\021!=\363\003\007\014\343\000\000\000=\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000L\003\007\016\340\013\024&\361(K\216\376<q\324\377H"
  "\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377\377H\210\377"
  "\377@y\342\377-T\236\377\021\040<\362\003\005\013\360\002\002\005j\000\000\000\004\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\002\000\000\000C\003\010\016\242\004"
  "\010\016\350\002\003\007\372\012\022\"\361\017\034\065\356\020\036\067\357\013\024&\360\003\006"
  "\013\367\003\005\012\361\004\011\021\264\000\000\000V\000\000\000\010\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000",
};

