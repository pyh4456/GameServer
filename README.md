# Game Server
## 1. 소개
본 프로젝트는 C++로 구현한 MMORPG 게임서버이다. 프로젝트에는 IOCP와 JobQueue를 사용해 다수의 스레드를 효율적으로 처리했으며, Protobuf를 사용한 직렬화를 통해 네트워크 통신의 최적화를 이루었다.<br><br>
현재 페이지의 프로젝트는 클라이언트-서버 모델 중 서버에 해당하며 클라이언트 역할은 아래의 프로젝트에 있다.<br>
[https://github.com/pyh4456/Unreal-Engine-Client](https://github.com/pyh4456/Unreal-Engine-Client)<br>

## 2. 시연 영상
## 3. 사용 기술
#### -IOCP, winsock2
#### -JobQueue, Job
#### -Protobuf

## 4. 시스템 구성
### Data flow
![GameServerSystem drawio](https://github.com/pyh4456/GameServer/assets/62279820/98846dba-c8e3-45ab-b90f-482726341934)

## 5. 사용법
