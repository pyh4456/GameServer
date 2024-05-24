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

## 3. 시연 영상
[![Video Label](http://img.youtube.com/vi/m0CNrVMPunQ/0.jpg)](https://youtu.be/m0CNrVMPunQ)

## 4. 사용 기술
#### -IOCP, winsock2
네트워크 통신을 위해 winsock2 라이브러리의 소켓을 사용했으며, 여러 소켓 통신을 Non-blocking으로 처리하기 위해 IOCP를 채용했다.<br>
IOCP에 관여하는 객체는 Listener와 Session 이며 두 객체는 각각 다음의 역할을 맏는다.
 - Listener: 소켓을 생성해 클라이언트의 서버 접속을 받아준다.
 - Session: Listener에서 생성된 소켓을 가지고 있으며, 해당 소켓에서 오는 패킷을 처리한다.
 
#### -JobQueue, Job
IOCP 모델은 멀티스레딩에서 효율적으로 작동하기 때문에 서버 내부에서도 멀티스레딩을 통해 작업을 처리한다. 이를위해 IOCP모델처럼 비동기적으로 작업을 처리할 수 있게 JobQueue를 정의했다.
JobQueue는 함수객체인 Job의 대기열이며, 서버의 각 스레드들이 JobQueue에 Job을 넣으면 함수를 처리하는 스레드가 Job을 소모하게 된다. 이때, 하나의 스레드에 너무 많은 Job이 몰리지 않도록 하나의 스레드가 일정 시간이상 Job을 처리한다면 다른 스레드가 Job을 가져가게끔 설계되어 있다.

#### -Protobuf
Protobuf는 구조화된 데이터를 컴팩트하게 직렬화 해 네트워크 전송을 빠르게 해준다. 본 프로젝트에서는 각 프로토콜을 헤더로 사용해 프로토콜별로 처리하는 함수를 매칭할 수 있는 PacketHandler를 정의해서 사용한다.

## 5. 시스템 구성
![GameServerSystem drawio (1)](https://github.com/pyh4456/GameServer/assets/62279820/a3aa9753-9df7-461e-92c5-cb043e38663e)

서버의 구성요소는 크게 세가지 Service, IocpObejct, JobQueue로 나누어지며 각각의 세부 사항은 다음과 같다.
### 5.1 Service
![Service](https://github.com/pyh4456/GameServer/assets/62279820/e3f70c45-8ac5-4eb0-9921-3becb0872da9)

서버에서 전반적인 네트워크 기능을 담당한다. 서비스에는 IOCP에 접근하기 위한 IocpCore가 있으며 IocpObject는 IocpCore를 통해 Completion Queue를 사용한다. NetAddress는 서버 소켓의 주소를 담고 있으며, 생성된 IocpObject는 NetAddress의 주소를 받아서 바인딩한다.

### 5.2 IocpObject
![IocpObject 계층구조](https://github.com/pyh4456/GameServer/assets/62279820/73284d82-598b-4c88-9ff7-1a36b77a26ea)

클라이언트와의 소켓 통신을 관리하며, 소켓으로부터 데이터를 받으면 데이터에 해당하는 IocpEvent를 Completion Queue에 등록. IocpObejct중 Listener는 서버에 연결을 시도하는 클라이언트를 받아주며, 클라이언트가 접속했을때 AcceptEvent를 Service에 전달한다.
Service가 AcceptEvent를 받으면 Session을 만들어 IOCP에 등록한다. 이후 Session이 클라이언트로부터 IocpEvent를 수신하면 클라이언트의 요청에 맞는 Job을 생성해 JobQueue에 입력한다.<br>
### 5.3 JobQueue
![JobQueueFlowchart drawio](https://github.com/pyh4456/GameServer/assets/62279820/f8f3c97e-9a9d-428c-bb2b-9388f6cf488f)

본 프로젝트에서는 JobQueue를 상속받은 Room 클래스가 게임에서의 맵 역할을 한다. 
JobQueue는 전달받은 Job을 순차적으로 처리하며, Job을 처리하는 규칙은 다음과 같다. (여기서 Job을 실행하는 권한을 가진 스레드를 worker라고 한다.)
1. 임의의 스레드가 Job을 등록하러 갔을때, worker가 없으면 해당 스레드가 Job의 실행 권한을 가진다. worker가 있다면 등록만 하고 빠져나온다.
2. worker는 JobQueue에 쌓여있는 Job이 없어질 때 까지 Job을 실행하며, 너무 많은 Job이 등록되면 현재까지 쌓인 Job만 처리하며 worker자리를 비워놓는다.
