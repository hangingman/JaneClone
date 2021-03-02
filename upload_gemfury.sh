#!/bin/bash
#
# Upload deb file to gemfury script
# ref https://gemfury.com/help/upload-packages
#
#
# Load version from version.txt
MAIL=idiotpanzer@gmail.com
VERSION=$(awk '{print $2}' ./version.txt | paste -s -d"." -)
PUSH_URL="https://${GEMFURY_TOKEN}@push.fury.io/hangingman/"

function upload_deb()
{
    echo "Prepare GemFury..."
    gem install gemfury
    echo "JaneClone version is ${VERSION}"
    # Package
    DEB_FILENAME="janeclone_${VERSION}_amd64.deb"
    echo "File name is ${DEB_FILENAME}"

    if ! [ -e ./build/${DEB_FILENAME} ]; then
        echo "${DEB_FILENAME} is not created abort...";
        exit 1;
    fi

    expect -c "
    spawn fury login
    expect -re \".*Email: \" {
      send \"${MAIL}\"
      send \r
      expect -re \".*Password:.*\" {
        send \"${GEMFURY_PASSWD}\"
        send \r
        expect eof exit 0
      }
    }
    "
    if [ $(fury versions "janeclone" | grep ${VERSION} | wc -l) == "1" ]; then
	# パッケージ削除
	fury yank "janeclone" -v ${VERSION};
    fi

    # .deb file
    # curl -F package=@<Package File> <Push-URL>
    curl -F package=@./build/${DEB_FILENAME} ${PUSH_URL}
}

function upload_dmg()
{
    #
    # Here upload executing !
    #
    echo "JaneClone version is ${VERSION}"

    # Package
    DMG_FILENAME="janeclone-osx64-${VERSION}.dmg"

    # .dmg file
    RESOURCE="/content/hiroyuki-nagata/generic/JaneClone/${VERSION}/${DMG_FILENAME};publish=1?override=1"
    cat ../${DMG_FILENAME} | ${CURL} ${METHOD} ${API}${RESOURCE}
}

if [ "$TRAVIS_OS_NAME" == "linux" ]; then
    echo "--> Here we upload debian package file...";
    upload_deb || exit 1;
    echo "--> Finish !";
fi
# if [ "$TRAVIS_OS_NAME" == "osx" ]; then
#     echo "--> Here we upload disk image file for osx..."
#     upload_dmg || exit 1
#     echo "--> Finish !"
# fi
