
iOS LiveMapClient API
=======

[LiveMapServer](https://github.com/interruping/livemap-server)와 연동을 위한 API 라이브러리.
LiveMapServer가 제공하는 기능을 쉽게 사용할 수 있는 API 라이브러리 입니다.
LiveMapServer와 연결하여 자신의 서비스를 구축하고 싶은 iOS Application 프로젝트에 서브모듈로
이 프로젝트를 추가해 사용할 수 있습니다.

기능
========

LiveMapServer와 SSL 소켓으로 암호화된 통신을 합니다.

LiveMapServer로부터 특정한 ID를 부여받을 수 있습니다. ID는 32bit 정수 값이고
접속 할때마다 값이 달라질 수 있습니다.

LiveMapServer로 GPS정보를 업로드할 수 있습니다. LiveMapServer로 GPS 정보를 업로드
하게 되면 LiveMapServer는 해당 GPS로부터 가까운 위치에 다른 클라이언트의 정보를 알려줍니다.
LiveMapServer가 알려주는 타 클라이언트들의 정보는 가장 최근에 업데이트된 정보입니다.

LiveMapServer에 접속하고 있는 다른 클라이언트로 메시지를 전송할 수 있고, 다른 클라이언트
가 내 자신으로 송신한 메시지도 수신할 수 있습니다.


iOS LiveMapClient API를 iOS 프로젝트에 Import하기
=======

1. LiveMapClient API를 사용할 xcodeproj가 git을 사용하고 있다면, 해당
프로젝트의 루트 디렉토리에서 submodule로 불러옵니다.

        $ git submodule add https://github.com/interruping/ios_livemap_client_api.git

2. git을 사용하고 있지 않다면 git clone으로 해당프로젝트의 루트 디렉토리로 불러옵니다.

        $ git clone https://github.com/interruping/ios_livemap_client_api.git

3. Xcode에서 프로젝트의 폴더를 우측클릭하여 'Add Files to [프로젝트 이름]'을 클릭합니다.

![Add Files to](https://user-images.githubusercontent.com/29074678/31337096-bc9917ae-ad34-11e7-836d-7d2343588bbd.png)

4. 이전에 추가된 ios_livemap_client_api 폴더 하위 폴더 src 하위에 있는 소스코드를 전부 선택하고 Add 를 눌러줍니다.

![Select All files](https://user-images.githubusercontent.com/29074678/31337237-5e12cb98-ad35-11e7-9ea9-b32fea3ad997.png)

5. 프로젝트의 폴더에 ios_livemap_client_api의 소스코드들이 추가 되었는지 확인합니다.

![check source](https://user-images.githubusercontent.com/29074678/31337309-b34df51a-ad35-11e7-98c3-a70dc6b2e819.png)


iOS LiveMapClient API를 업데이트하기
=======

iOS LiveMapClient API를 사용하는 Xcode 프로젝트가 git을 사용하고 있고, ios_livemap_client_api를 submodule로 추가하였다면
ios_livemap_client_api의 부모 프로젝트에서 submodule update git 명령을 실행합니다.

    $ git submodule update ios_livemap_client_api
    
프로젝트가 ios_livemap_client_api를 단독으로 clone 한 경우, ios_livemap_client_api 폴더로 이동하여 Release 브랜치를 pull 리모트
로부터 pull해줍니다.

    $ git pull origin Release



