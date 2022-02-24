NAME = webserv

INCLUDE_DIRS = src/

CFLAGS += -O3				# I NEED SPEEEED
#CFLAGS += -O0 -g3 -ggdb		# for debugging purposes
#CFLAGS += -D NDEBUG		# remove assert calls
CFLAGS += -ferror-limit=2	# usually only the first or second errors are usefull, the rest is just junk
