# Game Server
## 1. 소개
본 프로젝트는 C++로 구현한 MMORPG 게임서버이다. 프로젝트에는 IOCP와 JobQueue를 사용해 다수의 스레드를 효율적으로 처리했으며, Protobuf를 사용한 직렬화를 통해 네트워크 통신의 최적화를 이루었다.<br><br>
현재 페이지의 프로젝트는 클라이언트-서버 모델 중 서버에 해당하며 클라이언트 역할은 아래의 프로젝트에 있다.<br>
[https://github.com/pyh4456/Unreal-Engine-Client](https://github.com/pyh4456/Unreal-Engine-Client)<br>

## 2. 사용법
### 서버
1. 현재 레포지토리의 파일을 다운받아 Server.sln 솔루션을 실행.
2. 솔루션 내의 GameServer 프로젝트를 빌드 및 실행.

### 클라이언트
[https://github.com/pyh4456/Unreal-Engine-Client](https://github.com/pyh4456/Unreal-Engine-Client)<br>
1. 위의 레포지토리의 언리얼 프로젝트를 다운받아 언리얼 엔진(UE5.3 필요)을 이용해 빌드하거나, 패키지된 파일을 다운 받아서 실행한다.
2. 서버가 실행되어 있는 상태라면 로그인 버튼으로 접속 가능 (이름은 임의의 것을 사용한다. 비밀번호는 입력하지 않아도 된다.)
#### 조작법
 - 마우스 이동: 시선 이동
 - W, A, S, D: 캐릭터 이동
 - Space bar: 캐릭터 상승
 - C: 캐릭터 하강
 - Shift: 이동속도 상승
 - 마우스 오른쪽 클릭: 조준
 - 마우스 왼쪽 클릭(조준 상태에서): 총알 발사 

### 더미
1. Server.sln 솔루션의 DummyClient프로젝트를 빌드한다.
2. 프로젝트 폴더 내의 \Binaries\Debug\DummyClient.exe를 실행한다.
3. 터미널에 추가하고 싶은 더미의 개수를 입력한다. (200까지 동작 확인)
4. 서버가 실행되어 있는 상태라면 "OnConnected"라는 메세지가 터미널에 더미의 개수만큼 출력된다.
5. 더미를 끄고 싶다면 터미널에 "quit"을 입력한 후 터미널 창을 닫는다.
