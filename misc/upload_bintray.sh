#!/bin/bash

# Upload deb file to bintray.com script
# 
# == Example ==
#
# The example below shows how to upload a picture called sweeping_the_rock.png 
# to a Picasa Web Album with the title "Sweeping the rock":
#
# == Example Code ==
# curl --silent --request POST --data-binary "@sweeping_the_rock.png" 
# --header "Slug: Sweeping the rock" 
# --header "Content-Type: image/png" 
# --header "Authorization: GoogleLogin auth=ABCDEFG" 
#          "http://picasaweb.google.com/data/feed/api/user/brad.gushue/albumid/5113621341847124417" | tidy -xml -indent -quiet

# Load version from configure.ac
VERSION=`cat ../configure.ac | grep AC_INIT | awk '{print $2}' | sed -e 's/\[\|\]\|,//g'`
echo "JaneClone version is ${VERSION}"

FILENAME="janeclone_${VERSION}_amd64.deb"
echo "File name is ${FILENAME}"

if ! test -e ../${FILENAME}; then
    echo "${FILENAME} is not created abort...";
    exit 1;
fi

CURL="curl -v --user hiroyuki-nagata:${BINTRAY_TOKEN}"
METHOD="-X PUT --data-binary @- "
API="https://api.bintray.com"
RESOURCE="/content/hiroyuki-nagata/deb/JaneClone/${VERSION}/${FILENAME}?override=1?publish=1"

# Here upload executing !
cat ../${FILENAME} | ${CURL} ${METHOD} ${API}${RESOURCE}
