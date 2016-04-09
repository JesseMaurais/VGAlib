
#define debug(condition)\
	if (!(condition))\
	 printf("debug \"%s\"(%d): %s\n", __FILE__, __LINE__, #condition);\
	else

#define setsafe(arg, value)\
	if (arg) *arg = value;
