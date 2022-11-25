/* GIMP RGBA C-Source image dump (view_search.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  guint8 	 pixel_data[32 * 32 * 4 + 1];
} view_search = {
  32, 32, 4,
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\020\000\000\000]\017\017\017\251\021\021\021\334\010\010\010\365\000\000\000\377"
  "\000\000\000\377\006\006\006\367\021\021\021\340\021\021\021\260\000\000\000e\000\000\000\027\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\036\017"
  "\017\017\251\010\010\010\367JJJ\362\230\230\230\377\313\313\313\377\355\355\355"
  "\377\377\377\377\377\377\377\377\377\361\361\361\377\317\317\317\377\237"
  "\237\237\377TTT\364\011\011\011\367\017\017\017\267\000\000\000)\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\014\011\011\011\221\011\011\011\365{{{\371\357\357\357"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\364\364\364\377\213\213\213\374\015\015\015\366\020\020\020"
  "\244\000\000\000\024\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000!\015\015\015\335ZZZ\367\350\350\350\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\367\367\367\377\337"
  "\337\337\377\323\323\323\377\353\353\353\377\377\377\377\377\377\377\377"
  "\377\357\357\357\377mmm\371\013\013\013\350\000\000\000.\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000:\013\013\013\361\222\222"
  "\222\374\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\264\264"
  "\264\377IJJ\377\006\010\007\377\000\000\000\377\000\000\000\377\000\000\000\377!$#\377zzz\377\354"
  "\354\354\377\377\377\377\377\247\247\247\376\014\014\014\365\003\003\003M\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000%\013\013\013\362\267"
  "\267\267\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\355\355\355\377FHG\377"
  "\000\000\000\377\003!\030\377\001uQ\377\001\241o\377\001\251u\377\001\220c\377\001L\065\377\001"
  "\004\003\377\014\017\016\377\254\254\254\377\377\377\377\377\312\312\312\377\013"
  "\013\013\365\000\000\000\066\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\016\016\016"
  "\016\340\226\226\226\375\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\356\356\356"
  "\377&)(\377\002\010\006\377\001\224f\377\002\373\255\377\002\377\260\377\002\377\260\377"
  "\002\377\260\377\002\377\260\377\002\377\260\377\002\330\225\377\003;)\377\001\001\001\377"
  "\233\233\233\377\377\377\377\377\266\266\266\376\013\013\013\356\000\000\000\035\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\016\016\016\245fff\370\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377JKJ\377\002\010\006\377\002\301\205\377\002\377\260"
  "\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377"
  "\002\377\260\377\002\377\260\377\002\371\254\377\002L\065\377\004\005\005\377\312\312\312"
  "\377\377\377\377\377\201\201\201\373\020\020\020\301\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000)\017\017\017\365\360\360\360\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\270\270\270\377\000\000\000\377\001\221d\377\002\377\260\377\002\377\260\377\002\377"
  "\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260"
  "\377\002\377\260\377\002\377\260\377\002\362\247\377\003\032\023\377;=<\377\377\377"
  "\377\377\372\372\372\377\032\032\032\363\000\000\000D\000\000\000\000\000\000\000\000\017\017\017\275"
  "\216\216\216\374\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "TTT\377\003\033\024\377\002\372\255\377\002\377\260\377\002\377\260\377\002\377\260\377"
  "\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002"
  "\377\260\377\002\377\260\377\002\377\260\377\001\221d\377\000\000\000\377\330\330\330"
  "\377\377\377\377\377\251\251\251\377\016\016\016\330\000\000\000\001\000\000\000!\014\014\014"
  "\367\370\370\370\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\014\017\016\377\001hH\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377"
  "\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260"
  "\377\002\377\260\377\002\377\260\377\002\377\260\377\002\343\235\377\000\000\000\377\222"
  "\222\222\377\377\377\377\377\376\376\376\377\034\034\034\363\000\000\000:\004\004\004xe"
  "ee\370\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\360\360\360"
  "\377\000\000\000\377\001\217c\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377"
  "\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260"
  "\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\003\014\011\377"
  "ttt\377\377\377\377\377\377\377\377\377\201\201\201\374\014\014\014\230\022\022"
  "\022\306\257\257\257\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\357\357\357\377\000\000\000\377\001\217c\377\002\377\260\377\002\377\260\377\002\377"
  "\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260"
  "\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377"
  "\003\015\012\377ttt\377\377\377\377\377\377\377\377\377\311\311\311\377\020\020"
  "\020\341\012\012\012\361\343\343\343\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\013\015\014\377\001jI\377\002\377\260\377\002"
  "\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377"
  "\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260"
  "\377\002\346\237\377\000\000\000\377\220\220\220\377\377\377\377\377\377\377\377"
  "\377\371\371\371\377\004\004\004\374\005\005\005\373\376\376\376\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377QQQ\377\003\036\026\377"
  "\002\373\255\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002"
  "\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377"
  "\260\377\002\377\260\377\001\226h\377\000\000\000\377\325\325\325\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\030\030\030\363\017\017\017\365\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\263\263\263\377\000\000\000\377\001\227h\377\002\377\260\377\002\377\260\377\002\377\260"
  "\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377"
  "\002\377\260\377\002\377\260\377\002\364\250\377\003\035\025\377\066\070\067\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377'''\360\020\020\020"
  "\365\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377CED\377\002\012\010\377\002\311\213\377\002\377"
  "\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260\377\002\377\260"
  "\377\002\377\260\377\002\377\260\377\002\373\255\377\001S:\377\003\004\004\377\306\306"
  "\306\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "(((\360\005\005\005\372\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\353\353\353\377\040#\"\377"
  "\002\013\010\377\001\236m\377\002\375\257\377\002\377\260\377\002\377\260\377\002\377\260"
  "\377\002\377\260\377\002\377\260\377\002\337\232\377\002C/\377\000\000\000\377\223\223"
  "\223\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\031\031\031\362\010\010\010\364\346\346\346\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\350\350\350\377<>>\377\000\000\000\377\003*\036\377\001\177X\377\001"
  "\255w\377\001\266~\377\001\233k\377\001W<\377\002\007\005\377\010\012\011\377\243\243\243"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\373\373\373\377\004\004\004\373\023\023\023\315\264\264\264\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\376\376\376\377\064\064\064\377\063"
  "\065\065\377\001\002\002\377\000\000\000\377\000\000\000\377\000\000\000\377\027\032\031\377ooo\377\346"
  "\346\346\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\316\316\316\377\016\016\016\346\006\006\006\200"
  "lll\371\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\341\341\341\377\025\025"
  "\025\377\360\360\360\377\017\017\017\377\304\304\304\377\306\306\306\377\341"
  "\341\341\377\376\376\376\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\210\210\210\375\015\015\015\237\000\000\000)\021\021\021\366\373\373\373"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\207\207\207\377iii\377\246\246\246\377"
  "KKK\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377%%%\362\000\000\000C\000\000\000"
  "\000\017\017\017\312\234\234\234\376\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377///\377\301\301"
  "\301\377OOO\377\242\242\242\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\266\266\266"
  "\377\016\016\016\342\000\000\000\002\000\000\000\000\000\000\000\065\022\022\022\365\364\364\364\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\327"
  "\327\327\377\032\032\032\377\357\357\357\377\020\020\020\377\360\360\360\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\374\374\374\377$$$\364\000\000\000N\000\000\000\000\000\000\000\000\000\000\000\000\020\020"
  "\020\263www\372\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377~~~\377sss\377\240\240\240\377QQQ\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\223\223\223\375\017\017\017\315\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\027\013\013\013\353\250\250\250\376\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377&&&\377\312\312\312\377III\377\250\250\250\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\304\304\304\377\013\013\013\361\000\000\000%\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\061\013\013\013\364\311\311\311\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\314"
  "\314\314\377###\377\355\355\355\377\020\020\020\377\363\363\363\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\332\332"
  "\332\377\020\020\020\365\000\000\000D\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\003\003\003L\013\013\013\364\252\252\252\376\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377uuu\377www\377\234"
  "\234\234\377UUU\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\274\274\274\377\017\017\017\365\005\005\005b\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\060\013\013\013\351"
  "sss\371\363\363\363\377\377\377\377\377\377\377\377\377\377\377\377\377p"
  "pp\377\017\017\017\377\023\023\023\377\254\254\254\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\371\371\371\377\206\206\206\374\013\013\013\361\000\000\000?\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\026\017\017\017\256\020\020\020\365\225\225\225\375\372\372\372"
  "\377\377\377\377\377\377\377\377\377\363\363\363\377\245\245\245\377\366"
  "\366\366\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\375\375\375\377\245\245\245\377\030\030\030\365\017\017\017"
  "\275\000\000\000\040\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\061\020\020\020\303"
  "\015\015\015\366fff\370\260\260\260\377\344\344\344\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\351\351\351\377\266\266\266"
  "\377ooo\371\023\023\023\365\017\017\017\320\000\000\000=\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\"\004\004\004w\023\023\023\305"
  "\013\013\013\360\005\005\005\372\020\020\020\365\021\021\021\365\006\006\006\371\010\010\010\363"
  "\023\023\023\314\006\006\006\177\000\000\000*\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000",
};
