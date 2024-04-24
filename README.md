# Game Server
## 1. 소개
본 프로젝트는 C++로 구현한 MMORPG 게임서버이다. 프로젝트에는 IOCP와 JobQueue를 사용해 다수의 스레드를 효율적으로 처리했으며, Protobuf를 사용한 직렬화를 통해 네트워크 통신의 최적화를 이루었다.<br><br>
현재 페이지의 프로젝트는 클라이언트-서버 모델 중 서버에 해당하며 클라이언트 역할은 아래의 프로젝트에 있다.<br>
[https://github.com/pyh4456/Unreal-Engine-Client](https://github.com/pyh4456/Unreal-Engine-Client)<br>

## 2. 시연 영상
[![Video Label](http://img.youtube.com/vi/73F0t-Co-v4/0.jpg)](https://youtu.be/73F0t-Co-v4)

## 3. 사용 기술
#### -IOCP, winsock2
#### -JobQueue, Job
#### -Protobuf

## 4. 시스템 구성
### Data flow
![GameServerSystem drawio](https://github.com/pyh4456/GameServer/assets/62279820/98846dba-c8e3-45ab-b90f-482726341934)

## 5. 사용법
### 서버
1. 현재 레포지토리의 파일을 다운받아 Server.sln 솔루션을 실행.
2. 솔루션 내의 DummyClient와 GameServer 프로젝트를 빌드.
3. GameServer프로젝트를 실행.

### 클라이언트
1. 상단의 소개에 있는 클라이언트 프로젝트를 다운받아 언리얼 엔진을 이용해 빌드하거나, 패키지된 파일을 받아서 실행한다.
2. 서버가 열려있는 상태라면 로그인 버튼으로 접속 가능  
