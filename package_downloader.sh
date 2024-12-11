#!/bin/bash

# OS 감지
OS=$(uname)

if [[ "$OS" == "Linux" ]]; then
    echo "Linux 개발 환경 설정 중..."
    sudo apt update
    sudo apt install -y libjson-c-dev libssl-dev inotify-tools sqlite3 libsqlite3-dev \
        qtbase5-dev qtcreator build-essential \
        libqt5network5 libqt5networkauth5-dev \
        libqt5sql5 libqt5sql5-sqlite \
        libfontconfig1 mesa-common-dev

elif [[ "$OS" == "Darwin" ]]; then
    echo "macOS 개발 환경 설정 중..."
    # Homebrew 설치 확인
    if ! command -v brew &> /dev/null; then
        echo "Homebrew가 없습니다. 설치 중..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi

    brew update
    brew install json-c openssl sqlite qt
    brew install --cask qt-creator

elif [[ "$OS" == "MINGW"* || "$OS" == "CYGWIN"* ]]; then
    echo "Windows 개발 환경 설정 중..."
    echo "다음 패키지를 수동으로 설치하세요:"
    echo "- JSON-C (소스에서 빌드)"
    echo "- OpenSSL (https://slproweb.com/products/Win32OpenSSL.html)"
    echo "- SQLite (https://sqlite.org/download.html)"
    echo "- Qt (https://www.qt.io/download)"
else
    echo "지원되지 않는 운영 체제입니다."
fi

echo "개발 환경 설정 완료!"

