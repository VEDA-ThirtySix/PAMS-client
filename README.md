<div align="center">
  <img src="https://github.com/user-attachments/assets/cc551581-4eb3-4cb8-928e-86e0ae65b1fa" alt="PAMS_LOGO" width="200"/>

</div>
  
<div align="center">

  
## Parking Area Management System(PAMS)
> **VEDA 1기 - Final Project**
> 
> **라즈베리 파이 카메라를 활용한 주차장 관리 시스템**

> **A Parking Management System Utilizing a Raspberry Pi Camera**

![C](https://img.shields.io/badge/-C-A8B9CC?style=flat-square&logo=c&logoColor=white) 
![C++](https://img.shields.io/badge/-C++-00599C?style=flat-square&logo=cplusplus&logoColor=white) 
![Python](https://img.shields.io/badge/-Python-3776AB?style=flat-square&logo=python&logoColor=white)
![Qt Creator](https://img.shields.io/badge/-Qt%20Creator-41CD52?style=flat-square&logo=qt&logoColor=white)
![Ubuntu](https://img.shields.io/badge/-Ubuntu-E95420?style=flat-square&logo=ubuntu&logoColor=white)  
![Raspbian](https://img.shields.io/badge/-Raspbian-C51A4A?style=flat-square&logo=raspberry-pi&logoColor=white) 

</div>


## TEAM
- 송현준 (Song Hyun Zun) [Github] (https://github.com/Song-H-J) email: 
- 김태원 (Kim Tae Won) [Github] (https://github.com/kimtae9217) email: 
- 지형근 (Ji Hyeng Gun) [Github] (https://github.com/hyeonggeun0209) email:
- 신기윤 (Shin Ki yun) [Github] (https://github.com/kiyuniii) email:
- 김시현 (Kim Si Hyun) [Github] (https://github.com/kongshyun) email: tlgus927@naver.com



# 목차
- [TEAM](#TEAM)
- [프로젝트 배경 및 목표](#프로젝트-배경-및-목표)
- [주요 기능](#주요-기능)
- [기술 스택](#기술-스택)
- [시스템 아키텍처](#시스템-아키텍처)
- [설치 및 실행방법](#설치-및-실행방법)
- [프로젝트 구조](#프로젝트-구조)
- [팀원별 역할](#팀원별-역할)


### 프로젝트 배경 및 목표
> **배경**
> 장기 주차 문제 해결과 주차장 운영 효율성을 높이기 위한 솔루션으로 개발

> **목표**
> 라즈베리 파이 카메라와 딥러닝 기술을 활용하여 차량 번호판 인식 및 입출차 관리를 자동화하고, 실시간 데이터 전송과 입주민 관리를 통해 주차장의 운영 효율성을 극대화하는 것을 목표로 한다.


## 주요 기능 
**실시간 영상 스트리밍**
- Raspberry Pi Camera로 촬영된 영상을 RTSP 프로토콜을 통해 클라이언트(Qt)로 실시간 전송.
**번호판 인식 및 데이터 처리**
- OpenCV와 TensorFlow를 활용하여 차량 번호판을 인식하고 OCR(광학 문자 인식)을 통해 텍스트를 추출 후 데이터 저장.
**사진 및 메타데이터 전송**
- 저장된 사진과 메타데이터(번호판 정보, 입출차 시간 등)를 HTTP 프로토콜로 클라이언트에 실시간 전송.
**입주민 정보 관리 및 인증**
- 클라이언트에서 입주민 정보를 저장 및 관리하며, 서버에서 입차 차량 번호판과 비교하여 입주민 여부를 판별.
**영상 기록 요청 및 스트리밍**
- 클라이언트가 특정 시간대의 기록을 요청하면, 서버에서 해당 시간의 15초 클립을 스트리밍으로 제공.


## 기술 스택
| 카테고리 | 사용기술 |
|-------|-------|
| 하드웨어 | Raspberry Pi 5, Picam |
| 미디어 스트리밍 | FFmpeg |
| 프로그래밍 환경 | Qt, VSCode |
| 프로그래밍 언어 | C, C++, python |
| 컴퓨터 비전 | OpenCV, TensorFlow |
| 통신 프로토콜 | RTSP, SRTP/DTLS, TCP/HTTP |
| 디버깅 도구 | CppCheck, Valgrind |
| 테스트 환경 | Ubuntu 24.04/22.04 LTS<br>, MacOS, Rasberry Pi OS |
| 빌드 시스템 | qmake, GNU |
| 보안 라이브러리 | OpenSSL |
| 협업 도구 | git, Notion, Slack |

## 시스템 아키텍처
![Parking System](./26-architecture.jpg)


## 설치 및 실행방법
### Linux(Client)
환경 업데이트 및 업그레이드
``` sh
- $ sudo apt update
- $ sudo apt -y upgrade
```
설치 환경으로 이동 및 패키지 설치
``` sh
- $ git clone https://github.com/VEDA-ThirtySix/RaspberryPi-5-RTSP-Client.git
- $ chmod +x download_packages
- $ ./download_packages
```
빌드 및 실행
- 방법 1: Qt Creator로 실행
    > 프로젝트 파일(cctv_36.pro)을 `Qt Creator`에서 열고, 실행 버튼을 누릅니다 (Ctrl+R).

- 방법 2: 터미널로 실행
    > Navigate to 'Working Directory'
```sh
- $ qmake           
- $ make clean
- $ make            //build
- $ ./cctv_36.pro     //Run the Client Program
```

### Linux(Server)
환경 업데이트
``` sh
- $ sudo apt update
- $ sudo apt -y upgrade
```
설치 환경으로 이동 및 패키지 설치
```sh
- $ git clone https://github.com/VEDA-ThirtySix/RaspberryPi-5-RTSP-Server.git
- $ chmod +x download_packages
- $ ./download_packages
```
빌드 및 실행
> 설치환경으로 이동 및 각각의 디렉터리에 아래의 단계들을 각각 수행
> : rtspServer, userServer, plateServer
```sh
- $ make clean
- $ make            //build
- ./cctv_36.pro     //Run the Server Program
```

### MacOS(Clinent)
환경 업데이트 및 업그레이드
```sh
- $ brew update
- $ brew install qt5
- $ git clone https://github.com/VEDA-ThirtySix/RaspberryPi-5-RTSP-Client.git
- $ chmod +x download_packages
- $ ./download_packages
```
빌드 및 실행
- 방법 1: Qt Creator로 실행
    > 프로젝트 파일(cctv_36.pro)을 `Qt Creator`에서 열고, 실행 버튼을 누릅니다 (Ctrl+R).
- 방법 2: 터미널로 실행
> 설치환경으로 이동 및 각각의 디렉터리에 아래의 단계들을 각각 수행
> : rtspServer, userServer, plateServer
```sh
  - $ qmake
  - $ make clean
  - $ make           //build
  - $ ./cctv_36.pro   //Run the Client Program
```

### MacOS(Server)
환경 업데이트 및 업그레이드
```sh
- $ brew update
- $ brew install qt5
- $ git clone https://github.com/VEDA-ThirtySix/RaspberryPi-5-RTSP-Server.git
- $ chmod +x download_packages
- $ ./download_packages
```
빌드 및 실행
> 설치환경으로 이동 및 각각의 디렉터리에 아래의 단계들을 각각 수행
> : rtspServer, userServer, plateServer
```sh
  - $ qmake
  - $ make clean
  - $ make           //build
  - $ ./cctv_36.pro   //Run the Client Program
```


### Windows(Client)
환경 업데이트 및 업그레이드
``` sh
- Download and install Qt Creator from the official website (https://www.qt.io/download).
- Clone the repository:
  - Open Command Prompt and navigate to your working directory.
  - > git clone https://github.com/VEDA-ThirtySix/RaspberryPi-5-RTSP-Client.git
```

빌드 및 실행
- 방법 1: Qt Creator로 실행
    > 프로젝트 파일(cctv_36.pro)을 `Qt Creator`에서 열고, 실행 버튼을 누릅니다 (Ctrl+R).

- 방법 2: 터미널로 실행
> 대안적으로, MinGW가 설치된 명령 프롬프트를 사용
> MinGW 명령 프롬프트를 엽니다.<br>
작업 디렉토리(Working Directory)로 이동합니다.<br>
```sh
  - > qmake
  - > mingw32-make clean
  - > mingw32-make     //build
  - > ./cctv_36.exe    //Run the Client Program
```

### Windows(Server)
환경 업데이트 및 업그레이드
```sh
- Download and install Qt Creator from the official website (https://www.qt.io/download).
- Clone the repository:
  - Open Command Prompt and navigate to your working directory.
  - > git clone https://github.com/VEDA-ThirtySix/RaspberryPi-5-RTSP-Server.git
```
빌드 및 실행
> 대안적으로, MinGW가 설치된 명령 프롬프트를 사용
> MinGW 명령 프롬프트를 엽니다.<br>
작업 디렉토리(Working Directory)로 이동합니다.<br>
설치환경으로 이동 및 각각의 디렉터리에 아래의 단계들을 각각 수행<br>
: `rtspServer`, `userServer`, `plateServer`
```sh
  - > qmake
  - > mingw32-make clean
  - > mingw32-make     //build
  - > ./cctv_36.exe    //Run the Server Program
```


## 프로젝트 구조
### Common
| 파일명 | 기능 |
|-------|-------|
| inc | 헤더 파일 |
| src | 소스 파일  |
| objs | 오브젝트 파일 |

### CLIENT
| 파일명 | 기능 |
|-------|-------|    
| model | DTO(데이터 전송 객체) 및 SQL 함수 정의 및 데이터 처리 로직 |
| view | 사용자 인터페이스(UI) 구성 및 사용자 입력 처리 |
| controller | 비즈니스 로직 제어 및 모델과 뷰 사이의 데이터 흐름 관리  | 
| network | HTTP 및 TCP 통신 구현, 서버와의 데이터 교환 관리 |

### SERVER
| 파일명 | 기능 |
|-------|-------|
| rtspServer | - 클라이언트로 RTSP 실시간 스트리밍 서버 구현<br>  - 번호판 발견 시 OCR 저장 및 클립(15초) 녹화<br> - 클립 영상 전송 처리
 |
| userServer | - 클라이언트 요청 처리 및 응답<br> -입주민 정보 데이터베이스 관리 및 저장<br> -클립 요청 파싱 및 `rtspServer`에 스트리밍 요청 전달 |
| plateServer | - `rtspServer`에서 전달받은 메타데이터(JSON) 생성 및 클라이언트로 전송 |

### Database
| 파일명 | 위치 | 데이터 | 
|-------|-------|-------|
| metadata.db | cctv_36 | Basic(입주민정보), Time(입출차정보) |
| parking.db | userServer | Time(입출차정보) |

### DTO(Data Transfer Object)
| 클래스명 | 멤버변수 |
|-------|-------|
| BasicInfo | name(이름), plate(번호판), home(동호수), phone(전화번호) |
| TimeInfo | id(식별번호), plate(번호판), time(시간), type(입출차구분), image(사진_BLOB) |


<h1 id="팀원소개"> 👨‍👩‍👧‍👧 팀원 소개</h1>

|                                               [송현준](https://github.com/Song-H-J)                                                |                                                  [지형근](https://github.com/hyeonggeun0209)                                                  |                                               [김시현](https://github.com/kongshyun)                                                |                                                   [김태원](https://github.com/kimtae9217)                                                   |                                                   [신기윤](https://github.com/kiyuniii)                                                   |
| :--------------------------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------------------------------: |
| <img width="204" alt="songHY" src="https://avatars.githubusercontent.com/u/76940288?v=4"> | <img width="204" alt="jihg" src="https://avatars.githubusercontent.com/u/55240146?v=4"> | <img width="204" alt="KimSH" src="https://avatars.githubusercontent.com/u/77137429?v=4"> | <img width="204" alt="KimTW" src="https://avatars.githubusercontent.com/u/55828162?v=4"> | <img width="204" alt="ShinKY" src="https://avatars.githubusercontent.com/u/130027851?v=4"> |
|                                                                 **Server**                                                                 |                                                                    **Server**                                                                     |                                                                    **Client**                                                                     |                                                                    **Client**                                                                     |                                                                    **Client**                                                                     |

## 팀원별 역할
- 송현준: RTSPS 서버 개발 및 영상 처리 담당.
- 지형근: 딥러닝 기반 분석 기능 구현.
- 김시현: Qt Application UI 설계 및 데이터 파싱.
- 김태원: 영상 스트리밍 플레이어 개발.
- 신기윤: 데이터베이스 설계 및 이벤트 관리.
