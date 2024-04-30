# Game Server
## 1. 소개
본 프로젝트는 C++로 구현한 MMORPG 게임서버이다. 프로젝트에는 IOCP와 JobQueue를 사용해 다수의 스레드를 효율적으로 처리했으며, Protobuf를 사용한 직렬화를 통해 네트워크 통신의 최적화를 이루었다.<br><br>
현재 페이지의 프로젝트는 클라이언트-서버 모델 중 서버에 해당하며 클라이언트 역할은 아래의 프로젝트에 있다.<br>
[https://github.com/pyh4456/Unreal-Engine-Client](https://github.com/pyh4456/Unreal-Engine-Client)<br>

## 2. 시연 영상
[![Video Label](http://img.youtube.com/vi/73F0t-Co-v4/0.jpg)](https://youtu.be/73F0t-Co-v4)

## 3. 사용 기술
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

## 4. 시스템 구성
![GameServerSystem drawio](https://github.com/pyh4456/GameServer/assets/62279820/73bc0a8f-392a-4cf1-9cad-9a3a211a2af0)

서버의 구성요소는 크게 세가지 IocpObejct, Service, JobQueue로 나누어지며 각각의 세부 기능은 다음과 같다.
### 4.1 IocpObject
클라이언트와의 소켓 통신을 관리하며, 소켓으로부터 데이터를 받으면 데이터에 해당하는 IocpEvent를 Service에 전달한다. IocpObejct중 Listener는 서버에 연결을 시도하는 클라이언트를 받아주며, 클라이언트가 접속했을때 AcceptEvent를 Service에 전달한다.
Service가 AcceptEvent를 받으면 Session을 만들어 IOCP에 등록하며, Session을 통해 클라이언트와 Service가 서로 IocpEvnet를 주고 받을 수 있다.<br>
### 4.2 Service
IOCP에서 IocpEvent를 전달받아 해당 IocpObject가 받은 데이터를 처리한다. Listener로부터 이벤트를 받았다면 Session을 생성해 IOCP에 등록하고, Session으로부터 이벤트를 받았다면 이벤트 종류에 따라 Job을 생성해 JobQueue에 등록한다. 
### 4.3 JobQueue 
본 프로젝트에서는 JobQueue를 상속받은 Room 클래스가 게임에서의 맵 역할을 한다. 
JobQueue는 전달받은 Job을 순차적으로 처리하며, Job을 처리하는 규칙은 다음과 같다. (여기서 Job을 실행하는 권한을 가진 스레드를 worker라고 한다.)
1. 임의의 스레드가 Job을 등록하러 갔을때, worker가 없으면 해당 스레드가 Job의 실행 권한을 가진다. worker가 있다면 등록만 하고 빠져나온다.
2. worker는 JobQueue에 쌓여있는 Job이 없어질 때 까지 Job을 실행하며, 너무 많은 Job이 등록되면 현재까지 쌓인 Job만 처리하며 worker자리를 비워놓는다.

## 5. 사용법
### 서버
1. 현재 레포지토리의 파일을 다운받아 Server.sln 솔루션을 실행.
2. 솔루션 내의 DummyClient와 GameServer 프로젝트를 빌드.
3. GameServer프로젝트를 실행.

### 클라이언트
1. 상단의 소개에 있는 클라이언트 프로젝트를 다운받아 언리얼 엔진을 이용해 빌드하거나, 패키지된 파일을 받아서 실행한다.
2. 서버가 열려있는 상태라면 로그인 버튼으로 접속 가능  
