AUTOMAKE_OPTIONS 	= foreign subdir-objects
ACLOCAL_AMFLAGS		= -I m4

HASH_SRCS 		= hashcodes/crypto_hash.cpp hashcodes/sha1.cpp hashcodes/md5.cpp
HTTP_SRCS 		= http/httpclient.cpp http/httpcurl.cpp
AUTH_SRCS 		= oauth/base64.cpp oauth/oauth.cpp
TWCR_SRCS 		= twitter/twitter_client.cpp
UI_SRCS   		= ui/simple_ui.cpp ui/simple_terminal.cpp
SRCS	  		= main.cpp
KEYS	  		= keys/apikeys.hpp

### Lib Target
lib_LTLIBRARIES		= libctw.la
libctw_la_SOURCES	= $(HASH_SRCS) $(HTTP_SRCS) $(AUTH_SRCS) $(TWCR_SRCS) $(UI_SRCS)
libctw_la_CXXFLAGS	= @CURL_CFLAGS@ \
			  -I./hashcodes \
			  -I./http      \
			  -I./oauth     \
			  -I./twitter   \
			  -I./ui	\
			  -I./include
# libtool versioning -> http://ftp.gnu.org/pub/old-gnu/Manuals/libtool-1.4.2/html_chapter/libtool_6.html
libctw_la_LDFLAGS	= -no-undefined -version-info 0:0:0

$(KEYS):
	-@ ./write_apikeys.sh
